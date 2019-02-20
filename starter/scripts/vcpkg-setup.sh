#!/bin/bash

# Logging Prefix 
PREFIX="[vcpkg-setup]"

# Useful colors (for logging)
GREEN='\033[1;32m'
WHITE='\033[1;37m'
RED='\033[1;31m'
NC='\033[0m' # No Color

log() {
    echo -e "${GREEN}$PREFIX${WHITE} $*${NC}"
}

die() {
    echo -e "${RED}$PREFIX $*${NC}" 1>&2
    exit 1
}

log "Initializing and updating vcpkg submodule..."
git submodule sync --recursive || die "Error while synchronizing vcpkg submodule repo!"
git submodule update --init --recursive || die "Error while initializing/updating vcpkg submodule!"
git submodule foreach git checkout master || die "Errpr while checking out vcpkg submodule!"

cd vcpkg
./bootstrap-vcpkg.sh || die "Error while bootstraping vcpkg submodule!"

./vcpkg version || die "Could not find vcpkg binary!"

log "vcpkg is up!"
cd -

