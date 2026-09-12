/*
*******************************************************************************
 Entropy

 Collects user provided entropy
*******************************************************************************
*/

#ifndef ENTROPY_H
#define ENTROPY_H

#include <stdint.h>

typedef enum
{
    ENTROPY_VN_WAITING = 0,
    ENTROPY_VN_DISCARDED,
    ENTROPY_VN_ADDED_0,
    ENTROPY_VN_ADDED_1

} entropy_vn_result_t;

void entropy_clear(void); // clear the buffer
void entropy_add_coin_bit(uint8_t bit); // add one bit from a coin flip
entropy_vn_result_t entropy_add_von_neumann_coin_bit(uint8_t bit); // process one bit from a Von Neumann coin flip
void entropy_add_dice_roll(uint8_t roll, uint16_t target_bits); // add bits from a dice roll
uint16_t entropy_count(void); // number of bits currently stored
uint8_t entropy_is_full(uint16_t target_bits); // zero = not full, non-zero = target reached
uint8_t entropy_get_bit(uint16_t index); // read one stored bit; returns zero if index is not valid
void entropy_get(uint8_t *buffer); // pack stored entropy into a 32-byte buffer, zero-padding unused bytes

#endif
