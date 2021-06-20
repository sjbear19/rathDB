/*
 * This file belongs to Brown University's computer
 * science department as part of csci1951L - Blockchains
 * and cryptocurrencies.
 *
 * This software was developed by Colby Anderson (HTA)
 * and Parker Ljung (TA) of csci1951L in Summer of 2021.
 */

#ifndef RATHDB_STENCIL_COIN_H
#define RATHDB_STENCIL_COIN_H

#include <transaction_output.h>

class Coin {
public:
    /// Unspent TransactionOutput
    std::unique_ptr<TransactionOutput> transaction_output;
    /// Whether or not this TransactionOutput's containing Transaction is a coinbase
    bool is_coin_base;
    /// height of containing Transaction's Block
    uint32_t height;

    Coin(std::unique_ptr<TransactionOutput> transaction_output_, bool is_coin_base_, uint32_t height_);
};

#endif //RATHDB_STENCIL_COIN_H
