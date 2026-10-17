# SKLSHUB75 0.1.0-experimental.2 — experimental ABI 1

This release publishes the expanded Embedded Swift interface and the complete,
qualified 23-file binary distribution for Raspberry Pi Pico 2 and Pico 2 W.

Swift now imports `HUB75`-prefixed types and enums, hardware descriptor initializers,
configuration presets/validation/resolution, driver initialization/reconfiguration/
stopping, `readHUB75Status(into:)`, `runHUB75Core1()`,
`HUB75RenderRequest.isCurrent()` and hardware geometry methods. These annotations
preserve the existing C interface and ABI 1. Earlier experimental.1 assets and
annotations remain unchanged.

The payload includes all five licence, attribution and provenance files. It was
qualified with 44 comprehensive checks, including positive and negative Embedded
Swift imports and source-free C/C++ CMake linkage for both boards. Release preparation
preserved all 23 producer files without rebuilding, overlays or manifest regeneration.
The declaration-only SwiftPM product supplies matching interfaces; the firmware ZIP
supplies the Arm static library for CMake linkage.

**Experimental status:** formal hardware scanout/timing qualification remains
outstanding. Build/link validation does not establish on-device timing or refresh
performance. Compatibility is limited to the documented secure RP2350 Arm ABI,
Pico SDK 2.3.1, 150 MHz clk_sys and qualified toolchain assumptions. Reserve Core 1,
PIO0 state machines 0 and 1, 21 PIO instruction slots, a dynamically claimed DMA
channel and the documented GPIOs. See [ABI requirements](docs/ABI.md).

Approved release assets:

- [skls-hub75-0.1.0-experimental.2-rp2350-arm-s.zip](https://github.com/stinky-kitten/skls-hub75-dist/releases/download/v0.1.0-experimental.2/skls-hub75-0.1.0-experimental.2-rp2350-arm-s.zip) (53,456 bytes)
  SHA-256: `58789080212e2d2ad1c3ced10623fb61e9a6992c17c76d9b50c95a82613f032c`
- [SHA256SUMS](https://github.com/stinky-kitten/skls-hub75-dist/releases/download/v0.1.0-experimental.2/SHA256SUMS) (115 bytes)
  SHA-256: `c2d0bcd1bb50d8d9ba83cfaace6eb67d7d1cc85909377216508f28e8a51cba25`

The archive manifest SHA-256 is
`ac30f90692b56c4615996c1c649f6a85d3b70872ab7187437c2dd19119b5c7a2`.
The approved Stinky Kitten HUB75 Binary Library Licence — Version 1.0 applies to
Stinky Kitten Limited's original work. Raspberry Pi-derived material retains its
independent BSD-3-Clause rights and notices. See [licensing](LICENSES/NOTICE.md).
