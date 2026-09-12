/*
*******************************************************************************
 Tests

 Unit tests
*******************************************************************************
*/

#include "tests.h"

#include <stdio.h>
#include <string.h>

#include "bip39.h"
#include "wordlist.h"
#include "entropy.h"

typedef struct
{
    const char *name;
    uint8_t entropy[32];
    const char *mnemonic[24];

} bip39_test_t;

// test vectors from the official Trezor BIP-39 vectors.json
static const bip39_test_t bip39_tests[] =
{
    {
        "Test 1",
        {
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        },
        {
            "abandon","abandon","abandon","abandon",
            "abandon","abandon","abandon","abandon",
            "abandon","abandon","abandon","abandon",
            "abandon","abandon","abandon","abandon",
            "abandon","abandon","abandon","abandon",
            "abandon","abandon","abandon","art"
        }
    },
    {
        "Test 2",
        {
            0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,
            0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,
            0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,
            0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f
        },
        {
            "legal","winner","thank","year",
            "wave","sausage","worth","useful",
            "legal","winner","thank","year",
            "wave","sausage","worth","useful",
            "legal","winner","thank","year",
            "wave","sausage","worth","title"
        }
    },
    {
        "Test 3",
        {
            0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
            0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
            0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
            0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80
        },
        {
            "letter","advice","cage","absurd",
            "amount","doctor","acoustic","avoid",
            "letter","advice","cage","absurd",
            "amount","doctor","acoustic","avoid",
            "letter","advice","cage","absurd",
            "amount","doctor","acoustic","bless"
        }
    },
    {
        "Test 4",
        {
            0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
            0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
            0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
            0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
        },
        {
            "zoo","zoo","zoo","zoo",
            "zoo","zoo","zoo","zoo",
            "zoo","zoo","zoo","zoo",
            "zoo","zoo","zoo","zoo",
            "zoo","zoo","zoo","zoo",
            "zoo","zoo","zoo","vote"
        }
    },
    {
        "Test 5",
        {
            0x68,0xa7,0x9e,0xac,0xa2,0x32,0x48,0x73,
            0xea,0xcc,0x50,0xcb,0x9c,0x6e,0xca,0x8c,
            0xc6,0x8e,0xa5,0xd9,0x36,0xf9,0x87,0x87,
            0xc6,0x0c,0x7e,0xbc,0x74,0xe6,0xce,0x7c
        },
        {
            "hamster","diagram","private","dutch",
            "cause","delay","private","meat",
            "slide","toddler","razor","book",
            "happy","fancy","gospel","tennis",
            "maple","dilemma","loan","word",
            "shrug","inflict","delay","length"
        }
    },
    {
        "Test 6",
        {
            0x9f,0x6a,0x28,0x78,0xb2,0x52,0x07,0x99,
            0xa4,0x4e,0xf1,0x8b,0xc7,0xdf,0x39,0x4e,
            0x70,0x61,0xa2,0x24,0xd2,0xc3,0x3c,0xd0,
            0x15,0xb1,0x57,0xd7,0x46,0x86,0x98,0x63
        },
        {
            "panda","eyebrow","bullet","gorilla",
            "call","smoke","muffin","taste",
            "mesh","discover","soft","ostrich",
            "alcohol","speed","nation","flash",
            "devote","level","hobby","quick",
            "inner","drive","ghost","inside"
        }
    },
    {
        "Test 7",
        {
            0x06,0x6d,0xca,0x1a,0x2b,0xb7,0xe8,0xa1,
            0xdb,0x28,0x32,0x14,0x8c,0xe9,0x93,0x3e,
            0xea,0x0f,0x3a,0xc9,0x54,0x8d,0x79,0x31,
            0x12,0xd9,0xa9,0x5c,0x94,0x07,0xef,0xad
        },
        {
            "all","hour","make","first",
            "leader","extend","hole","alien",
            "behind","guard","gospel","lava",
            "path","output","census","museum",
            "junior","mass","reopen","famous",
            "sing","advance","salt","reform"
        }
    },
    {
        "Test 8",
        {
            0xf5,0x85,0xc1,0x1a,0xec,0x52,0x0d,0xb5,
            0x7d,0xd3,0x53,0xc6,0x95,0x54,0xb2,0x1a,
            0x89,0xb2,0x0f,0xb0,0x65,0x09,0x66,0xfa,
            0x0a,0x9d,0x6f,0x74,0xfd,0x98,0x9d,0x8f
        },
        {
            "void","come","effort","suffer",
            "camp","survey","warrior","heavy",
            "shoot","primary","clutch","crush",
            "open","amazing","screen","patrol",
            "group","space","point","ten",
            "exist","slush","involve","unfold"
        }
    },
};

