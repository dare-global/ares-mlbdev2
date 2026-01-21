#!/bin/bash
# #############################################################################
# Fetch algo-utils scripts via Git sparse checkout
# #############################################################################
#
# Downloads only the scripts/ares directory from algo-utils repo.
# The checkout is:
#   - Sparse (only scripts/ares directory)
#   - Shallow (no git history, --depth 1)
#   - Pinned to a specific tag
#   - Read-only (files cannot be modified)
#   - Detached HEAD (cannot accidentally push)
#
# For *.latest tags, automatically checks if remote has updated.
#
# Usage (must be sourced):
#   source ./scripts/source-ares-scripts.sh
#
# This fetches ares scripts and sets PATH in your current shell.
#
# #############################################################################

set -e

# This script must be sourced, not executed
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    echo "This script must be sourced to set PATH in your shell:"
    echo ""
    echo "    source $0"
    echo ""
    exit 1
fi

# Configuration
ALGO_UTILS_REPO="git@github.com:dare-global/algo-utils.git"
ALGO_UTILS_DEFAULT_TAG="ares.scripts.latest"  # Pin to latest ares scripts

# Get script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
EXTERNAL_DIR="${PROJECT_ROOT}/external/algo-utils"
ALGO_UTILS_SCRIPTS="${EXTERNAL_DIR}/scripts/ares"

# Default values (arguments not supported when sourcing)
ALGO_UTILS_TAG="$ALGO_UTILS_DEFAULT_TAG"
FORCE_FETCH=false

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
print_warn() { echo -e "${YELLOW}[WARN]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }

# Function to get remote commit for a tag
get_remote_commit() {
    git ls-remote --refs "$ALGO_UTILS_REPO" "refs/tags/$1" 2>/dev/null | cut -f1
}

# Function to get local commit
get_local_commit() {
    if [ -d "$EXTERNAL_DIR/.git" ]; then
        (cd "$EXTERNAL_DIR" && git rev-parse HEAD 2>/dev/null)
    fi
}


# Check if we need to fetch
NEED_FETCH=false

if [ "$FORCE_FETCH" = true ]; then
    print_info "Force fetch requested"
    NEED_FETCH=true
elif [ ! -d "$EXTERNAL_DIR/.git" ]; then
    NEED_FETCH=true
else
    # Get current local commit
    LOCAL_COMMIT=$(get_local_commit)
    
    # For *.latest tags, always check remote for updates
    if [[ "$ALGO_UTILS_TAG" == *".latest" ]]; then
        print_info "Checking for updates to ${ALGO_UTILS_TAG}..."
        REMOTE_COMMIT=$(get_remote_commit "$ALGO_UTILS_TAG")
        
        if [ -n "$REMOTE_COMMIT" ] && [ "$LOCAL_COMMIT" != "$REMOTE_COMMIT" ]; then
            print_info "Remote has updates (local: ${LOCAL_COMMIT:0:8}, remote: ${REMOTE_COMMIT:0:8})"
            NEED_FETCH=true
        else
            print_info "algo-utils already up to date (${LOCAL_COMMIT:0:8})"
        fi
    else
        # For fixed tags, just check if we have the right tag
        CURRENT_REF=$(cd "$EXTERNAL_DIR" && git describe --tags --exact-match 2>/dev/null || echo "unknown")
        if [ "$CURRENT_REF" != "$ALGO_UTILS_TAG" ]; then
            NEED_FETCH=true
        else
            print_info "algo-utils already at ${ALGO_UTILS_TAG}"
        fi
    fi
fi

if [ "$NEED_FETCH" = false ]; then
    # Still set PATH even if no fetch needed
    export PATH="${ALGO_UTILS_SCRIPTS}:$PATH"
    export SRC_PATH="${PROJECT_ROOT}"
    return 0
fi

# Remove existing checkout if present
if [ -d "$EXTERNAL_DIR" ]; then
    print_warn "Removing old version to fetch ${ALGO_UTILS_TAG}..."
    chmod -R u+w "$EXTERNAL_DIR" 2>/dev/null || true
    rm -rf "$EXTERNAL_DIR"
fi

print_info "Fetching algo-utils@${ALGO_UTILS_TAG} (sparse checkout)..."

# Create external directory if needed
mkdir -p "$(dirname "$EXTERNAL_DIR")"

# Sparse checkout with specific tag/branch
git clone --filter=blob:none --sparse --depth 1 \
    --branch "$ALGO_UTILS_TAG" \
    "$ALGO_UTILS_REPO" "$EXTERNAL_DIR"

cd "$EXTERNAL_DIR"

# Configure sparse checkout to only include scripts/ares
git sparse-checkout set scripts/ares

# Detach HEAD - prevents accidental commits/pushes
git checkout --detach HEAD 2>/dev/null || true

# Remove git hooks to prevent any git operations from running hooks
rm -rf .git/hooks/* 2>/dev/null || true

# Make everything read-only to prevent modifications
find . -type f -exec chmod a-w {} \; 2>/dev/null || true
find . -type d -exec chmod a-w {} \; 2>/dev/null || true

print_info "✓ algo-utils@${ALGO_UTILS_TAG} installed (read-only)"
print_info "  Location: ${EXTERNAL_DIR}"
print_info "  Scripts:  ${ALGO_UTILS_SCRIPTS}/"

# List available scripts
echo ""
print_info "Available scripts:"
ls -1 "${ALGO_UTILS_SCRIPTS}/"*.sh 2>/dev/null | while read -r script; do
    echo "    $(basename "$script")"
done

# Export PATH and SRC_PATH to current shell
export PATH="${ALGO_UTILS_SCRIPTS}:$PATH"
export SRC_PATH="${PROJECT_ROOT}"
echo ""
print_info "PATH updated. You can now run: build.sh gcc, upload_package.sh --list, etc."
