#!/bin/bash
# ares-mlbdev2 Release Script - Creates library package
#
# Usage: ./scripts/release.sh [options...]
#
# Examples:
#   ./scripts/release.sh                    # build, package, upload
#   ./scripts/release.sh --skip-upload      # build, package only
#   ./scripts/release.sh --dryrun           # preview what would be done
#
# Package Contents:
#   - include/                    (headers)
#     - Utility/                  (utility library headers)
#     - Logger/                   (logger library headers)
#     - MFStore/                  (memory-mapped file store headers)
#     - NatsWrapper/              (NATS client wrapper headers)
#   - lib/                        (static libraries)
#     - libUtility.a
#     - libLogger.a
#     - libMFStore.a
#     - libNatsWrapper.a
#   - lib/cmake/ares-mlbdev2/     (CMake config for find_package)

set -e

cd "$(dirname "${BASH_SOURCE[0]}")/.."
source ./scripts/source-algo-utils.sh 2>/dev/null

dev.sh release "$@"
