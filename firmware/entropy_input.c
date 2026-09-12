/*
*******************************************************************************
 Entropy input

 Acquires and displays entropy supplied by the user
*******************************************************************************
*/

#include "entropy_input.h"
#include "lcd.h"
#include "input.h"
#include "delay.h"
#include "entropy.h"
#include "wordlist.h"
#include "bip39.h"

#define MAX_WORD_COUNT 24
#define MAX_ENTROPY_BYTES 32
#define MNEMONIC_LENGTH_COUNT 3
#define FEEDBACK_DELAY_MS 1000

typedef enum
{
    ENTROPY_COIN = 0,
    ENTROPY_VON_NEUMANN_COIN,
    ENTROPY_DICE_ROLLS

} entropy_source_t;

typedef struct
{
    uint8_t word_count;
    uint16_t entropy_bits;

} mnemonic_config_t;

#define ENTROPY_SOURCE_COUNT 3

static uint8_t packed_entropy[MAX_ENTROPY_BYTES];
static uint16_t word_indexes[MAX_WORD_COUNT];
static mnemonic_config_t active_config;

static const mnemonic_config_t mnemonic_configs[MNEMONIC_LENGTH_COUNT] =
{
	{ 24, 256 },
    { 18, 192 },
    { 12, 128 },
};

static const char *mnemonic_labels[MNEMONIC_LENGTH_COUNT] =
{
    "24 WORDS",
    "18 WORDS",
    "12 WORDS"
};

static const char *entropy_sources[ENTROPY_SOURCE_COUNT] =
{
    "COIN FLIPS",
    "VON NEUMANN COIN",
    "DICE ROLLS"
};

static uint8_t checksum_bits(void)
{
    return (uint8_t)(active_config.entropy_bits / 32);
}

static uint8_t last_word_entropy_bits(void)
{
    return (uint8_t)(11 - checksum_bits());
}

static uint8_t complete_entropy_words(void)
{
    return (uint8_t)(active_config.word_count - 1);
}

static uint16_t last_word_start_bit(void)
{
    return (uint16_t)(active_config.word_count - 1) * 11;
}

static void select_mnemonic_length(void)
{
    uint8_t current = 0;

    while (1)
    {
        lcd_write_line1(mnemonic_labels[current]);
        lcd_write_line2("A=OK  B=NEXT");

        input_wait();

        if (input_get() == INPUT_BUTTON_B)
        {
            current++;

            if (current >= MNEMONIC_LENGTH_COUNT)
            {
                current = 0;
            }
        }
        else
        {
            active_config = mnemonic_configs[current];
            return;
        }
    }
}

static void wait_start(const char *title)
{
    lcd_write_line1(title);
    lcd_write_line2("A=START");

    while (1)
    {
        input_wait();

        if (input_get() == INPUT_BUTTON_A)
        {
            return;
        }
    }
}

static uint8_t append_text(char *line, uint8_t position, const char *text)
{
    while (*text != '\0' && position < 16)
    {
        line[position++] = *text++;
    }

    line[position] = '\0';
    return position;
}

static uint8_t append_uint16(char *line, uint8_t position, uint16_t value)
{
    char digits[5];
    uint8_t count = 0;

    do
    {
        digits[count++] = (char)('0' + (value % 10));
        value /= 10;
    }
    while (value != 0 && count < sizeof(digits));

    while (count > 0 && position < 16)
    {
        line[position++] = digits[--count];
    }

    line[position] = '\0';
    return position;
}

static void format_word_number(char line[17], uint8_t word_number)
{
    line[0] = 'W';
    line[1] = (char)('0' + (word_number / 10));
    line[2] = (char)('0' + (word_number % 10));
    line[3] = ' ';
}

static void format_entropy_word_line(uint8_t word_number, char line[17])
{
    uint16_t start_bit;
    uint16_t bit;
    uint8_t i;

    format_word_number(line, word_number);
    start_bit = (uint16_t)(word_number - 1) * 11;

    for (i = 0; i < 11; i++)
    {
        bit = start_bit + i;

        if (bit < entropy_count())
        {
            line[4 + i] = entropy_get_bit(bit) ? '1' : '0';
        }
        else
        {
            line[4 + i] = '_';
        }
    }

    line[15] = '\0';
}

