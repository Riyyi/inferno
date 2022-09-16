#!/bin/sh

# Manage pre-commit hooks in .git/hooks
# Depends: -

# ------------------------------------------

scriptName="$(basename "$0")"

b="$(tput bold)"
u="$(tput smul)"
red="$(tput setf 4)"
n="$(tput sgr0)"

help() {
	cat << EOF
${b}NAME${n}
	${scriptName} - manage pre-commit hooks

${b}SYNOPSIS${n}
	${b}${scriptName}${n} ${u}COMMAND${n}

${b}COMMANDS${n}
	${b}install${n}
		Install all pre-commit hooks.

	${b}remove${n}
		Remove all pre-commit hooks.
EOF
}

# Exit if no option is provided
[ "$#" -eq 0 ] && help && exit 1

if [ ! -d ".git" ]; then
   echo "${b}${red}Error:${n} please run this script from the project root." >&2
   exit 1
fi

currentDir="$(pwd -P)"

# Get the path from the project root to the script
subDir="$(dirname -- "$0")"

# Get the full path to this script while handling spaces and symlinks correctly
scriptPath="$(cd -P -- "$subDir" && pwd -P)"
cd "$scriptPath/.." || exit 1

hooks="
lint-ci.sh
"

install() {
	echo "Installing pre-commit hooks"

	preCommit="$currentDir/.git/hooks/pre-commit"
	if ! test -f "$preCommit"; then
		touch "$preCommit"
		chmod +x "$preCommit"
		echo "#!/bin/sh" > "$preCommit"
	fi

	for hook in $hooks; do
		sed -Ei "/$hook/d" "$preCommit"
		sed -Ei "\$ a $subDir/$hook" "$preCommit"
	done
}

remove() {
	echo "Removing pre-commit hooks"

	preCommit=".git/hooks/pre-commit"
	for hook in $hooks; do
		sed -Ei "/$hook/d" "$preCommit"
	done
}

# Command handling
shift $((OPTIND - 1))
case "$1" in
	install | remove)
		"$1"
		;;
	*)
		echo "$scriptName: invalid command '$1'"
		echo "Try '$scriptName -h' for more information."
		exit 1
		;;
esac
