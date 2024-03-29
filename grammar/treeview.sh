#!/bin/bash

ANTLR_PATH="/opt/homebrew/Cellar/antlr/4.9.2/antlr-4.9.2-complete.jar"

if [ ! -f $ANTLR_PATH ]; then
    echo "This script is hard-coded to work on my machine, you need to update it to make it work... :)"
    exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

antlr -o "$DIR/tmp/" CM.g4 || (exit 1)
cp CM.g4 "$DIR/tmp/"
cd "$DIR/tmp/" && javac -cp "$ANTLR_PATH" *.java || (exit 1)

cd "$DIR/tmp/" && java -cp ".:$ANTLR_PATH" org.antlr.v4.gui.TestRig CM root -gui "$@"
