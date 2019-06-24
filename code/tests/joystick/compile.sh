#!/bin/bash
cmake . -B build > /dev/null
(cd build && make && cp ps4_test ..)
rm -r build
printf "\nCopied ps4_test to current directory.\n"
