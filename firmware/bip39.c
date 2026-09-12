/*
*******************************************************************************
 BIP-39

 Implementation
*******************************************************************************
*/

#include "bip39.h"

#include "sha256.h"
#include <string.h>

void bip39_encode(const uint8_t *entropy,
                  uint16_t entropy_bits,
                  uint16_t words[BIP39_MAX_WORDS])
{
    uint16_t i;
    uint16_t j;
    uint16_t value;
    uint8_t entropy_bytes;
    uint8_t checksum_bits;
    uint8_t word_count;
    uint8_t buffer[BIP39_MAX_ENTROPY_BYTES + 1];
    uint8_t hash[32];
    SHA256_CTX ctx;

    memset(words, 0, BIP39_MAX_WORDS * sizeof(words[0]));
    memset(buffer, 0, sizeof(buffer));

    if (entropy_bits != 128 &&
        entropy_bits != 192 &&
        entropy_bits != 256)
    {
        return;
    }

    entropy_bytes = (uint8_t)(entropy_bits / 8);
    checksum_bits = (uint8_t)(entropy_bits / 32);
    word_count = (uint8_t)((entropy_bits + checksum_bits) / 11);

    // SHA-256 is computed over exactly the selected amount of entropy
    sha256_init(&ctx);
    sha256_update(&ctx, entropy, entropy_bytes);
    sha256_final(&ctx, hash);

    // append the checksum byte; only the required leading bits are consumed
    memcpy(buffer, entropy, entropy_bytes);
    buffer[entropy_bytes] = hash[0];

    // split ENT || CS into 11-bit BIP-39 indexes
    for (i = 0; i < word_count; i++)
    {
        value = 0;

        for (j = 0; j < 11; j++)
        {
            uint16_t bit = i * 11 + j;

            value <<= 1;

            if (buffer[bit / 8] & (0x80 >> (bit % 8)))
            {
                value |= 1;
            }
        }

        words[i] = value;
    }
}
