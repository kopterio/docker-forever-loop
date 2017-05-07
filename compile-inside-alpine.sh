#!/bin/sh

apk update
apk add gcc musl-dev
gcc -Wall -o forever-http-loop forever-http-loop.c -static
strip forever-http-loop
ls -altr forever-http-loop
echo "done."
