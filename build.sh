#!/bin/sh

set -xe

gcc -DDEBUG -g -ggdb -O0 -o hvm value.c scanner.c utils.c chunk.c hvm.c compiler.c main.c 
