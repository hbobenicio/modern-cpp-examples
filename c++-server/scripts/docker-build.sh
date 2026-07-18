#!/bin/bash
set -eu -o pipefail

echo "building docker image..."
docker build . -t apague:0.0.0 --progress=plain --no-cache

echo "creating temporary container..."
CONTAINER_ID=$(docker create --name apague apague:0.0.0)
echo "CONTAINER_ID: $CONTAINER_ID"

echo "copying artifacts from temporary container..."
docker cp "$CONTAINER_ID:/usr/local/main" "$PWD/main"

echo "removing temporary container..."
docker container rm "$CONTAINER_ID"
echo "done."

