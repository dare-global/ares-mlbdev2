# MlbDev2

MLB Old C++ Development Redux

## Building

### Quick Start

```bash
# Fetch algo-utils and set PATH
source ./scripts/source-algo-utils.sh

# Build with GCC (recommended)
dev.sh build gcc

# Build with specific options
dev.sh build gcc -b release
dev.sh build gcc -b debug
dev.sh build gcc -c  # Clean build

# Build with sanitizers
dev.sh build asan    # AddressSanitizer
dev.sh build tsan    # ThreadSanitizer
```

The build automatically:
1. Downloads Boost and NATS from `ares-external`
2. Builds all libraries
3. Creates installable package

### CMake Build (Direct)

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j$(nproc)
```

### Build Options

| Option | Description | Default |
|--------|-------------|---------|
| `CMAKE_BUILD_TYPE` | `Debug` or `Release` | `Release` |
| `BOOST_VERSION` | Boost version to use | `1.83.0` |
| `MLBDEV2_OS_NAME` | OS name for package downloads | Auto-detected |
| `MLBDEV2_OS_VERSION` | OS version for package downloads | Auto-detected |

### Building with Different Boost Versions

```bash
# Build with Boost 1.90.0
BOOST_VERSION=1.90.0 dev.sh build gcc

# Build with default Boost 1.83.0
dev.sh build gcc
```

### Output

Libraries are built to `build/lib/`:
- `libUtility.a`
- `libLogger.a`
- `libMFStore.a`
- `libNatsWrapper.a`

### Installation

```bash
cmake --install . --prefix /usr/local
```

---

## Releasing

### Full Release (All Boost Versions)

```bash
# Preview release (dry-run by default)
dev.sh release --extra "BOOST_VERSION=1.83.0,1.90.0:boost"

# Execute release with upload
dev.sh release --extra "BOOST_VERSION=1.83.0,1.90.0:boost" --upload --run
```

This automatically:
1. Builds with each Boost version (1.83.0, 1.90.0)
2. Creates packages in `archive/`
3. Uploads all to the same GitHub release

### Single Version Release

```bash
# Build, package, and upload a single version
BOOST_VERSION=1.83.0 dev.sh release --extra boost1.83.0 --upload --run
```

### Manual Packaging

```bash
# Package only
dev.sh package --extra boost1.83.0

# Or use CMake directly
cmake --build build/gcc-release --target package

# Upload separately
dev.sh upload ares-mlbdev2 2026.01.21 -x boost1.83.0
```

Package naming: `ares-mlbdev2-<version>-<extra>-<compiler><ver>-<build_type>-<os>-<os_ver>.tar.gz`

Example: `ares-mlbdev2-2026.01.21-boost1.83.0-gcc14-release-ubuntu-24.04.tar.gz`

---

## Dependencies

### External Dependencies (from ares-external)

These are automatically downloaded during build:

| Package | Version | Used By |
|---------|---------|---------|
| **Boost** | 1.83.0 (default), 1.90.0 | Utility, Logger, MFStore |
| **NATS (nats.c)** | 3.11.0 | NatsWrapper |

### System Dependencies

Install these before building:

| Package | Required For | Install (Ubuntu/Debian) |
|---------|--------------|-------------------------|
| **GCC 14** | Compilation | `apt install g++-14` |
| **CMake 4.2.1+** | Build system | See [cmake.org](https://cmake.org) |
| **OpenSSL** | NatsWrapper (TLS) | `apt install libssl-dev` |
| **pthread** | Threading | (usually pre-installed) |

---

## Libraries

| Library | Description |
|---------|-------------|
| **Utility** | Core utilities: string handling, time support, command-line parsing, file I/O |
| **Logger** | Logging framework with console and file handlers |
| **MFStore** | Memory-mapped file store |
| **NatsWrapper** | C++ wrapper for NATS messaging |

## Requirements Summary

- C++23 compiler (GCC 14+ recommended)
- CMake 4.2.1+
- OpenSSL development headers (`libssl-dev`)
- Access to `ares-external` GitHub repository (for Boost/NATS downloads)

## Scripts

| Script | Description |
|--------|-------------|
| `scripts/source-algo-utils.sh` | Fetches algo-utils (scripts + cmake) and sets PATH (must be sourced) |

### Using dev.sh

After sourcing, `dev.sh` provides a unified CLI:

```bash
dev.sh build gcc              # Build with GCC
dev.sh build gcc -c           # Clean build
dev.sh build asan             # Build with AddressSanitizer
dev.sh test gcc --coverage    # Run tests with coverage
dev.sh package                # Create package
dev.sh release --help         # Show release options
```

## License

Distributed under the Boost Software License, Version 1.0.
See [LICENSE](LICENSE) for details.
