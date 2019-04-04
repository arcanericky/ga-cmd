#!/bin/bash

VERSION=$(git tag --points-at HEAD | head -1)
if [ -z "$VERSION" ]; then
  VERSION=$(git rev-parse HEAD | cut -c 1-7)
fi

echo $VERSION

