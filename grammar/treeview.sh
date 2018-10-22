#!/bin/bash

ANTLR_PATH="/usr/local/Cellar/antlr/4.7.1/antlr-4.7.1-complete.jar"

if [ ! -f $ANTLR_PATH ]; then
    echo "This script is hard-coded to work on my machine, you need to update it to make it work... :)"
    exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

antlr4 -o "$DIR/tmp/" c99.g4 || (echo "antlr generation failed" && exit 1)
cp c99.g4 "$DIR/tmp/"
cd "$DIR/tmp/" && javac -cp "$ANTLR_PATH" *.java || (echo "parser compilation failed" && exit 1)

cd "$DIR/tmp/" && java -cp ".:$ANTLR_PATH" org.antlr.v4.gui.TestRig c99 functionDefinition -gui "$@"
