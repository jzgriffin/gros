# Copyright (c) 2023 Jeremiah Z. Griffin
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

from dataclasses import dataclass
from enum import Enum, auto
from typing import Iterable, Optional
import argparse
import re
import sys


class ConfigError(Exception):
    def __init__(self, message: str):
        super().__init__(message)


class Xlen(Enum):
    X32 = auto()
    X64 = auto()


class Ext(Enum):
    # Base
    I = auto()

    # Standard unprivileged
    M = auto()
    A = auto()
    F = auto()
    D = auto()
    C = auto()

    # Standard machine-level
    Zicsr = auto()
    Zifencei = auto()


class Abi(Enum):
    # XLEN=32
    ILP32 = auto()
    ILP32F = auto()
    ILP32D = auto()

    # XLEN=64
    LP64 = auto()
    LP64F = auto()
    LP64D = auto()


class Mmu(Enum):
    BARE = auto()

    # XLEN=32
    SV32 = auto()

    # XLEN=64
    SV39 = auto()
    SV48 = auto()


@dataclass
class Configuration:
    xlen: Xlen
    exts: set[Ext]
    abi: Abi
    mmu: Mmu


XLEN_ABIS: dict[Xlen, set[Abi]] = {
    Xlen.X32: {
        Abi.ILP32,
        Abi.ILP32F,
        Abi.ILP32D,
    },
    Xlen.X64: {
        Abi.LP64,
        Abi.LP64F,
        Abi.LP64D,
    },
}

XLEN_MMUS: dict[Xlen, set[Mmu]] = {
    Xlen.X32: {
        Mmu.BARE,
        Mmu.SV32,
    },
    Xlen.X64: {
        Mmu.BARE,
        Mmu.SV39,
        Mmu.SV48,
    },
}

BASE_EXTS: dict[str, set[Ext]] = {
    "I": {Ext.I},
    "G": {
        Ext.I,
        Ext.M,
        Ext.A,
        Ext.D,
        Ext.Zifencei,
    },
}

STANDARD_UNPRIVILEGED_EXTS: list[Ext] = [
    Ext.M,
    Ext.A,
    Ext.F,
    Ext.D,
    Ext.C,
]

STANDARD_MACHINE_EXTS: list[Ext] = [
    Ext.Zicsr,
    Ext.Zifencei,
]

IMPLIED_EXTS: dict[Ext, set[Ext]] = {
    Ext.F: {Ext.Zicsr},
    Ext.D: {Ext.F},
}

ABI_EXTS: dict[Abi, set[Ext]] = {
    Abi.ILP32: set(),
    Abi.ILP32F: {Ext.F},
    Abi.ILP32D: {Ext.D},
    Abi.LP64: set(),
    Abi.LP64F: {Ext.F},
    Abi.LP64D: {Ext.D},
}


def parse_xlen(spec: str) -> Xlen:
    try:
        return Xlen[f"X{spec}"]
    except KeyError:
        raise ConfigError(f"Unknown XLEN {spec}")


def format_xlen(xlen: Xlen) -> str:
    return xlen.name.removeprefix("X")


def parse_xlens(spec: str) -> set[Xlen]:
    return {parse_xlen(x) for x in spec.split(",")}


def format_xlens(xlens: Iterable[Xlen]) -> str:
    return ",".join({format_xlen(x) for x in xlens})


def get_exts_closure(exts: set[Ext]) -> set[Ext]:
    closure = set()
    for ext in exts:
        closure.update(get_ext_closure(ext))
    return closure


def get_ext_closure(ext: Ext) -> set[Ext]:
    closure = {ext}
    for implied_ext in IMPLIED_EXTS.get(ext, set()):
        closure.update(get_ext_closure(implied_ext))
    return closure


def parse_exts(spec: str) -> set[Ext]:
    if len(spec) == 0:
        raise ConfigError("Empty extension specification")

    exts: set[Ext] = set()

    # Base
    token = spec[0]
    spec = spec[1:]
    try:
        exts.update(get_exts_closure(BASE_EXTS[token]))
    except KeyError:
        raise ConfigError(f"Unknown base extension {token}")

    # Sections
    def parse_section(section: list[Ext], prefix: Optional[str] = None):
        nonlocal spec
        nonlocal exts

        while len(spec) != 0:
            # Advance to the next adequately prefixed non-underscore token.
            while len(spec) != 0 and spec[0] == "_":
                spec = spec[1:]
            if len(spec) == 0:
                return
            if prefix is None and spec[0] == "Z":
                return
            if prefix is not None and not spec.startswith(prefix):
                return

            token = spec[0]
            spec = spec[1:]

            # Prefixed extensions extend to the next underscore.
            if prefix is not None:
                while len(spec) != 0 and spec[0] != "_":
                    token += spec[0]
                    spec = spec[1:]

            try:
                ext = Ext[token]
            except KeyError:
                raise ConfigError(f"Unknown extension {token}")

            # Search the section, in order, for this token.
            while len(section) != 0 and section[0] != ext:
                section = section[1:]
            if len(section) == 0:
                raise ConfigError(f"Out-of-order extension {token}")
            section = section[1:]
            exts.update(get_ext_closure(ext))

    parse_section(STANDARD_UNPRIVILEGED_EXTS)
    parse_section(STANDARD_MACHINE_EXTS, prefix="Z")

    if len(spec) != 0:
        raise ConfigError(f"Specification has trailing extension(s) {spec}")

    return exts


