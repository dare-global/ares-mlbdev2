#!/bin/bash
# Source this to fetch ares scripts and set PATH
# Usage: source ./scripts/source-ares-scripts.sh

[[ "${BASH_SOURCE[0]}" == "${0}" ]] && { echo "Usage: source $0"; exit 1; }

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
ALGO_UTILS="${PROJECT_ROOT}/external/algo-utils"
SCRIPTS_DIR="${ALGO_UTILS}/scripts/ares"
REPO="git@github.com:dare-global/algo-utils.git"
TAG="ares.scripts.latest"

# Check if update needed
need_update() {
    [[ ! -d "$ALGO_UTILS/.git" ]] && return 0
    local remote=$(git ls-remote --refs "$REPO" "refs/tags/$TAG" 2>/dev/null | cut -f1)
    local local=$(cd "$ALGO_UTILS" && git rev-parse HEAD 2>/dev/null)
    [[ "$remote" != "$local" ]]
}

if need_update; then
    echo "Fetching algo-utils@${TAG}..."
    chmod -R u+w "$ALGO_UTILS" 2>/dev/null; rm -rf "$ALGO_UTILS"
    git -c advice.detachedHead=false clone --filter=blob:none --sparse --depth 1 -b "$TAG" "$REPO" "$ALGO_UTILS" 2>/dev/null
    (cd "$ALGO_UTILS" && git sparse-checkout set scripts/ares)
    find "$ALGO_UTILS" -type f -exec chmod a-w {} \; 2>/dev/null
    echo "✓ algo-utils ready"
fi

export SRC_PATH="$PROJECT_ROOT"
export PATH="${SCRIPTS_DIR}:$PATH"