static void format_last_word_input_line(char line[17])
{
    uint8_t i;
    uint8_t entropy_part_bits;
    uint8_t cs_bits;
    uint8_t position;
    uint16_t start_bit;
    uint16_t bit;

    entropy_part_bits = last_word_entropy_bits();
    cs_bits = checksum_bits();
    start_bit = last_word_start_bit();

    format_word_number(line, active_config.word_count);
    position = 4;

    for (i = 0; i < entropy_part_bits; i++)
    {
        bit = start_bit + i;

        if (bit < entropy_count())
        {
            line[position++] = entropy_get_bit(bit) ? '1' : '0';
        }
        else
        {
            line[position++] = '_';
        }
    }

    line[position++] = '|';

    for (i = 0; i < cs_bits; i++)
    {
        line[position++] = '_';
    }

    line[position] = '\0';
}

static void format_current_word_line(char line[17])
{
    uint16_t count = entropy_count();

    if (count < last_word_start_bit())
    {
        format_entropy_word_line((uint8_t)(count / 11 + 1), line);
    }
    else
    {
        format_last_word_input_line(line);
    }
}

static uint16_t entropy_word_index(uint8_t word_number)
{
    uint16_t start_bit;
    uint16_t value = 0;
    uint8_t i;

    start_bit = (uint16_t)(word_number - 1) * 11;

    for (i = 0; i < 11; i++)
    {
        value <<= 1;
        value |= entropy_get_bit(start_bit + i);
    }

    return value;
}

static void format_word_result_line(uint16_t index, char line[17])
{
    uint8_t position = 0;

    position = append_uint16(line, position, index);

    if (position < 16)
    {
        line[position++] = ' ';
        line[position] = '\0';
    }

    append_text(line, position, wordlist_get(index));
}

static void show_completed_word(uint8_t word_number)
{
    char line1[17];
    char line2[17];
    uint16_t index;

    index = entropy_word_index(word_number);

    format_entropy_word_line(word_number, line1);
    format_word_result_line(index, line2);

    lcd_write_line1(line1);
    lcd_write_line2(line2);

    // explicit pause for manual verification
    input_wait();
}

static void show_newly_completed_words(uint16_t before_count, uint16_t after_count)
{
    uint8_t before_words;
    uint8_t after_words;
    uint8_t word_number;
    uint8_t complete_words;

    complete_words = complete_entropy_words();
    before_words = (uint8_t)(before_count / 11);
    after_words = (uint8_t)(after_count / 11);

    if (before_words > complete_words)
    {
        before_words = complete_words;
    }

    if (after_words > complete_words)
    {
        after_words = complete_words;
    }

    for (word_number = (uint8_t)(before_words + 1);
         word_number <= after_words;
         word_number++)
    {
        show_completed_word(word_number);
    }
}

static void format_feedback_word_line(uint16_t before_count,
                                      uint16_t after_count,
                                      char line[17])
{
    uint8_t before_words;
    uint8_t after_words;
    uint8_t complete_words;

    complete_words = complete_entropy_words();
    before_words = (uint8_t)(before_count / 11);
    after_words = (uint8_t)(after_count / 11);

    if (before_words < complete_words && after_words > before_words)
    {
        format_entropy_word_line((uint8_t)(before_words + 1), line);
    }
    else
    {
        format_current_word_line(line);
    }
}

static void show_input_screen(const char *controls)
{
    char line1[17];

    format_current_word_line(line1);
    lcd_write_line1(line1);
    lcd_write_line2(controls);
}

static void show_von_neumann_feedback(uint8_t first,
                                      uint8_t second,
                                      entropy_vn_result_t result,
                                      uint16_t before_count,
                                      uint16_t after_count)
{
    char line1[17];
    char line2[17];
    uint8_t position = 0;

    format_feedback_word_line(before_count, after_count, line1);

    line2[position++] = first ? 'T' : 'H';
    line2[position++] = second ? 'T' : 'H';
    line2[position++] = ' ';
    line2[position++] = '-';
    line2[position++] = '>';
    line2[position++] = ' ';
    line2[position] = '\0';

    if (result == ENTROPY_VN_DISCARDED)
    {
        append_text(line2, position, "DISCARD");
    }
    else if (result == ENTROPY_VN_ADDED_0)
    {
        append_text(line2, position, "0");
    }
    else if (result == ENTROPY_VN_ADDED_1)
    {
        append_text(line2, position, "1");
    }

    lcd_write_line1(line1);
    lcd_write_line2(line2);
    delay_ms(FEEDBACK_DELAY_MS);
}

