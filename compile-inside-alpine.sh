#!/bin/sh

apk update
apk add gcc musl-dev
gcc -o forever-loop forever-loop.c -static
strip forever-loop
echo "done."
