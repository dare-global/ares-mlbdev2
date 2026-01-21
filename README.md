# MlbDev2

MLB Old C++ Development Redux

## Building

### Quick Start

```bash
# Fetch algo-utils and set PATH
source ./scripts/source-ares-scripts.sh

# Build with GCC (recommended)
build.sh gcc

# Build with specific options
build.sh gcc --build-type release
build.sh gcc --build-type debug
build.sh gcc --clean  # Clean build
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
| `MLBDEV2_OS_NAME` | OS name for package downloads | Auto-detected |
| `MLBDEV2_OS_VERSION` | OS version for package downloads | Auto-detected |

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

### Creating Distribution Package

```bash
# After building, create tarball in archive/
cmake --build build/gcc-release --target package
```

Package naming: `ares-mlbdev2-<version>-boost<boost_version>-<compiler><ver>-release-<os>-<os_ver>.tar.gz`

Example: `ares-mlbdev2-2026.01.21-boost1.83.0-gcc14-release-ubuntu-24.04.tar.gz`

---

## Dependencies

### External Dependencies (from ares-external)

These are automatically downloaded during build:

| Package | Version | Used By |
|---------|---------|---------|
| **Boost** | 1.83.0 | Utility, Logger, MFStore |
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
| `scripts/source-ares-scripts.sh` | Fetches ares scripts and sets PATH (must be sourced) |

### Using algo-utils Scripts

After sourcing, all algo-utils scripts are available:

```bash
build.sh gcc                    # Build with GCC
build.sh gcc --clean            # Clean build
upload_package.sh --list        # List packages in archive/
upload_package.sh --help        # See upload options
```

### Uploading to GitHub Releases

```bash
# Create package
cmake --build build/gcc-release --target package

# Upload (specify repo explicitly for now)
upload_package.sh ares-mlbdev2-2026.01.21 boost1.83.0 -c gcc -v 14 --repo=dare-global/ares-mlbdev2
```

## License

Distributed under the Boost Software License, Version 1.0.
See [LICENSE](LICENSE) for details.
