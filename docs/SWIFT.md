# Embedded Swift import

The public include directory contains `module.modulemap`, a module named
`SKLSHUB75`, and matching `SKLSHUB75.apinotes` beside the map. The consumer
imports the C header using its own compiler; no compiled Swift module is distributed.

Use `-I <package>/include`, the Embedded feature and whole-module compilation.
The qualified Swift snapshot is `main-snapshot-2026-09-21` / Swift 6.5-dev
(LLVM a0a7653f4c0c1a0, Swift ac2207a7efaa55c). The established consumer settings are:

```text
-target armv7em-none-none-eabi
-enable-experimental-feature Embedded -wmo -parse-as-library
-Xcc -mfloat-abi=soft -Xcc -fshort-enums
```

The C archive targets Cortex-M33/Armv8-M and uses softfp. This Swift target is a
compatible conservative instruction subset; soft and softfp both use base AAPCS
argument passing. The package never silently changes an application's Swift target
or float ABI. Ensure fixed profile definitions reach the Clang importer; the packaged
build-config header also fixes them to zero and rejects contradictory values.

```swift
import SKLSHUB75
var request = RenderRequest()
if takeRenderRequest(&request) {
    // Honor its generation/deadline and unwrap its borrowed framebuffer.
}
let transfer: TransferMode = .gamma
```

API notes rename the request type and take function, and import TransferMode with
`.linear`/`.gamma`. The enum is **open** (`NSEnum`), so use `@unknown default` in
switches to allow future C values. These annotations do not renumber or widen C enums.
The original C spellings remain in the C headers and C archive.

`takeRenderRequest` accepts an optional pointer because the C implementation accepts
null and returns false. The preset's two output pointers are nonnull, matching its
contract. Notes do not mark the framebuffer nonnull: a zero-initialized request can
contain nil. No ownership, bounds or lifetime enforcement is promised.

The current anonymous C typedef supports type renaming, but tested field annotations
had no effect without named C tags. No header/tag redesign is included for cosmetics.
There is no Swift wrapper framework. A bridging header is unnecessary for this module;
applications may keep separate bridges for their own SDK/application APIs.

The private validation suite compiles annotated names, rejects those names without
notes, compiles original spellings with notes disabled, tests the open enum and
nonnull outputs, and uses separate caches. The qualified importer discovers notes
by default. Retest when changing snapshots; merely placing YAML beside a textual
header is insufficient without module association.