static void show_dice_feedback(uint8_t dice,
                               uint16_t before_count,
                               uint16_t after_count)
{
    char line1[17];
    char line2[17];
    uint8_t position = 0;

    format_feedback_word_line(before_count, after_count, line1);

    line2[position++] = 'D';
    line2[position++] = (char)('0' + dice);
    line2[position++] = ' ';
    line2[position++] = '-';
    line2[position++] = '>';
    line2[position++] = ' ';
    line2[position] = '\0';

    if (dice >= 1 && dice <= 4 && after_count == before_count + 1)
    {
        // only the first bit was used; the second bit was discarded
        if (dice <= 2)
        {
            append_text(line2, position, "0 DROP");
        }
        else
        {
            append_text(line2, position, "1 DROP");
        }
    }
    else
    {
        switch (dice)
        {
            case 1:
                append_text(line2, position, "00");
                break;

            case 2:
                append_text(line2, position, "01");
                break;

            case 3:
                append_text(line2, position, "10");
                break;

            case 4:
                append_text(line2, position, "11");
                break;

            default:
                append_text(line2, position, "DISCARD");
                break;
        }
    }

    lcd_write_line1(line1);
    lcd_write_line2(line2);
    delay_ms(FEEDBACK_DELAY_MS);
}

static void format_last_word_final_line(uint16_t index, char line[17])
{
    int8_t bit;
    uint8_t position = 4;
    uint8_t cs_bits;

    cs_bits = checksum_bits();
    format_word_number(line, active_config.word_count);

    // upper bits of the 11-bit index are entropy
    for (bit = 10; bit >= (int8_t)cs_bits; bit--)
    {
        line[position++] = (index & ((uint16_t)1 << bit)) ? '1' : '0';
    }

    line[position++] = '|';

    // lower bits are checksum
    for (bit = (int8_t)cs_bits - 1; bit >= 0; bit--)
    {
        line[position++] = (index & ((uint16_t)1 << bit)) ? '1' : '0';
    }

    line[position] = '\0';
}

static void show_last_word(uint16_t index)
{
    char line1[17];
    char line2[17];

    format_last_word_final_line(index, line1);
    format_word_result_line(index, line2);

    lcd_write_line1(line1);
    lcd_write_line2(line2);

    // explicit pause for manual verification
    input_wait();
}

static void format_mnemonic_line(uint8_t word_number,
                                 uint8_t word_count,
                                 uint16_t index,
                                 char line[17])
{
    uint8_t position = 0;

    line[position++] = (char)('0' + (word_number / 10));
    line[position++] = (char)('0' + (word_number % 10));
    line[position++] = '/';
    line[position++] = (char)('0' + (word_count / 10));
    line[position++] = (char)('0' + (word_count % 10));
    line[position++] = ' ';
    line[position] = '\0';

    append_text(line, position, wordlist_get(index));
}

static void show_mnemonic(const uint16_t indexes[MAX_WORD_COUNT],
                          uint8_t word_count)
{
    uint8_t current = 0;
    char line1[17];

    while (1)
    {
        format_mnemonic_line((uint8_t)(current + 1),
                             word_count,
                             indexes[current],
                             line1);

        lcd_write_line1(line1);
        lcd_write_line2("A=PREV B=NEXT");

        input_wait();

        if (input_get() == INPUT_BUTTON_A)
        {
            if (current > 0)
            {
                current--;
            }
            else
            {
                current = word_count - 1;
            }
        }
        else
        {
            if (current < word_count - 1)
            {
                current++;
            }
            else
            {
                current = 0;
            }
        }
    }
}

static void finish_entropy_input(void)
{
    entropy_get(packed_entropy);

    bip39_encode(packed_entropy,
                 active_config.entropy_bits,
                 word_indexes);

    show_last_word(word_indexes[active_config.word_count - 1]);
    show_mnemonic(word_indexes, active_config.word_count);
}

