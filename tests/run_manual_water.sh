#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
test_dir=$(mktemp -d)
trap 'rm -rf "$test_dir"' EXIT
c++ -std=c++17 -I BLELib2_cpp tests/manual_water_codec.cpp BLELib2_cpp/FunShare.cpp BLELib2_cpp/AESFunctions.cpp -o "$test_dir/codec"
"$test_dir/codec"
