#!/usr/bin/env python3

"""
converter.py

Converts the official Trezor BIP-39 vectors.json into the C test
vector format used by the LastWord firmware.

Usage:
    python3 converter.py vectors.json > bip39_tests.inc
"""

import json
import sys

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} vectors.json")
    sys.exit(1)

with open(sys.argv[1], "r") as f:
    vectors = json.load(f)

vectors = vectors["english"]

print("static const bip39_test_t bip39_tests[] =")
print("{")

test_number = 1

for vector in vectors:

    entropy_hex = vector[0]
    mnemonic = vector[1]

    # Keep only 256-bit vectors (32 bytes)
    if len(entropy_hex) != 64:
        continue

    entropy = bytes.fromhex(entropy_hex)
    words = mnemonic.split()

    print("    {")
    print(f'        "Test {test_number}",')

    print("        {")

    for i in range(0, 32, 8):
        print("            ", end="")

        for j in range(8):
            index = i + j
            print(f"0x{entropy[index]:02x}", end="")
            if index != 31:
                print(",", end="")

        print()

    print("        },")

    print("        {")

    for i in range(0, 24, 4):
        print("            ", end="")

        for j in range(4):
            index = i + j
            print(f'"{words[index]}"', end="")
            if index != 23:
                print(",", end="")

        print()

    print("        }")
    print("    },")

    test_number += 1

print("};")