# ABI 1 contract

ABI identifier: `rp2350-arm-s-aapcs-softfp-short-enums-v1-profile00`.

Supported initial configuration: Raspberry Pi Pico 2 and Pico 2 W, RP2350 Arm
secure (`rp2350-arm-s`), with Pico SDK 2.3.1 and a 150 MHz system clock.
Both boards use the same archive. Board wireless/default-LED definitions are not
used by the driver. Applications retain their own startup, board and optional
wireless integration. Other RP2350 boards are unqualified, not automatically supported.

The archive uses 32-bit little-endian Arm ELF/EABI5, Cortex-M33,
`-mcpu=cortex-m33 -mthumb -march=armv8-m.main+fp+dsp -mcmse`,
`-mfloat-abi=softfp` (base AAPCS argument passing), `-fshort-enums`, and
`-ftls-model=local-exec`. Producer qualification is Arm GNU 15.2.Rel1,
GCC 15.2.1. It contains ordinary non-LTO objects, built with `-O3 -DNDEBUG`,
function/data sections and `-fno-math-errno`. It does not require compiler LTO IR
or its plugin. Compatibility with other compilers/SDK releases is not qualified.
Hard-float VFP argument passing, nonsecure images, RP2040 and RISC-V are unsupported.
There are no nonsecure-callable entry veneers; this is an ordinary secure-image library.

## Layout

Target measurements (not macOS host sizes):

| Public type | Bytes | Alignment |
|---|---:|---:|
| hardware descriptor | 10 | 2 |
| configuration | 40 | 4 |
| resolution policy | 3 | 1 |
| configuration resolution | 52 | 4 |
| status | 56 | 4 |
| render request | 32 | 8 |

All nine public enum types currently occupy one byte. Numeric values, enum
storage and all public field offsets are ABI. Config gamma is at offset 4,
scan schedule at 36; render framebuffer is at 4, presentation timestamp at 16,
and deadline at 24. Pointers and size_t are 4 bytes; float is 4 bytes.
Producer and C/C++ consumer assertions cover every field in these six structures,
all enum sizes, sizes and alignments, plus key numeric enum values.

Passing input structures by pointer still exposes their layouts. The hardware
preset also returns its descriptor by value. Do not extend/reorder structures,
renumber enums or widen enum storage without a new ABI and matching headers.
Do not use packing options that change these layouts.

The imported target supplies short-enum options for C/C++ and fixed profiling
macros. Swift applications must supply their own matching importer/target flags.
Each staged header includes `skls_hub75_binary_config.h`: contradictory profile
macros are compile errors, even without CMake. Driver refresh and bitplane
profiling are both fixed **0**. This is independent of application Swift renderer
profiling. No additional driver profiling variant is supplied.

## Runtime integration and ownership

The SDK dependencies are exactly `pico_stdlib`, `hardware_clocks`, `hardware_dma`,
`hardware_gpio`, `hardware_pio`, and `hardware_sync`. The application still provides
SDK startup/boot stage, linker script, runtime/heap and C/Arm arithmetic support.
Unresolved imports include SDK hardware/time services, malloc/free, memcpy/memset,
powf and `__aeabi_uldivmod`; metadata lists the actual archive imports.
The archive contains no SDK startup/runtime objects. No private PIO source/header
or separate driver resource is needed. PIO instructions are already compiled in.

Reserve Core 1 for the persistent worker and **PIO0 SM0/SM1** for this driver.
The application launches `skls_hub75_core1_run` (and links `pico_multicore` if it
uses the SDK launch helper). The driver dynamically claims one available DMA channel
and loads two programs into PIO0 instruction memory: 16 data instructions and 5 row
instructions, **21 of the 32 instruction slots** in total. Both programs must fit
alongside any other PIO0 programs. Successful program-space and DMA claims are
retained across stop/reconfiguration; do not treat stop as returning those resources
to another subsystem. It polls scanout progress and requires no separately linked driver ISR.
Do not alter the system clock while operating: the resolver currently accepts only
150 MHz clk_sys and PIO divider256 values 256/257. Pin groups must remain consecutive
and disjoint, with OE immediately after LAT, using the public config validation.
Default wiring: RGB GPIO16–21, row address GPIO6–10, clock11, LAT12, OE13.

Configuration inputs are read/copied during synchronous initialization or
reconfiguration. The caller owns those inputs; the driver owns framebuffer/DMA
allocations and private LUT state. Render requests copy a borrowed framebuffer
pointer, not ownership. Pixels are tightly packed row-major numeric `0xAARRGGBB`;
alpha is ignored. Requests use latest-value semantics. Honor generation and absolute
Pico-monotonic deadlines: no writes after the deadline, no freeing the pointer,
and no retaining it through stop/reconfiguration. A generation check does not extend
a deadline. Serialize Core-0 rendering/control operations as described by the headers.

## Capabilities and qualification

Six displayed bitplanes/four dither phases are the only admitted policy, despite
configuration fields existing for other values. Scan-order, transfer/gamma,
presentation divisor and target requests are runtime configuration within a
running generation/reconfiguration lifecycle; not every requested rate is achievable.
Nonzero additional blanking is rejected. PIO transition delays are compiled in.
Channel gains and brightness are compiled at 255; brightness gamma is 1.0.

Automated qualification builds C/C++ consumers for both boards against the same
relocated archive, checks ABI assertions, archive contents and header independence,
and compiles the Embedded Swift module with positive/negative notes tests.
`tools/validate_binary.py` in the private producer writes the actual compiler
version, commands and results outside this distributable payload. These checks
establish compilation/linkage, not on-device timing, brightness or hardware behavior.
