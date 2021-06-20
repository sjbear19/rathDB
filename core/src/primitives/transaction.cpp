/*
 * This file belongs to Brown University's computer
 * science department as part of csci1951L - Blockchains
 * and cryptocurrencies.
 *
 * This software was developed by Colby Anderson (HTA)
 * and Parker Ljung (TA) of csci1951L in Summer of 2021.
 */

#include <transaction.h>

Transaction::Transaction(
        std::vector<std::shared_ptr<TransactionInput>> transaction_inputs_,
        std::vector<std::shared_ptr<TransactionOutput>> transaction_outputs_, uint8_t version,
        uint32_t lock_time) : transaction_inputs(transaction_inputs_),
                              transaction_outputs(transaction_outputs_), version(version),
                              lock_time(lock_time) {}

bool Transaction::is_coin_base() {
    return (transaction_inputs.size() == 1 && transaction_inputs[0]->reference_transaction_hash == 0);
}
