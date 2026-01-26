#!/bin/bash
# Source this to fetch ares scripts and cmake modules from algo-utils
# Usage: source scripts/source-algo-utils.sh
#
# After sourcing, you can use:
#   - build.sh commands (build.sh gcc, build.sh test, etc.)
#   - Aliases: bgd (build gcc debug), bcd (build clang debug), etc.
#
# Run 'aliases' to see all available aliases.

[[ "${BASH_SOURCE[0]}" == "${0}" ]] && { echo "Usage: source $0"; exit 1; }

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
ALGO_UTILS="${PROJECT_ROOT}/external/algo-utils"
SCRIPTS_DIR="${ALGO_UTILS}/scripts/ares"
CMAKE_DIR="${ALGO_UTILS}/cmake"
REPO="git@github.com:dare-global/algo-utils.git"
TAG="ares.scripts.latest"

# Check if update needed
need_update() {
    [[ ! -d "$ALGO_UTILS/.git" ]] && return 0
    # Use || true to prevent set -e from killing the script on ls-remote failure
    local remote=$(git ls-remote --refs "$REPO" "refs/tags/$TAG" 2>&1 | grep -v "^fatal:" | cut -f1) || true
    local local=$(cd "$ALGO_UTILS" && git rev-parse HEAD 2>/dev/null) || true
    [[ "$remote" != "$local" ]]
}

if need_update; then
    echo "Fetching algo-utils@${TAG}..."
    echo "  GIT_CONFIG_GLOBAL=${GIT_CONFIG_GLOBAL:-<not set>}"
    echo "  REPO=$REPO"
    chmod -R u+w "$ALGO_UTILS" 2>/dev/null || true
    rm -rf "$ALGO_UTILS"
    
    # Show git config for debugging
    echo "  Git URL rewrites:"
    git config --global --get-regexp '^url\.' 2>/dev/null || echo "    (none configured)"
    
    if ! git -c advice.detachedHead=false clone --filter=blob:none --sparse --depth 1 -b "$TAG" "$REPO" "$ALGO_UTILS" 2>&1; then
        echo "ERROR: Failed to clone algo-utils from $REPO (tag: $TAG)" >&2
        return 1
    fi
    (cd "$ALGO_UTILS" && git sparse-checkout set scripts/ares cmake)
    find "$ALGO_UTILS" -type f -exec chmod a-w {} \; 2>/dev/null || true
    echo "✓ algo-utils ready (scripts + cmake)"
fi

export SRC_PATH="$PROJECT_ROOT"
export PATH="${SCRIPTS_DIR}:$PATH"
export ALGO_UTILS_CMAKE="${CMAKE_DIR}"

# Source aliases for convenient build commands
if [[ -f "${SCRIPTS_DIR}/aliases.sh" ]]; then
    source "${SCRIPTS_DIR}/aliases.sh"
fi
