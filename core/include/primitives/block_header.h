/*
 * This file belongs to Brown University's computer
 * science department as part of csci1951L - Blockchains
 * and cryptocurrencies.
 *
 * This software was developed by Colby Anderson (HTA)
 * and Parker Ljung (TA) of csci1951L in Summer of 2021.
 */

#ifndef RATH_BLOCK_HEADER_H
#define RATH_BLOCK_HEADER_H

#include <rath.pb.h>

class BlockHeader {
public:
    uint8_t version;
    uint32_t previous_block_hash;
    uint32_t merkle_root;
    uint32_t difficulty_target;
    uint32_t nonce;
    uint32_t timestamp;

    BlockHeader(uint8_t version=0,
                uint32_t previous_block_hash=0,
                uint32_t merkle_root=0,
                uint32_t difficulty_target=0,
                uint32_t nonce=0,
                uint32_t timestamp=0);
};


#endif //RATH_BLOCK_HEADER_H
