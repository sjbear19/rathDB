/*
 * This file belongs to Brown University's computer
 * science department as part of csci1951L - Blockchains
 * and cryptocurrencies.
 *
 * This software was developed by Colby Anderson (HTA)
 * and Parker Ljung (TA) of csci1951L in Summer of 2021.
 */

#include <transaction_output.h>
#include <rath.pb.h>

TransactionOutput::TransactionOutput(uint32_t amount, uint32_t public_key) :
        amount(amount), public_key(public_key) {}

std::string TransactionOutput::serialize(const TransactionOutput& transaction_output) {
    PTransactionOutput protobuf_transaction_output = PTransactionOutput();
    protobuf_transaction_output.set_amount(transaction_output.amount);
    protobuf_transaction_output.set_public_key(transaction_output.public_key);
    std::string serialized_transaction_output = protobuf_transaction_output.SerializeAsString();
    return serialized_transaction_output;
}

TransactionOutput TransactionOutput::deserialize(std::string serialized_transaction_output) {
    PTransactionOutput protobuf_transaction_output = PTransactionOutput();
    protobuf_transaction_output.ParseFromString(serialized_transaction_output);
    TransactionOutput transaction_output = TransactionOutput(protobuf_transaction_output.amount(),
                                                             protobuf_transaction_output.public_key());
    return transaction_output;
}
