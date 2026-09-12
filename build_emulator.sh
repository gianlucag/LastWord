#!/bin/sh

set -e

mkdir -p bin

gcc \
    firmware/main.c \
    firmware/board_emulator.c \
    firmware/lcd_emulator.c \
    firmware/delay_emulator.c \
    firmware/input_emulator.c \
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
    -Wall \
    -Wextra \
    -Werror \
	-Wpedantic \
	-Wshadow \
    -std=c99 \
    -o bin/lastword