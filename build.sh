#!/bin/sh

fatal() { echo "ERROR: $@"; exit 1; }

docker version >/dev/null || fatal "Cannot use docker."

docker build -t builder -f Dockerfile-builder .
builder=$(docker run -d builder)

echo "Waiting for compile to complete."

until docker logs --tail 1 $builder | grep 'done.'; do
  sleep 10
done

docker cp ${builder}:/forever-loop .
docker rm ${builder}

docker build -t forever-loop .