#!/bin/sh

# Manage pre-commit hooks in .git/hooks
# Depends: -

# ------------------------------------------

scriptName="$(basename "$0")"

help() {
	b=$(tput bold)
	u=$(tput smul)
	nc=$(tput sgr0)

	cat << EOF
${b}NAME${nc}
	${scriptName} - manage pre-commit hooks

${b}SYNOPSIS${nc}
	${b}${scriptName}${nc} ${u}COMMAND${nc}

${b}COMMANDS${nc}
	${b}install${nc}
		Install all pre-commit hooks.

	${b}remove${nc}
		Remove all pre-commit hooks.
EOF
}

# Exit if no option is provided
[ "$#" -eq 0 ] && help && exit 1

# Get the full path to this script while handling spaces and symlinks correctly
scriptPath="$(cd -P -- "$(dirname -- "$0")" && pwd -P)"
cd "$scriptPath/.." || exit 1

hooks="
lint-ci.sh
"

install() {
	echo "Installing pre-commit hooks"

	preCommit=".git/hooks/pre-commit"
	if ! test -f "$preCommit"; then
		touch "$preCommit"
		chmod +x "$preCommit"
		echo "#!/bin/sh" > "$preCommit"
	fi

	for hook in $hooks; do
		sed -Ei "/$hook/d" "$preCommit"
		sed -Ei "\$ a script/$hook" "$preCommit"
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
