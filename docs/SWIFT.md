# Embedded Swift import

## Released API versions

**v0.1.0-experimental.2** supplies the expanded `HUB75` API documented below.
Its declaration-only SwiftPM package and firmware ZIP contain matching headers and
API notes. The API remains experimental; annotations change Swift import behavior
without changing C ABI 1.

The earlier **v0.1.0-experimental.1** predates these changes. It exposes
`RenderRequest`, `TransferMode.linear`/`.gamma` and `takeRenderRequest(_:)`;
other operations retain their original C spellings. Its immutable assets and tag
are unchanged. Use documentation and metadata for the exact version consumed.
The experimental.2 ZIP preserves its qualified producer documentation, including
prepublication status text; this tagged guide describes the published interface.

The include directory contains `module.modulemap`, module `SKLSHUB75`, and matching
`SKLSHUB75.apinotes`. Consumers import the C headers with their own compiler;
no compiled Swift module or wrapper framework is distributed. API notes change
Swift spelling/import behavior, not C symbols, enum values, layouts or ABI 1.
The source target alone does not stage this module metadata.

## Compiler setup

Use `-I <package>/include`, the Embedded feature and whole-module compilation.
The qualified Swift snapshot is `main-snapshot-2026-09-21` / Swift 6.5-dev
(LLVM a0a7653f4c0c1a0, Swift ac2207a7efaa55c). The established settings are:

```text
-target armv7em-none-none-eabi
-enable-experimental-feature Embedded -swift-version 6 -wmo -parse-as-library
-Xcc -mfloat-abi=soft -Xcc -fshort-enums
```

The C archive targets Cortex-M33/Armv8-M and uses softfp. This Swift target is a
compatible conservative instruction subset; soft and softfp use base AAPCS argument
passing. CMake does not choose the application's Swift target or float ABI. The
packaged build-config header fixes driver profiling to zero and rejects contradictory
definitions. See [ABI requirements](ABI.md) for runtime and hardware constraints.
A bridging header is unnecessary for this module; applications may keep separate
bridges for their own SDK/application APIs. Retest when changing compiler snapshots.

## Release interface

| Imported types | Purpose |
|---|---|
| `HUB75HardwareDescriptor` | Geometry and mutable Pico wiring |
| `HUB75Configuration`, `HUB75ResolutionPolicy`, `HUB75ConfigurationResolution` | Requested settings, AUTO/FIXED authority and resolved output |
| `HUB75RenderRequest`, `HUB75Status` | Borrowed render work and runtime telemetry |
| `HUB75TransferMode` | `.linear`, `.gamma` |
| `HUB75DisplayPolicy` | `.current6Bit4Phase` |
| `HUB75TargetPolicy` | `.strict`, `.bestEffort`, `.fixedTiming` |
| `HUB75ResolutionMode` | `.auto`, `.fixed` |
| `HUB75ScanSchedule` | `.bitplaneMajor`, `.rowMajor`, `.low3RowGrouped`, `.low4RowGrouped`, `.rowMajorInterleaved`, `.rowMajorFolded`; `.count` is a sentinel |
| `HUB75ConfigurationResult` | `.ok` or a specific validation/resolution error |
| `HUB75LifecycleResult`, `HUB75LifecycleState`, `HUB75TargetResult` | Control results, lifecycle state and measured target outcome |

All nine annotated enums are open (`NSEnum`): use `@unknown default` in switches.
For example, lifecycle results include `.invalidConfig`, `.storageUnavailable`,
`.stopTimeout`, `.startTimeout`, `.calibrationFailed`, `.hardwareFailed` and
`.alreadyInitialized`. C structure fields retain their C names, such as
`transfer_mode`, `scan_schedule`, `lifecycle_state` and `deadline_us`.

`HUB75HardwareDescriptor(width:height:)` calls the default Pico wiring preset;
it does not validate the requested geometry. The zero-argument initializer only
zero-initializes the C structure. `addressedRows()` returns `UInt16` (height / 2)
and `addressBits()` returns `UInt8`, the ceiling of log2(addressed rows) for valid
geometry.

