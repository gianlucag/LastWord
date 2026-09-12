#!/bin/sh

XC8=/Applications/microchip/xc8/v3.10/bin/xc8-cc
DFP=/Applications/microchip/mplabx/v6.35/packs/Microchip/PIC18F-K_DFP/1.16.308/xc8

mkdir -p bin

$XC8 \
    -mcpu=18F47K42 \
	-mdfp="$DFP" \
	-o bin/lastword.hex \
    firmware/main.c \
    firmware/board_pic18.c \
    firmware/lcd_pic18.c \
    firmware/delay_pic18.c \
    firmware/input_pic18.c \
    firmware/sha256.c \
    firmware/bip39.c \
    firmware/entropy.c \
    firmware/wordlist.c \
    firmware/tests.c \
    firmware/menu.c \
    firmware/splash.c \
	firmware/entropy_input.c \
    firmware/self_test.c \
	firmware/about.c \

# keep only the final HEX file

find bin -type f \( \
    -name "*.o"   -o \
    -name "*.obj" -o \
    -name "*.d"   -o \
    -name "*.p1"  -o \
    -name "*.elf" -o \
    -name "*.map" -o \
    -name "*.lst" -o \
    -name "*.sym" -o \
    -name "*.cmf" -o \
    -name "*.hxl" -o \
    -name "*.sdb" -o \
    -name "*.rlf" -o \
    -name "*.pre" -o \
    -name "*.as"  -o \
    -name "*.i"   -o \
	-name "*.s"   -o \
    -name "*.cod" \
\) -delete