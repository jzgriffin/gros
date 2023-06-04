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

#include <string.h>

#include <assert.h>
#include <stdint.h>

size_t strlen(const char* s)
{
    assert(s != NULL);
    size_t n = 0;
    while (*s != '\0') {
        ++s;
        ++n;
    }
    return n;
}

void* memcpy(void* restrict s1, const void* restrict s2, size_t n)
{
    assert(s1 != NULL);
    assert(s2 != NULL);
    // TODO: Optimize.
    uint8_t* p1 = s1;
    const uint8_t* p2 = s2;
    while (n > 0) {
        *p1 = *p2;
        ++p1;
        ++p2;
    }
    return s1;
}

void* memset(void* s, int c, size_t n)
{
    assert(s != NULL);
    uint8_t* p = s;
    while (n > 0) {
        *p = (uint8_t)c;
        ++p;
        --n;
    }
    return s;
}
