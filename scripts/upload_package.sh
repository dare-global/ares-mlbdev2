#!/bin/bash
# Upload package to GitHub releases for ares-mlbdev2
# Wrapper that adapts ares-external's upload_package.sh
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Setup environment
export SRC_PATH="$PROJECT_ROOT"
export ARCHIVE_DIR="${PROJECT_ROOT}/archive"
export GITHUB_REPO="dare-global/ares-mlbdev2"

# Default compiler settings
COMPILER="${COMPILER:-gcc}"
declare -A COMPILER_VERSIONS=([gcc]="14" [clang]="22")

set_compiler_version() {
    local build_type="${1:-release}"
    [[ -z "$COMPILER_VERSION_MAJOR" ]] && COMPILER_VERSION_MAJOR="${COMPILER_VERSIONS[$COMPILER]}"
    export COMPILER_VERSION_MAJOR COMPILER BUILD_TYPE="$build_type"
    export COMPILER_BUILD_TYPE="${COMPILER}-${build_type}"
    export BUILD_PATH="${SRC_PATH}/build/${COMPILER_BUILD_TYPE}"
    export INSTALL_PATH="${SRC_PATH}/install/${COMPILER_BUILD_TYPE}"
    export ARCHIVE_PATH="${SRC_PATH}/archive"
}

get_os_info() {
    if [[ -f "/etc/lsb-release" ]]; then
        while IFS='=' read -r k v; do
            case "$k" in
                DISTRIB_ID) OS_NAME=$(echo "$v" | tr '[:upper:]' '[:lower:]' | tr -d '"') ;;
                DISTRIB_RELEASE) OS_VERSION=$(echo "$v" | tr -d '"') ;;
            esac
        done < /etc/lsb-release
    else
        OS_NAME="linux" OS_VERSION="unknown"
    fi
    export OS_NAME OS_VERSION
}

show_help() {
    cat << 'EOF'
Usage: ./upload_package.sh [OPTIONS] <version>

Upload ares-mlbdev2 package(s) to GitHub releases.

Arguments:
  version                   Version string (e.g., 2026.01.22-7cdb2000d9)

Options:
  --extra=EXTRA             Extra tag (e.g., boost1.83.0) - can be repeated
  --compiler=COMPILER       Compiler: gcc, clang (default: gcc)
  --compiler-version=VER    Compiler major version (default: 14)
  --build-type=TYPE         Build type: release, debug (default: release)
  --run, -r                 Actually execute upload (default: dry-run mode)
  --list, -l                List all available packages
  --help, -h                Show this help

Examples:
  # List packages
  ./upload_package.sh --list

  # Dry run
  ./upload_package.sh 2026.01.22-7cdb2000d9 --extra=boost1.83.0

  # Upload single variant
  ./upload_package.sh 2026.01.22-7cdb2000d9 --extra=boost1.83.0 --run

  # Upload all variants
  ./upload_package.sh 2026.01.22-7cdb2000d9 --extra=boost1.83.0 --extra=boost1.90.0 --run
EOF
}

list_packages() {
    [[ ! -d "$ARCHIVE_DIR" ]] && { echo "No archive directory: $ARCHIVE_DIR"; return 1; }
    echo "Available packages in $ARCHIVE_DIR:"
    find "$ARCHIVE_DIR" -maxdepth 1 -name "*.tar.gz" -type f -exec basename {} \; 2>/dev/null | sort
}

# Parse arguments
VERSION="" DRY_RUN=true LIST_MODE=false BUILD_TYPE="release"
declare -a EXTRAS=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        --help|-h) show_help; exit 0 ;;
        --list|-l) LIST_MODE=true ;;
        --run|-r) DRY_RUN=false ;;
        --extra=*) EXTRAS+=("${1#*=}") ;;
        --compiler=*) COMPILER="${1#*=}" ;;
        --compiler-version=*) COMPILER_VERSION_MAJOR="${1#*=}" ;;
        --build-type=*) BUILD_TYPE="${1#*=}" ;;
        -*) echo "Unknown option: $1"; show_help; exit 1 ;;
        *) VERSION="$1" ;;
    esac
    shift
done

[[ "$LIST_MODE" == "true" ]] && { list_packages; exit 0; }
[[ -z "$VERSION" ]] && { echo "Error: version required"; show_help; exit 1; }

set_compiler_version "$BUILD_TYPE"
get_os_info

RELEASE_TAG="ares-mlbdev2-${VERSION}"

# If no extras specified, find all matching archives
if [[ ${#EXTRAS[@]} -eq 0 ]]; then
    while IFS= read -r f; do
        [[ -n "$f" ]] && EXTRAS+=("$(basename "$f" .tar.gz | sed "s/ares-mlbdev2-${VERSION}-//" | sed "s/-${COMPILER}${COMPILER_VERSION_MAJOR}.*//")")
    done < <(find "$ARCHIVE_DIR" -name "ares-mlbdev2-${VERSION}-*-${COMPILER}${COMPILER_VERSION_MAJOR}-${BUILD_TYPE}-*.tar.gz" 2>/dev/null)
fi

[[ ${#EXTRAS[@]} -eq 0 ]] && { echo "No matching packages found for version $VERSION"; exit 1; }

echo "Release: $RELEASE_TAG"
echo "Packages:"
for extra in "${EXTRAS[@]}"; do
    archive="ares-mlbdev2-${VERSION}-${extra}-${COMPILER}${COMPILER_VERSION_MAJOR}-${BUILD_TYPE}-${OS_NAME}-${OS_VERSION}.tar.gz"
    if [[ -f "$ARCHIVE_DIR/$archive" ]]; then
        echo "  ✓ $archive"
    else
        echo "  ✗ $archive (not found)"
    fi
done
echo ""

if [[ "$DRY_RUN" == "true" ]]; then
    echo "[DRY RUN] Would upload to: https://github.com/$GITHUB_REPO/releases/tag/$RELEASE_TAG"
    echo "Add --run to actually upload"
    exit 0
fi

# Check gh auth
command -v gh &>/dev/null || { echo "Error: gh CLI required"; exit 1; }
gh auth status &>/dev/null || { echo "Error: Run 'gh auth login' first"; exit 1; }

# Collect archives
declare -a ARCHIVES=()
for extra in "${EXTRAS[@]}"; do
    archive="$ARCHIVE_DIR/ares-mlbdev2-${VERSION}-${extra}-${COMPILER}${COMPILER_VERSION_MAJOR}-${BUILD_TYPE}-${OS_NAME}-${OS_VERSION}.tar.gz"
    [[ -f "$archive" ]] && ARCHIVES+=("$archive")
done

[[ ${#ARCHIVES[@]} -eq 0 ]] && { echo "No archives to upload"; exit 1; }

# Create or update release
if gh release view "$RELEASE_TAG" --repo "$GITHUB_REPO" &>/dev/null; then
    echo "Uploading to existing release $RELEASE_TAG..."
    gh release upload "$RELEASE_TAG" "${ARCHIVES[@]}" --repo "$GITHUB_REPO"
else
    echo "Creating release $RELEASE_TAG..."
    gh release create "$RELEASE_TAG" \
        --repo "$GITHUB_REPO" \
        --title "ares-mlbdev2 $VERSION" \
        --notes "Release with variants: ${EXTRAS[*]}" \
        "${ARCHIVES[@]}"
fi

echo ""
echo "✓ Upload complete: https://github.com/$GITHUB_REPO/releases/tag/$RELEASE_TAG"
