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

CoinLocator::CoinLocator(uint32_t transaction_hash_, uint8_t output_index_)
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

CoinDatabase::CoinDatabase() : _main_cache_capacity(30), _mempool_capacity(50) {
    _database = std::make_unique<Database>();
    _main_cache_size = 0;
    _mempool_size = 0;
}

bool CoinDatabase::validate_block(std::vector<std::unique_ptr<Transaction>>& transactions) {
  for (int i = 0; i < transactions.size(); i++) {
    if (!(validate_transaction(*transactions[i]))) {
      return false;
    }
  }
  return true;
}
bool CoinDatabase::validate_transaction(const Transaction& transaction) {
  for (int i = 0; i < transaction.transaction_inputs.size(); i++) {
      std::string s = TransactionInput::serialize(*transaction.transaction_inputs[i]);
      if (_main_cache.find(s) == _main_cache.end()) {
          if (_database->get_safely(s) == "0") {
              return false;
          }
      }
  }
  return true;
}
void CoinDatabase::store_block(std::vector<std::unique_ptr<Transaction>> transactions) {
    remove_transactions_from_mempool(transactions);
    store_transactions_to_main_cache(std::move(transactions));
}
void CoinDatabase::store_transaction(std::unique_ptr<Transaction> transaction) {
    store_transaction_in_mempool(std::move(transaction));
}
bool CoinDatabase::validate_and_store_block(std::vector<std::unique_ptr<Transaction>> transactions) {

  if (validate_block(transactions)) {
    store_block(std::move(transactions));
    return true;
  }
  return false;
}
bool CoinDatabase::validate_and_store_transaction(std::unique_ptr<Transaction> transaction) {
  if (validate_transaction(*transaction)) {
    store_transaction(std::move(transaction));
    return true;
  }
  return false;  
}
void CoinDatabase::remove_transactions_from_mempool(const std::vector<std::unique_ptr<Transaction>>& transactions) {
    for (int i = 0; i < transactions.size(); i++) {
        uint32_t hash = transactions[i]->transaction_inputs[0]->reference_transaction_hash;
        _mempool_cache.erase(hash);
        _mempool_size--;
    }
}
void CoinDatabase::store_transactions_to_main_cache(std::vector<std::unique_ptr<Transaction>> transactions) {
    /*This stores transactions from a validated block to the main_cache. This should remove any spent TXO from
     the _main_cache. If the UTXO can not be found in the main_cache then it is removed from the _database. Note just
     in these few steps, there are many kinds of optimizations that can be done.

     Next, we must store the new UTXO. To do this, we create a coin_record for each transaction and store it to the database.
     For each transaction, we also add the UTXO to the main_cache. If at some point during the process it becomes full,
     then it should be flushed by calling flush_main_cache.
*/
    for (int i = 0; i < transactions.size(); i++) {
        std::string serial = Transaction::serialize(*transactions[i]);
        uint32_t tx_hash = RathCrypto::hash(serial);

        std::vector<uint32_t> utxos;
        std::vector<uint32_t> amounts;
        std::vector<uint32_t> public_keys;
        //remove txo from cache
        if (_main_cache_size >= _main_cache_capacity) {
            flush_main_cache();
        }

        for (int txi = 0; txi < transactions[i]->transaction_inputs.size(); txi++) {
            int utxo_index = transactions[i]->transaction_inputs[txi]->utxo_index;
            if (_main_cache.contains(std::to_string(utxo_index))) {
                _main_cache.erase(std::to_string(utxo_index));
            } else {
                _database->delete_safely(serial);
                utxos.push_back(utxo_index);
            }
        }
        for (int txo = 0; txo < transactions[i]->transaction_outputs.size(); txo++) {
/*            this->_main_cache.erase(serial); // is this how this works?
            this->_database->delete_safely(serial);
            utxos.push_back(txHash);*/
            amounts.push_back(transactions[i]->transaction_outputs[txo]->amount);
            public_keys.push_back(transactions[i]->transaction_outputs[txo]->public_key);
        }
        //remove txo from database
        //create coin_record
        std::unique_ptr<CoinRecord> cr = std::make_unique<CoinRecord>(transactions[0]->version, utxos, amounts, public_keys);
        _database->put_safely(       );


        _main_cache.insert(std::make_pair(       ));
    }
}
void CoinDatabase::store_transaction_in_mempool(std::unique_ptr<Transaction> transaction) {
    if (_mempool_capacity >= _mempool_size) {
        uint32_t hash = transaction->transaction_inputs[0]->reference_transaction_hash;
        _mempool_cache.insert(std::make_pair(hash, std::move(transaction)));
        _mempool_size++;

    }
}
void CoinDatabase::undo_coins(std::vector<std::unique_ptr<UndoBlock>> undo_blocks) {


}
void CoinDatabase::flush_main_cache() {
    for (auto it = _main_cache.begin(); it != _main_cache.end(); it++) {
        if (it->second->is_spent) {
            _database->delete_safely(it->first);
        }
        _main_cache.erase(it->first);
    }
}
