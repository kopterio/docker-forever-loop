#!/bin/sh

fatal() { echo "ERROR: $@"; exit 1; }

docker version >/dev/null || fatal "Cannot use docker."
docker build -t forever-http-loop .
