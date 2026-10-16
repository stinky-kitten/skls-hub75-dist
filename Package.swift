// swift-tools-version: 6.0
import PackageDescription

// Authentic public module only. The C anchor defines no driver functions.
let package = Package(
    name: "skls-hub75-dist",
    products: [.library(name: "SKLSHUB75", targets: ["SKLSHUB75"])],
    targets: [.target(name: "SKLSHUB75", path: ".",
        exclude: ["docs", "LICENSES", "tools", "README.md", "release.json", "release-notes.md"],
        sources: ["swiftpm/Interface.c"], publicHeadersPath: "include")]
)
