#!/bin/sh

# Run all linters
# Depends: git

# ------------------------------------------

# Get the full path to this script while handling spaces and symlinks correctly
scriptPath="$(cd -P -- "$(dirname -- "$0")" && pwd -P)"
cd "$scriptPath/.." || exit 1

green=$(tput setf 2)
red=$(tput setf 4)
nc=$(tput sgr0)

failures=0

linters="
lint-shell-script.sh
"

for linter in $linters; do
	echo "Running script/$linter"
	if "script/$linter"; then
		echo "[${green}PASS${nc}]: script/$linter"
	else
		echo "[${red}FAIL${nc}]: script/$linter"
		failures=$(( failures + 1 ))
	fi
done

# Get all files staged for commit
files="$(git diff --cached --name-only)"

echo "Running script/lint-clang-format.sh"
# shellcheck disable=SC2086
if script/lint-clang-format.sh && git diff --exit-code $files; then
    echo "[${green}PASS${nc}]: script/lint-clang-format.sh"
else
    echo "[${red}FAIL${nc}]: script/lint-clang-format.sh"
	failures=$(( failures + 1 ))
fi

exit "$failures"
