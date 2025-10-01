#!/usr/bin/bash

arg="$1"

if [[ "$arg" == "test1.c" ]]; then
  gcc -g "test/$arg" -o test1 -Wall -Wextra 
else
  make
fi