def format_ext(ext: Ext) -> str:
    return ext.name


def format_exts(exts: set[Ext]) -> str:
    spec = ""

    # Copy the extensions because the set will be iteratively reduced.
    exts = set(exts)

    # Select the largest base that is a subset of the extension set.
    base_exts = {k: get_exts_closure(v) for k, v in BASE_EXTS.items()}
    base_sizes = {k: len(v) if v.issubset(exts) else 0 for k, v in base_exts.items()}
    base_ext = max(base_sizes, key=base_sizes.get)
    if base_sizes[base_ext] != 0:
        spec += base_ext
        exts.difference_update(base_exts[base_ext])

    section = [x for x in STANDARD_UNPRIVILEGED_EXTS if x in exts]
    exts.difference_update(section)
    spec += "".join([x.name for x in section]) + "_"

    section = [x for x in STANDARD_MACHINE_EXTS if x in exts]
    exts.difference_update(section)
    spec += "_".join([x.name for x in section]) + "_"

    return spec.strip("_")


def parse_abi(spec: str) -> Abi:
    try:
        return Abi[spec.upper()]
    except KeyError:
        raise ConfigError(f"Unknown ABI {spec}")


def format_abi(abi: Abi) -> str:
    return abi.name.lower()


def parse_abis(spec: str) -> set[Abi]:
    return {parse_abi(x) for x in spec.split(",")}


def format_abis(abis: set[Abi]) -> str:
    return ",".join([format_abi(x) for x in abis])


def parse_mmu(spec: str) -> Mmu:
    try:
        return Mmu[spec.upper()]
    except KeyError:
        raise ConfigError(f"Unknown MMU {spec}")


def format_mmu(mmu: Mmu) -> str:
    return mmu.name.lower()


def parse_mmus(spec: str) -> set[Mmu]:
    return {parse_mmu(x) for x in spec.split(",")}


def format_mmus(mmus: set[Mmu]) -> str:
    return ",".join([format_mmu(x) for x in mmus])


def format_config(config: Configuration) -> str:
    return f"RV{format_xlen(config.xlen)}{format_exts(config.exts)}"


def parse_config(subarch_spec: str, abi_spec: str, mmu_spec: str) -> Configuration:
    prefix_match = re.search(r"^RV(\d+)", subarch_spec)
    if prefix_match is None:
        raise ConfigError(f"Invalid prefix in subarch {subarch_spec}")
    prefix = prefix_match.group(0)
    xlen = parse_xlen(prefix_match.group(1))
    exts = parse_exts(subarch_spec.removeprefix(prefix))

    abis = {x for x in XLEN_ABIS[xlen] if ABI_EXTS[x].issubset(exts)}
    abi = parse_abi(abi_spec)
    if abi not in abis:
        raise ConfigError(f"ABI {format_abi(abi)} is not in set {format_abis(abis)}")

    mmus = XLEN_MMUS[xlen]
    mmu = parse_mmu(mmu_spec)
    if mmu not in mmus:
        raise ConfigError(f"MMU {format_mmu(mmu)} is not in set {format_mmus(mmus)}")

    config = Configuration(
        xlen=xlen,
        exts=exts,
        abi=abi,
        mmu=mmu,
    )
    return config


def do_xlen(config: Configuration, args) -> int:
    print(format_xlen(config.xlen))
    return 0


def do_kernel_config(config: Configuration, args) -> int:
    values = [
        "ARCH_RISCV",
        f'RISCV_SUBARCH=\\"{format_config(config)}\\"',
        f"RISCV_XLEN={format_xlen(config.xlen)}",
        f"RISCV_ABI_{format_abi(config.abi).upper()}",
        f"RISCV_MMU_{format_mmu(config.mmu).upper()}",
    ]
    for ext in config.exts:
        values.append(f"RISCV_ARCH_EXT_{format_ext(ext).upper()}")

    print(" ".join(values))
    return 0


def do_cflags(config: Configuration, args) -> int:
    flags = [
        f"-march={format_config(config).lower()}",
        f"-mabi={format_abi(config.abi)}",
    ]

    print(" ".join(flags))
    return 0


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(prog=argv[0])

    parser.add_argument("--subarch", required=True)
    parser.add_argument("--abi", required=True)
    parser.add_argument("--mmu", required=True)

    subparsers = parser.add_subparsers(title="subcommands", required=True)

    kernel_config_parser = subparsers.add_parser("kernel-config")
    kernel_config_parser.set_defaults(func=do_kernel_config)

    xlen_parser = subparsers.add_parser("xlen")
    xlen_parser.set_defaults(func=do_xlen)

    cflags_parser = subparsers.add_parser("cflags")
    cflags_parser.set_defaults(func=do_cflags)

    args = parser.parse_args(argv[1:])

    try:
        config = parse_config(args.subarch, args.abi, args.mmu)
    except ConfigError as e:
        print(f"{e}", file=sys.stderr)
        return 1

    return args.func(config, args)


if __name__ == "__main__":
    sys.exit(main(sys.argv))