The following is compilable Embedded Swift against this version's metadata. These
functions are integration helpers: the application calls `prepareDisplay()` on
Core 0 before launching Core 1, launches `displayCore1Entry()` once using its own
platform integration, then services render work on Core 0. The supplied renderer
must complete all writes before the absolute Pico-monotonic deadline; the helper
cannot enforce its execution time. The 192 × 32 geometry uses the documented
hardware baseline; requested 240 Hz remains subject to calibration.

```swift
import SKLSHUB75

func prepareDisplay() -> Bool {
    var hardware = HUB75HardwareDescriptor(width: 192, height: 32)
    var configuration = HUB75Configuration()
    var policy = HUB75ResolutionPolicy()
    configuration.apply6Bit4Phase240HzPreset(resolutionPolicy: &policy)
    configuration.transfer_mode = .gamma
    configuration.scan_schedule = .bitplaneMajor

    guard configuration.validate(
        hardware: &hardware, resolutionPolicy: &policy
    ) == .ok else { return false }

    var resolution = HUB75ConfigurationResolution()
    guard configuration.resolve(
        hardware: &hardware, resolutionPolicy: &policy,
        systemClockHz: 150_000_000, into: &resolution
    ) == .ok else { return false }

    guard resolution.addressed_rows == hardware.addressedRows(),
          resolution.address_bits == hardware.addressBits()
    else { return false }

    return initializeHUB75(
        hardware: &hardware, configuration: &configuration,
        resolutionPolicy: &policy
    ) == .ok
}

func displayCore1Entry() {
    runHUB75Core1()
}

func serviceDisplay(
    render: (UnsafeMutablePointer<UInt32>, UInt16, UInt16, UInt64, UInt64) -> Void
) {
    var request = HUB75RenderRequest()
    guard takeRenderRequest(&request), request.isCurrent(),
          let pixels = request.framebuffer else { return }
    render(pixels, request.width, request.height,
           request.presentation_time_us, request.deadline_us)
}

func displayIsRunning() -> Bool {
    var status = HUB75Status()
    return readHUB75Status(into: &status) && status.lifecycle_state == .running
}

func restartDisplay() -> HUB75LifecycleResult {
    var hardware = HUB75HardwareDescriptor(width: 192, height: 32)
    var configuration = HUB75Configuration()
    var policy = HUB75ResolutionPolicy()
    configuration.apply6Bit4Phase240HzPreset(resolutionPolicy: &policy)
    return reconfigureHUB75(
        hardware: &hardware, configuration: &configuration,
        resolutionPolicy: &policy
    )
}

func stopDisplay() -> HUB75LifecycleResult {
    stopHUB75()
}
```

Validation leaves caller-owned inputs unchanged. Resolution writes its output only
on `.ok` and selects an initial candidate; startup calibration may refine AUTO
pulse timing. Initialization prepares the first generation without waiting for
Core 1 calibration. Once any generation has been committed, use reconfiguration
to restart; calling initialization again returns `.alreadyInitialized`.
The Core 1 worker remains resident across stop/reconfiguration. Serialize Core 0
control operations with rendering and inspect lifecycle results, including timeouts.

`readHUB75Status(into:)` returns false before any committed generation, filling
zero/uninitialized status. A true result identifies a generation and lifecycle
state, but does not alone guarantee a stable telemetry sample; rate fields may
still be zero/default during startup or transitions.

Render requests use latest-value semantics. `isCurrent()` checks generation
eligibility; it does not extend the deadline or confer ownership. Pixels are
borrowed, tightly packed row-major numeric `0xAARRGGBB`; alpha is ignored. Never
free or retain the pointer through stop/reconfiguration, or write after the deadline.
`takeRenderRequest(nil)` is permitted and returns false. The preset's two outputs
are nonnull. The framebuffer remains optional because a zero-initialized request
can contain nil. No ownership, bounds or lifetime enforcement is added by notes.

## Validation

In the private producer, run the VS Code task **HUB75: Test Swift API Notes** or
`python3 tools/test_swift_api_notes.py` for fast importer validation. It compiles
working headers and notes with fresh caches and checks expected negative diagnostics.
The full binary validator additionally checks the staged payload, ABI and source-free
C/C++ firmware linkage for both boards. Importer compilation alone establishes no
runtime or hardware qualification. Neither check publishes a release.
