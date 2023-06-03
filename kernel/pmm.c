// Copyright (c) 2023 Jeremiah Z. Griffin
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <kernel/pmm.h>

#include <kernel/arch/mmu.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// This implementation meets the bare-minimum definition of a memory manager.
// It allocates physical pages without regard considering the amount of DRAM
// available in the system.  It does not free unused pages or even track which
// pages have been freed.  It is a starting point for a more advanced manager.

extern void* const __end;
static PhysicalAddress _start = 0;
static PhysicalAddress _next = 0;

size_t allocate_physical_pages(PhysicalAddress* addrs, size_t count)
{
    dprintf("Allocating %u physical pages into %p\n", count, addrs);
    size_t i = 0;
    for (; i < count; ++i) {
        addrs[i] = _next;
        _next += PAGE_SIZE;
    }
    return i;
}

PhysicalAddress allocate_physical_page(void)
{
    PhysicalAddress addr = 0;  // Unmodified if allocate_physical_pages fails.
    allocate_physical_pages(&addr, 1);
    return addr;
}

void free_physical_pages(PhysicalAddress* addrs, size_t count)
{
    dprintf("Freeing %u physical pages at %p\n", count, addrs);
}

void free_physical_page(PhysicalAddress addr)
{
    free_physical_pages(&addr, 1);
}

void initialize_pmm(void)
{
    PhysicalAddress start = ROUND_PAGE_UP((PhysicalAddress)&__end);
    dprintf("Initializing PMM with starting address %p\n", start);

    _start = start;
    _next = _start;
}