static uint8_t test_entropy_clear(void)
{
    entropy_clear();

    if (entropy_count() != 0)
        return 0;

    if (entropy_is_full(256))
        return 0;

    return 1;
}

static uint8_t test_entropy_coin(void)
{
    entropy_clear();

    entropy_add_coin_bit(0);
    entropy_add_coin_bit(1);
    entropy_add_coin_bit(1);

    return entropy_count() == 3;
}

static uint8_t test_entropy_get(void)
{
    uint8_t buffer[32];

    entropy_clear();

    entropy_add_coin_bit(1);
    entropy_add_coin_bit(0);
    entropy_add_coin_bit(1);

    if (entropy_get_bit(0) != 1)
        return 0;

    if (entropy_get_bit(1) != 0)
        return 0;

    if (entropy_get_bit(2) != 1)
        return 0;

    // index 3 has not been stored yet
    if (entropy_get_bit(3) != 0)
        return 0;

    entropy_get(buffer);

    if (buffer[0] != 0xA0)
        return 0;

    return 1;
}

static uint8_t test_entropy_von_neumann_coin(void)
{
    uint8_t entropy[32];

    entropy_clear();

    // 00 -> discard
    if (entropy_add_von_neumann_coin_bit(0) != ENTROPY_VN_WAITING)
        return 0;

    if (entropy_add_von_neumann_coin_bit(0) != ENTROPY_VN_DISCARDED)
        return 0;

    if (entropy_count() != 0)
        return 0;

    // 11 -> discard
    if (entropy_add_von_neumann_coin_bit(1) != ENTROPY_VN_WAITING)
        return 0;

    if (entropy_add_von_neumann_coin_bit(1) != ENTROPY_VN_DISCARDED)
        return 0;

    if (entropy_count() != 0)
        return 0;

    // 01 -> 0
    if (entropy_add_von_neumann_coin_bit(0) != ENTROPY_VN_WAITING)
        return 0;

    if (entropy_add_von_neumann_coin_bit(1) != ENTROPY_VN_ADDED_0)
        return 0;

    if (entropy_count() != 1)
        return 0;

    // 10 -> 1
    if (entropy_add_von_neumann_coin_bit(1) != ENTROPY_VN_WAITING)
        return 0;

    if (entropy_add_von_neumann_coin_bit(0) != ENTROPY_VN_ADDED_1)
        return 0;

    if (entropy_count() != 2)
        return 0;

    entropy_get(entropy);

    // produced bits are: 0 1
    if (entropy[0] != 0x40)
        return 0;

    return 1;
}

