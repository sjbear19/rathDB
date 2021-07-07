/*
 * This file belongs to Brown University's computer
 * science department as part of csci1951L - Blockchains
 * and cryptocurrencies.
 *
 * This software was developed by Colby Anderson (HTA)
 * and Parker Ljung (TA) of csci1951L in Summer of 2021.
 */

#include <coin_database.h>
#include <rathcrypto.h>

CoinLocator::CoinLocator(
        uint32_t transaction_hash_, uint8_t output_index_)
        : transaction_hash(transaction_hash_),
          output_index(output_index_) {}

std::string CoinLocator::serialize(const CoinLocator& coin_locator) {
    return std::to_string(coin_locator.transaction_hash) + "-" +
           std::to_string(coin_locator.output_index);
}

std::unique_ptr<CoinLocator> CoinLocator::deserialize(const std::string& serialized_coin_locator) {
    std::string transaction_hash = serialized_coin_locator.substr(0, serialized_coin_locator.find("-"));
    std::string output_index = serialized_coin_locator.substr(serialized_coin_locator.find("-") + 1, serialized_coin_locator.size());
    return std::make_unique<CoinLocator>(std::stoul (transaction_hash,nullptr,0), std::stoul (output_index,nullptr,0));
}

std::string CoinLocator::serialize_from_construct(uint32_t transaction_hash, uint8_t output_index) {
    return std::to_string(transaction_hash) + "-" +
           std::to_string(output_index);
}

bool CoinDatabase::validate_block(std::vector<std::shared_ptr<Transaction>> transactions) {
  for int i = 0; i < transactions.size(); i++ {
    if !(validate_transaction(transactions[i])) {
      return false;
    }
  }
  return true;
}
bool CoinDatabase::validate_transaction(std::shared_ptr<Transaction> transaction) {
  std::vector<std::unique_ptr<TransactionInput>> v = transaction.transaction_inputs;
  for int i = 0; i < v.size(); i++ {
    std::string s = deserialize(v[i]);
    if _main_cache.find(s) == nullptr {
      if _database.find(s) == nullptr {
        return false;
      }
    }
  }
  return true;
}
void CoinDatabase::store_block(std::vector<std::shared_ptr<Transaction>> transactions) {}
void CoinDatabase::store_transaction(std::shared_ptr<Transaction> transaction) {}
bool CoinDatabase::validate_and_store_block(std::vector<std::shared_ptr<Transaction>> transactions) {
  if validate_block(transactions) {
    store_block(transactions);
    return true;
  }
  return false;
}
bool CoinDatabase::validate_and_store_transaction(std::shared_ptr<Transaction> transaction) {
  if validate_transaction(transaction) {
    store_transaction(transaction);
    return true;
  }
  return false;  
}
void CoinDatabase::remove_transactions_from_mempool(std::vector<std::shared_ptr<Transaction>> transactions) {}
void CoinDatabase::store_transactions_to_main_cache(std::vector<std::shared_ptr<Transaction>> transactions) {
  
}
void CoinDatabase::store_transaction_in_mempool(std::shared_ptr<Transaction> transaction) {}
void CoinDatabase::undo_coins(std::vector<std::shared_ptr<UndoBlock>> undo_blocks) {}
void CoinDatabase::flush_main_cache() {}
