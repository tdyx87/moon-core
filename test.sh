#!/bin/sh

echo "output:" $1

./build/debug/bin/test_core --gtest_filter=$1