static uint8_t test_entropy_dice(void)
{
    uint8_t entropy[32];
    uint16_t i;

    entropy_clear();

    entropy_add_dice_roll(1, 256);   // 00
    if (entropy_count() != 2)
        return 0;

    entropy_add_dice_roll(2, 256);   // 01
    if (entropy_count() != 4)
        return 0;

    entropy_add_dice_roll(3, 256);   // 10
    if (entropy_count() != 6)
        return 0;

    entropy_add_dice_roll(4, 256);   // 11
    if (entropy_count() != 8)
        return 0;

    entropy_add_dice_roll(5, 256);   // discard
    entropy_add_dice_roll(6, 256);   // discard

    if (entropy_count() != 8)
        return 0;

    entropy_get(entropy);

    /* 00 01 10 11 = 00011011 = 0x1B */
    if (entropy[0] != 0x1B)
        return 0;

    /*
     * At an 11-bit word boundary only the first bit of the
     * accepted dice encoding is used. The second is dropped.
     */
    entropy_clear();

    for (i = 0; i < 10; i++)
    {
        entropy_add_coin_bit(0);
    }

    entropy_add_dice_roll(4, 256);   // 11 -> use first 1, drop second 1

    if (entropy_count() != 11)
        return 0;

    if (entropy_get_bit(10) != 1)
        return 0;

    entropy_add_dice_roll(2, 256);   // next word starts with 01

    if (entropy_count() != 13)
        return 0;

    if (entropy_get_bit(11) != 0 || entropy_get_bit(12) != 1)
        return 0;

    /*
     * The same rule applies at the selected entropy limit.
     * For 12 words, 128 bits must be collected exactly.
     */
    entropy_clear();

    for (i = 0; i < 127; i++)
    {
        entropy_add_coin_bit(0);
    }

    entropy_add_dice_roll(4, 128);   // use first 1, drop second 1

    if (entropy_count() != 128)
        return 0;

    if (entropy_get_bit(127) != 1)
        return 0;

    return 1;
}

static uint8_t test_entropy_lengths(void)
{
    uint16_t i;

    entropy_clear();

    for (i = 0; i < 128; i++)
    {
        entropy_add_coin_bit(0);
    }

    if (!entropy_is_full(128))
        return 0;

    if (entropy_is_full(192))
        return 0;

    for (; i < 192; i++)
    {
        entropy_add_coin_bit(0);
    }

    if (!entropy_is_full(192))
        return 0;

    if (entropy_is_full(256))
        return 0;

    for (; i < 256; i++)
    {
        entropy_add_coin_bit(0);
    }

    if (!entropy_is_full(256))
        return 0;

    return 1;
}

static uint8_t test_entropy(void)
{
    if (!test_entropy_clear())        return 0;
    if (!test_entropy_coin())         return 0;
    if (!test_entropy_get())          return 0;
    if (!test_entropy_von_neumann_coin()) return 0;
    if (!test_entropy_dice())             return 0;
    if (!test_entropy_lengths())          return 0;
    return 1;
}


static uint8_t test_bip39_12_words(void)
{
    uint8_t entropy[32];
    uint16_t words[24];
    uint8_t i;

    memset(entropy, 0, sizeof(entropy));

    /* Bytes outside the selected 128-bit entropy must be ignored. */
    memset(&entropy[16], 0xA5, 16);

    bip39_encode(entropy, 128, words);

    for (i = 0; i < 11; i++)
    {
        if (strcmp(wordlist_get(words[i]), "abandon") != 0)
            return 0;
    }

    if (strcmp(wordlist_get(words[11]), "about") != 0)
        return 0;

    return 1;
}

static uint8_t test_bip39_18_words(void)
{
    uint8_t entropy[32];
    uint16_t words[24];
    uint8_t i;

    memset(entropy, 0, sizeof(entropy));

    /* Bytes outside the selected 192-bit entropy must be ignored. */
    memset(&entropy[24], 0xA5, 8);

    bip39_encode(entropy, 192, words);

    for (i = 0; i < 17; i++)
    {
        if (strcmp(wordlist_get(words[i]), "abandon") != 0)
            return 0;
    }

    if (strcmp(wordlist_get(words[17]), "agent") != 0)
        return 0;

    return 1;
}

static uint8_t test_bip39(void)
{
    uint16_t words[24];

    unsigned int t;
    int i;

    for (t = 0; t < sizeof(bip39_tests) / sizeof(bip39_tests[0]); t++)
    {
        bip39_encode(bip39_tests[t].entropy, 256, words);

        for (i = 0; i < 24; i++)
        {
            if (strcmp(wordlist_get(words[i]),
                       bip39_tests[t].mnemonic[i]) != 0)
            {
                return 0;
            }
        }
    }

    return 1;
}

uint8_t test_run(void)
{
    if (!test_bip39_12_words()) return 0;
    if (!test_bip39_18_words()) return 0;
    if (!test_bip39())          return 0;
    if (!test_entropy())        return 0;

    return 1;
}
