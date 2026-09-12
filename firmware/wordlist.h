/*
*******************************************************************************
 Wordlist

 Api to get a BIP39 word by its index (e.g. 0 -> abandon)
*******************************************************************************
*/

#ifndef WORDLIST_H
#define WORDLIST_H

#include <stdint.h>

const char *wordlist_get(uint16_t index);

#endif
