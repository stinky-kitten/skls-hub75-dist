# Incorporated third-party material

This is the incorporated-material notice inventory, not permission to
redistribute the proprietary driver or its public interface files.

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

The PIO comparison and modifications are documented in PIO-PROVENANCE.md.
PICO-EXAMPLES-BSD-3-Clause.txt includes the complete applicable upstream licence.
Raspberry Pi's BSD-licensed contribution remains distinct from original proprietary
code; proposed proprietary restrictions do not withdraw its existing BSD rights.
