#!/bin/sh

# Run shellcheck across the codebase
# Depends: git, shellcheck

# ------------------------------------------

# Get the full path to this script while handling spaces and symlinks correctly
scriptPath="$(cd -P -- "$(dirname -- "$0")" && pwd -P)"
cd "$scriptPath/.." || exit 1

if ! command -v shellcheck > /dev/null 2>&1; then
	echo "shellcheck is not available, but shell script files need linting!"
	echo "Either skip this script, or install shellcheck."
	exit 1
fi

files="${1:-$(git --no-pager diff --cached --name-only)}"
files="$(echo "$files" | grep -E '\.sh$')"

if [ -z "$files" ]; then
	echo "No .sh files to check."
	exit
fi

# shellcheck disable=SC2086
shellcheck $files
