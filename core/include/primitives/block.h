/*
 * This file belongs to Brown University's computer
 * science department as part of csci1951L - Blockchains
 * and cryptocurrencies.
 *
 * This software was developed by Colby Anderson (HTA)
 * and Parker Ljung (TA) of csci1951L in Summer of 2021.
 */

#ifndef RATH_BLOCK_H
#define RATH_BLOCK_H

#include <transaction.h>
#include <block_header.h>
#include <vector>
#include <rath.pb.h>
#include <string>

class Block {
public:
    std::shared_ptr<BlockHeader> block_header;
    std::vector<std::shared_ptr<Transaction>> transactions;

    Block(std::shared_ptr<BlockHeader> block_header_, std::vector<std::shared_ptr<Transaction>> transactions_);
    Block();

    static std::string serialize(std::shared_ptr<Block> block);
    static std::shared_ptr<Block> deserialize(std::string serialized_block);
};

#endif //RATH_BLOCK_H
