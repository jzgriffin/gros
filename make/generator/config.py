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

import argparse
import os
import re
import sys


ITEM_REGEX = re.compile(r"(\w+)(=(.*))?")


def parse_items(file):
    items = {}
    for line in file:
        match = ITEM_REGEX.search(line)
        if not match:
            print(f'Invalid configuration line "{line}"', file=sys.stderr)
            continue
        key = match.group(1)
        value = match.group(3) or "1"
        if key not in items:
            items[key] = value
        elif items[key] != value:
            print(
                f"Conflicting values for {key}: {value}, {items[key]}", file=sys.stderr
            )
    return items


def generate_defines(items, file):
    for k, v in sorted(items.items()):
        file.write(f"#define {k} {v}\n")


def generate(src_file, items, obj_file):
    for line in src_file:
        if line.strip() == "#pragma config":
            generate_defines(items, obj_file)
        else:
            obj_file.write(line)


def do_generate(args):
    with open(args.obj_lst_file, "r") as obj_lst_file:
        items = parse_items(obj_lst_file)
    with open(args.src_file, "r") as src_file:
        os.makedirs(os.path.dirname(args.obj_file), exist_ok=True)
        with open(args.obj_file, "w") as obj_file:
            generate(src_file, items, obj_file)
    return 0


def do_deploy(args):
    # Only copy the file if the contents have changed.
    with open(args.obj_file, "rb") as f:
        obj_config = f.read()
    copy = True
    if os.path.isfile(args.out_file):
        with open(args.out_file, "rb") as f:
            out_config = f.read()
            copy = obj_config != out_config
    if copy:
        os.makedirs(os.path.dirname(args.out_file), exist_ok=True)
        with open(args.out_file, "wb") as f:
            f.write(obj_config)
    return 0


def main(argv):
    parser = argparse.ArgumentParser(prog=argv[0])

    subparsers = parser.add_subparsers(title="subcommands", required=True)

    generate_parser = subparsers.add_parser("generate")
    generate_parser.add_argument("src_file")
    generate_parser.add_argument("obj_lst_file")
    generate_parser.add_argument("obj_file")
    generate_parser.set_defaults(func=do_generate)

    deploy_parser = subparsers.add_parser("deploy")
    deploy_parser.add_argument("obj_file")
    deploy_parser.add_argument("out_file")
    deploy_parser.set_defaults(func=do_deploy)

    args = parser.parse_args(argv[1:])

    return args.func(args)


if __name__ == "__main__":
    sys.exit(main(sys.argv))
