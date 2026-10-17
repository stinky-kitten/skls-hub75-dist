# Incorporated third-party material

The skls-hub75 driver is an original software implementation developed by Stinky
Kitten Limited. It is not a compiled distribution of Raspberry Pi's HUB75 example
application. The Company's original contributions are covered by
SKLS-HUB75-BINARY-LICENCE-1.0.txt; third-party rights remain independently licensed.

| Material | Incorporated form | License / notice |
|---|---|---|
| Raspberry Pi Pico SDK 2.3.1 hardware/platform header helpers | Compiled inline machine code and register definitions | BSD-3-Clause, attached Pico SDK license |
| Raspberry Pi HUB75 PIO example material | Adapted PIO machine instructions and helper code, compiled into the driver object | BSD-3-Clause, attached Pico SDK license |

Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
Copyright (c) 2024 Raspberry Pi (Trading) Ltd.
Copyright (c) 2024 Raspberry Pi Ltd.
Copyright (c) 2025 Raspberry Pi (Trading) Ltd.
Copyright (c) 2026 Raspberry Pi (Trading) Ltd.

The source PIO carries the 2020 Raspberry Pi notice and SPDX BSD-3-Clause.
The SDK hardware/platform headers carry their respective 2020/2024/2025/2026 notices.
`PICO-SDK-BSD-3-Clause.txt` reproduces the copyright, conditions and disclaimer.
Upstream references: https://github.com/raspberrypi/pico-sdk and
https://github.com/raspberrypi/pico-examples/tree/master/pio/hub75 .
No private implementation source, PIO source or private headers are included.

No Pico SDK runtime/startup archive, newlib/libgcc object, CYW43, TinyUSB or lwIP
implementation object is bundled in this driver-only archive. Consuming firmware
must separately inventory and satisfy the notices for the components it links,
including toolchain runtime and optional wireless/USB implementations. This package
notice is not a complete-firmware license inventory.

## HUB75 PIO provenance and original driver implementation

Material is derived from `raspberrypi/pico-examples/pio/hub75/hub75.pio`:

Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
SPDX-License-Identifier: BSD-3-Clause

The derived portions include the hub75_row and hub75_data_rgb888 PIO programs and
the associated embedded C helpers: hub75_row_program_init and
hub75_data_rgb888_program_init (PIO initialisation), hub75_wait_tx_stall (TX-stall
handling) and hub75_data_rgb888_set_shift (instruction patching). Stinky Kitten's
modifications include additional NOP instructions to adjust timing and blanking
around the row-latch operation.

The surrounding DMA management, buffering, PWM and bitplane scheduling, temporal
dithering, timing calculations and display control were developed by Stinky Kitten
Limited, which owns its original contributions. Raspberry Pi retains copyright
and BSD-3-Clause rights in the derived portions. The approved custom licence does
not override those independently granted rights or their licence conditions.

PICO-SDK-BSD-3-Clause.txt contains the complete applicable Raspberry Pi copyright,
conditions and disclaimer. The existing public release also includes the byte-identical
PICO-EXAMPLES-BSD-3-Clause.txt and PIO-PROVENANCE.md with the pinned comparison and
modifications. The private producer stages both files unchanged from that published tag alongside
the Pico SDK licence, this notice and the approved binary/interface licence.
All five files are required by its payload allowlist and covered by manifest hashes. No third-party copyright or licence conditions are removed.
