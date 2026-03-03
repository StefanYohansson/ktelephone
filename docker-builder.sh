#!/usr/bin/env sh

DISTRO="${1:-fedora43}"

docker build \
    -t ktelephone \
    -f Dockerfile.$DISTRO \
    .
docker create --name ktelephone ktelephone
mkdir -p build
docker cp ktelephone:/export/. ./build/
docker rm ktelephone

cp -r run.sh build/
