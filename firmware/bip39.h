/*
*******************************************************************************
 BIP-39

 Encodes user supplied entropy and checksum into BIP-39 word indexes
*******************************************************************************
*/

#ifndef BIP39_H
#define BIP39_H

#include <stdint.h>

#define BIP39_MAX_WORDS 24
#define BIP39_MAX_ENTROPY_BYTES 32

void bip39_encode(const uint8_t *entropy,
                  uint16_t entropy_bits,
                  uint16_t word_indexes[BIP39_MAX_WORDS]);

#endif
