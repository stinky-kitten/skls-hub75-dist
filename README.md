# SKLSHUB75 experimental binary distribution

**Release v0.1.0-experimental.2**, ABI **1**, supplies the expanded Embedded Swift
`HUB75` interface. The previous v0.1.0-experimental.1 retains its earlier annotations.
See [Swift API versions and setup](docs/SWIFT.md).

Download the complete firmware package from the
[v0.1.0-experimental.2 release](https://github.com/stinky-kitten/skls-hub75-dist/releases/tag/v0.1.0-experimental.2):

```sh
curl --fail --location --remote-name https://github.com/stinky-kitten/skls-hub75-dist/releases/download/v0.1.0-experimental.2/skls-hub75-0.1.0-experimental.2-rp2350-arm-s.zip
curl --fail --location --remote-name https://github.com/stinky-kitten/skls-hub75-dist/releases/download/v0.1.0-experimental.2/SHA256SUMS
shasum -a 256 -c SHA256SUMS
unzip skls-hub75-0.1.0-experimental.2-rp2350-arm-s.zip
```

ZIP SHA-256: `58789080212e2d2ad1c3ced10623fb61e9a6992c17c76d9b50c95a82613f032c`.
Use the extracted `skls-hub75-0.1.0-experimental.2-rp2350-arm-s` directory as
`SKLSHUB75_ROOT` and set `SKLSHUB75_REQUIRED_VERSION` to `0.1.0-experimental.2`.

The repository provides declaration-only SwiftPM integration. Pin the exact version:

```swift
.package(url: "https://github.com/stinky-kitten/skls-hub75-dist.git",
         exact: "0.1.0-experimental.2")
```

Add `.product(name: "SKLSHUB75", package: "skls-hub75-dist")` to the application
target's dependencies. Link the Arm implementation separately through CMake as below;
the SwiftPM product does not link that implementation.

The approved [binary/interface licence](LICENSES/SKLS-HUB75-BINARY-LICENCE-1.0.txt)
covers Stinky Kitten Limited's original contributions. Third-party rights remain
separate. **The API remains experimental; formal hardware scanout/timing
qualification is outstanding.**

One driver archive is intended for Raspberry Pi **Pico 2** and **Pico 2 W**, with
`PICO_PLATFORM=rp2350-arm-s`, Pico SDK **2.3.1**, and **150 MHz clk_sys**.
The application supplies its SDK runtime and launches Core 1. No wireless
initialization or CYW43 library is required by this driver.

After `pico_sdk_init()`:

```cmake
# Set SKLSHUB75_REQUIRED_VERSION to the full package version you pinned.
find_package(SKLSHUB75 CONFIG REQUIRED
    PATHS "${SKLSHUB75_ROOT}/cmake" NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
if(NOT SKLSHUB75_PACKAGE_VERSION STREQUAL SKLSHUB75_REQUIRED_VERSION)
    message(FATAL_ERROR "Unexpected SKLSHUB75 package version")
endif()
target_link_libraries(my_firmware PRIVATE SKLSHUB75::hub75)
```

`SKLSHUB75_ROOT` is the unpacked binary package, including for local driver development.
No private source checkout is involved. `SKLSHUB75_PACKAGE_VERSION` and
`manifest.json` identify the full selected version. CMake's numeric version is its
MAJOR.MINOR.PATCH core; optional numeric `find_package(... EXACT ...)` checks cannot
distinguish prereleases or build metadata, so retain the full-version check above.
Generated candidates also expose `SKLSHUB75_CMAKE_VERSION` and
`SKLSHUB75_PUBLICATION_STATUS`; the manifest records `cmake_version` and
`publication_status`. The payload retains its private preparation status as provenance; the public
`release.json`, release tag and GitHub release identify publication. The packaged
README and release documentation likewise retain their qualified preparation text.
The producer has no default version and rejects identifiers already recorded as
published. Pin the complete version and checksum; local output must not overwrite
a published release. See [release conditions](docs/RELEASE.md).

C and C++ include `<skls_hub75/skls_hub75.h>`. Swift uses `import SKLSHUB75`.
See [ABI requirements](docs/ABI.md), [Swift setup](docs/SWIFT.md), and
[release conditions](docs/RELEASE.md). `manifest.json` contains payload hashes,
archive membership, producer identity, capabilities and compatibility settings.

The archive implements six PWM bitplanes with four-phase temporal dithering,
six selectable scan schedules, configurable transfer/gamma and refresh-target
requests within the current qualification envelope. Requested refresh is not a
guarantee. PIO transition timing and channel gain/brightness calibration are
compiled in. Arbitrary bit depths, extra blanking and other targets are not supported.

Compiled machine code, symbols and constants remain inspectable. Removing debug
metadata reduces accidental disclosure; it does not prevent reverse engineering.
No on-device scanout or refresh-rate claim follows from build/link validation.
