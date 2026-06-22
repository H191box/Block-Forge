#!/bin/bash
set -e

docker build -t gba-block-forge .

docker run --rm \
    -v "$(pwd)":/project \
    -w /project \
    gba-block-forge \
    make
