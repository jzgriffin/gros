/* Copyright (c) 2023 Jeremiah Z. Griffin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef KERNEL_DEVICE_H
#define KERNEL_DEVICE_H

#include <kernel/compiler.h>

#define DEVICE_INITIALIZER_NAME(tag, func) \
    device_initializer_ ## tag ## _ ## func
#define DEVICE_INITIALIZER(tag, func) \
    DeviceInitializer \
        DEVICE_INITIALIZER_NAME(tag, func) USED \
        LINKER_SECTION(.device.initializer) = func

#define DEVICE_FINALIZER_NAME(tag, func) \
    device_finalizer_ ## tag ## _ ## func
#define DEVICE_FINALIZER(tag, func) \
    DeviceFinalizer \
        DEVICE_FINALIZER_NAME(tag, func) USED \
        LINKER_SECTION(.device.finalizer) = func

typedef void (*DeviceInitializer)(void);
typedef void (*DeviceFinalizer)(void);

extern const DeviceInitializer __device_initializer_start;
extern const DeviceInitializer __device_initializer_end;

extern const DeviceFinalizer __device_finalizer_start;
extern const DeviceFinalizer __device_finalizer_end;

#endif  /* KERNEL_DEVICE_H */
