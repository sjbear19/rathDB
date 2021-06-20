//
// Created by Colby Anderson on 6/5/21.
//

#ifndef RATH_TRANSACTION_H
#define RATH_TRANSACTION_H

#include <transaction_input.h>
#include <transaction_output.h>
#include <vector>

class Transaction {
public:
    uint8_t version;
    std::vector<std::shared_ptr<TransactionInput>> transaction_inputs;
    std::vector<std::shared_ptr<TransactionOutput>> transaction_outputs;
    uint32_t lock_time;

    Transaction(std::vector<std::shared_ptr<TransactionInput>> transaction_inputs_, std::vector<std::shared_ptr<TransactionOutput>> transaction_outputs_, uint8_t version=0, uint32_t lock_time=0);

    bool is_coin_base();
};

#endif //RATH_TRANSACTION_H
