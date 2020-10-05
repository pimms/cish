#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

term() {
    echo $@
    exit 1
}

compare_file() {
    gcc -w "$file" -o $GCCDIR/a.out || term "failed to compile '$file'"

    CLI_ARGS=$(cat $file | grep -e "//\s*args" | sed -e "s/\/\/.*args//")

    GCC_OUT=$($GCCDIR/a.out $CLI_ARGS)
    GCC_CODE=$?

    CISH_ARGS=$(cat $file | grep -e "//\s*cish_cli" | sed -e "s/\/\/.*cish_cli//")
    CISH_OUT=$(cish_cli $CISH_ARGS $file $CLI_ARGS)
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
        echo -e "\r[✅] `basename $file`"
    else
        echo -e $EMSG
        echo -e "\r[❌] `basename $file`"
        echo
    fi
}

which cish_cli > /dev/null || term "cish_cli must be on the path"

FILES=$(ls $DIR/*.c)
GCCDIR=$DIR/.tmp

mkdir -p $GCCDIR


if [ "$#" -eq 0 ]; then
    while read file
    do
        compare_file $file
    done < <(ls $DIR/*.c)
else
    for file in "$@"
    do
        compare_file $file
    done
fi
