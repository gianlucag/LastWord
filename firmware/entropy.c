/*
*******************************************************************************
 Entropy

 Collects user provided entropy
*******************************************************************************
*/

#include "entropy.h"
#include <string.h>

#define ENTROPY_MAX_BITS 256

static uint8_t entropy[ENTROPY_MAX_BITS];
static uint16_t bit_count;
static uint8_t vn_have_first;
static uint8_t vn_first;

static void entropy_add_bit(uint8_t bit)
{
    if (bit_count >= ENTROPY_MAX_BITS)
        return;

    entropy[bit_count++] = bit ? 1 : 0;
}

void entropy_clear(void)
{
    memset(entropy, 0, sizeof(entropy));

    bit_count = 0;
    vn_have_first = 0;
    vn_first = 0;
}

uint16_t entropy_count(void)
{
    return bit_count;
}

uint8_t entropy_is_full(uint16_t target_bits)
{
    if (target_bits == 0 || target_bits > ENTROPY_MAX_BITS)
    {
        return 0;
    }

    return bit_count >= target_bits;
}

uint8_t entropy_get_bit(uint16_t index)
{
    if (index >= bit_count)
    {
        return 0;
    }

    return entropy[index];
}

void entropy_get(uint8_t *buffer)
{
    uint16_t i;

    memset(buffer, 0, 32);

    for (i = 0; i < ENTROPY_MAX_BITS; i++)
    {
        if (entropy[i])
        {
            buffer[i / 8] |= 0x80 >> (i % 8);
        }
    }
}

void entropy_add_coin_bit(uint8_t bit)
{
    entropy_add_bit(bit);
}

entropy_vn_result_t entropy_add_von_neumann_coin_bit(uint8_t bit)
{
    bit = bit ? 1 : 0;

    // first bit of the pair
    if (!vn_have_first)
    {
        vn_first = bit;
        vn_have_first = 1;
        return ENTROPY_VN_WAITING;
    }

    // complete pair received
    vn_have_first = 0;

    // discard equal pairs
    if (vn_first == bit)
    {
        return ENTROPY_VN_DISCARDED;
    }

    /* 01 -> 0
       10 -> 1 */
    entropy_add_bit(vn_first);

    return vn_first ? ENTROPY_VN_ADDED_1 : ENTROPY_VN_ADDED_0;
}

void entropy_add_dice_roll(uint8_t roll, uint16_t target_bits)
{
    uint8_t first_bit;
    uint8_t second_bit;

    if (target_bits == 0 || target_bits > ENTROPY_MAX_BITS)
    {
        return;
    }

    if (bit_count >= target_bits)
    {
        return;
    }

    switch (roll)
    {
        case 1:
            first_bit = 0;
            second_bit = 0;
            break;

        case 2:
            first_bit = 0;
            second_bit = 1;
            break;

        case 3:
            first_bit = 1;
            second_bit = 0;
            break;

        case 4:
            first_bit = 1;
            second_bit = 1;
            break;

        case 5:
        case 6:
        default:
            // discard roll
            return;
    }

    // always use the first bit
    entropy_add_bit(first_bit);

    /*
     * Never carry the second bit into another 11-bit BIP-39 word.
     * Also discard it if the selected entropy length was just reached.
     */
    if (bit_count < target_bits && (bit_count % 11) != 0)
    {
        entropy_add_bit(second_bit);
    }
}
