#!/bin/bash
set -eu -o pipefail

if [ -f 'env.sh' ]; then
    . env.sh
fi

CXX=${CXX:-clang++}

COMMON_CFLAGS="-std=c++23 -stdlib=libc++ -fuse-ld=lld -Wall -Wextra -Wpedantic -Werror=vla -fno-omit-frame-pointer -g"
DEBUG_CFLAGS="-O0 -fsanitize=address,undefined"
RELEASE_CFLAGS="-O3 -march=native -flto=thin"

BIN="main"
SRC="./src/server/main.cpp"

#NOTE you can't compile statically and use the sanitizers at the same time
# -stdlib=libc++
# -static 
# -fsanitize=address,undefined -fno-omit-frame-pointer
# -flto
# -march=native
echo "[$BIN.d] compiling..."
set -x
bear -- "$CXX" $COMMON_CFLAGS $DEBUG_CFLAGS   -o $BIN.d $SRC
{ set +x; } 2> /dev/null

echo "[$BIN] compiling..."
set -x
"$CXX"         $COMMON_CFLAGS $RELEASE_CFLAGS -o $BIN   $SRC
{ set +x; } 2> /dev/null
