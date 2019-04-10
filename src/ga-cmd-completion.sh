#!/usr/bin/env bash

_get_ga_cmd_completions() {
    if [ "${#COMP_WORDS[@]}" != "2" ]; then
        return
    fi

    LIST=""
    for NAME in $(ga-cmd -l)
    do
        LIST="$LIST $NAME"
    done
    COMPREPLY=($(compgen -W "$LIST" "${COMP_WORDS[1]}"))
}

complete -F _get_ga_cmd_completions ga-cmd