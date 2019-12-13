#!/usr/bin/env zsh
function glob_sources {
    base_dir=$1
    glob_target=$2
    pattern=$base_dir
    find $pattern | grep -e /$glob_target$
}

glob_sources $1 $2