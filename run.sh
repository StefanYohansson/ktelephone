#!/bin/bash
DIR="$(dirname "$(readlink -f "$0")")"
LD_LIBRARY_PATH="$DIR" "$DIR/ktelephone" "$@"
