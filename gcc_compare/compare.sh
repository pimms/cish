#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

term() {
    echo $@
    exit 1
}

which cish_cli > /dev/null || term "cish_cli must be on the path"

FILES=$(ls $DIR/*.c)
GCCDIR=$DIR/.tmp

mkdir -p $GCCDIR

while read file
do
    gcc "$file" -o $GCCDIR/a.out || term "failed to compile '$file'"

    GCC_OUT=$($GCCDIR/a.out)
    GCC_CODE=$?

    CISH_OUT=$(cish_cli $file)
    CISH_CODE=$?

    EMSG=""
    OK=1

    if [ $CISH_CODE -ne $GCC_CODE ]; then
        EMSG="$EMSG""Expected exit code $GCC_CODE, got $CISH_CODE\n"
        OK=0
    fi
    if [ "$CISH_OUT" != "$GCC_OUT" ]; then
        EMSG="$EMSG""Expected output:\n\t$GCC_OUT\nGot output:\n\t$CISH_OUT\n"
        OK=0
    fi

    if [ "$OK" -eq 1 ]; then
        echo -e "\r[✅] $file"
    else
        echo -e $EMSG
        echo -e "\r[❌] $file"
        echo
    fi

done < <(ls $DIR/*.c)