static void entropy_input_by_von_neumann_coin(void)
{
    char controls[17];
    uint8_t first;
    uint8_t second;
    uint16_t before_count;
    uint16_t after_count;
    entropy_vn_result_t result;

    wait_start("VON NEUMANN COIN");
    entropy_clear();

    while (!entropy_is_full(active_config.entropy_bits))
    {
        show_input_screen("VN1 A:H B:T");

        input_wait();
        first = input_get() == INPUT_BUTTON_A ? 0 : 1;

        result = entropy_add_von_neumann_coin_bit(first);

        if (result != ENTROPY_VN_WAITING)
        {
            // defensive check: the first flip must only start a pair
            entropy_clear();
            return;
        }

        controls[0] = 'V';
        controls[1] = 'N';
        controls[2] = '2';
        controls[3] = ' ';
        controls[4] = first ? 'T' : 'H';
        controls[5] = '?';
        controls[6] = ' ';
        controls[7] = 'A';
        controls[8] = ':';
        controls[9] = 'H';
        controls[10] = ' ';
        controls[11] = 'B';
        controls[12] = ':';
        controls[13] = 'T';
        controls[14] = '\0';

        show_input_screen(controls);

        input_wait();
        second = input_get() == INPUT_BUTTON_A ? 0 : 1;

        before_count = entropy_count();
        result = entropy_add_von_neumann_coin_bit(second);
        after_count = entropy_count();

        show_von_neumann_feedback(first, second, result,
                                  before_count, after_count);
        show_newly_completed_words(before_count, after_count);
    }

    finish_entropy_input();
}

static void entropy_input_by_coin(void)
{
    uint16_t before_count;
    uint16_t after_count;
    int button;

    wait_start("COIN FLIPS");
    entropy_clear();

    while (!entropy_is_full(active_config.entropy_bits))
    {
        show_input_screen("A:H=0 B:T=1");

        input_wait();
        button = input_get();

        before_count = entropy_count();
        entropy_add_coin_bit(button == INPUT_BUTTON_A ? 0 : 1);
        after_count = entropy_count();

        show_newly_completed_words(before_count, after_count);
    }

    finish_entropy_input();
}

static void entropy_input_by_dice_rolls(void)
{
    uint8_t dice = 0;
    char controls[17];
    uint16_t before_count;
    uint16_t after_count;

    wait_start("DICE ROLLS");
    entropy_clear();

    while (!entropy_is_full(active_config.entropy_bits))
    {
        controls[0] = 'D';
        controls[1] = ':';
        controls[2] = dice == 0 ? '?' : (char)('0' + dice);
        controls[3] = ' ';
        controls[4] = 'A';
        controls[5] = ':';
        controls[6] = 'O';
        controls[7] = 'K';
        controls[8] = ' ';
        controls[9] = 'B';
        controls[10] = ':';
        controls[11] = '+';
        controls[12] = '\0';

        show_input_screen(controls);
        input_wait();

        if (input_get() == INPUT_BUTTON_B)
        {
            if (dice == 0 || dice == 6)
            {
                dice = 1;
            }
            else
            {
                dice++;
            }
        }
        else if (dice != 0)
        {
            before_count = entropy_count();
            entropy_add_dice_roll(dice, active_config.entropy_bits);
            after_count = entropy_count();

            show_dice_feedback(dice, before_count, after_count);
            show_newly_completed_words(before_count, after_count);

            dice = 0;
        }
    }

    finish_entropy_input();
}

void entropy_input_run(void)
{
    entropy_source_t current = ENTROPY_COIN;

    select_mnemonic_length();

    while (1)
    {
        lcd_write_line1(entropy_sources[current]);
        lcd_write_line2("A=OK  B=NEXT");

        input_wait();

        if (input_get() == INPUT_BUTTON_B)
        {
            current++;

            if (current >= ENTROPY_SOURCE_COUNT)
            {
                current = ENTROPY_COIN;
            }
        }
        else
        {
            switch (current)
            {
                case ENTROPY_COIN:
                    entropy_input_by_coin();
                    return;

                case ENTROPY_VON_NEUMANN_COIN:
                    entropy_input_by_von_neumann_coin();
                    return;

                case ENTROPY_DICE_ROLLS:
                    entropy_input_by_dice_rolls();
                    return;
            }
        }
    }
}
