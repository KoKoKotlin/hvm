#!/bin/sh

set -xe

gcc -g -ggdb -O0 -o hvm value.c utils.c chunk.c hvm.c main.c 
