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

std::unique_ptr<CoinRecord> CoinDatabase::get_coin_record(std::string hash) {
    return CoinRecord::deserialize(_database->get_safely(hash));
}

std::unique_ptr<TransactionOutput> CoinDatabase::get_tx_output(uint32_t tx_hash, uint32_t utxo_i) {
    std::string locator = CoinLocator::serialize_from_construct(tx_hash, utxo_i);
    std::unique_ptr<TransactionOutput> transaction_output = std::make_unique<TransactionOutput>(*_main_cache.at(locator)->transaction_output);
    return transaction_output;
}

CoinDatabase::CoinDatabase() : _main_cache_capacity(30), _mempool_capacity(50) {
    printf("in coindatabase constructor");
    _database = std::make_unique<Database>();
    _main_cache_size = 0;
    _mempool_size = 0;
}

bool CoinDatabase::validate_block(std::vector<std::unique_ptr<Transaction>>& transactions) {
    printf("in validate_block");
  for (int i = 0; i < transactions.size(); i++) {
    if (!(validate_transaction(*transactions[i]))) {
      return false;
    }
  }
  return true;
}
bool CoinDatabase::validate_transaction(const Transaction& transaction) {
    printf("in validate_block");
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
    printf("in store_block");
    remove_transactions_from_mempool(transactions);
    store_transactions_to_main_cache(std::move(transactions));
}
void CoinDatabase::store_transaction(std::unique_ptr<Transaction> transaction) {
    printf("in store_transaction");
    store_transaction_in_mempool(std::move(transaction));
}
bool CoinDatabase::validate_and_store_block(std::vector<std::unique_ptr<Transaction>> transactions) {
    printf("in validate_and_store_block");
  if (validate_block(transactions)) {
    store_block(std::move(transactions));
    return true;
  }
  return false;
}
bool CoinDatabase::validate_and_store_transaction(std::unique_ptr<Transaction> transaction) {
    printf("in validate_and_store_transaction");
  if (validate_transaction(*transaction)) {
    store_transaction(std::move(transaction));
    return true;
  }
  return false;  
}
void CoinDatabase::remove_transactions_from_mempool(const std::vector<std::unique_ptr<Transaction>>& transactions) {
    printf("in remove_transactions_from_mempool");
    for (int i = 0; i < transactions.size(); i++) {
        uint32_t hash = transactions[i]->transaction_inputs[0]->reference_transaction_hash;
        _mempool_cache.erase(hash);
        _mempool_size--;
    }
}
void CoinDatabase::store_transactions_to_main_cache(std::vector<std::unique_ptr<Transaction>> transactions) {
    printf("in store_transactions_to_main_cache");
    /*This stores transactions from a validated block to the main_cache. This should remove any spent TXO from
     the _main_cache. If the UTXO can not be found in the main_cache then it is removed from the _database. Note just
     in these few steps, there are many kinds of optimizations that can be done.

     Next, we must store the new UTXO. To do this, we create a coin_record for each transaction and store it to the database.
     For each transaction, we also add the UTXO to the main_cache. If at some point during the process it becomes full,
     then it should be flushed by calling flush_main_cache.
*/
    /*for (int i = 0; i < transactions.size(); i++) {
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
*//*            this->_main_cache.erase(serial); // is this how this works?
            this->_database->delete_safely(serial);
            utxos.push_back(txHash);*//*
            amounts.push_back(transactions[i]->transaction_outputs[txo]->amount);
            public_keys.push_back(transactions[i]->transaction_outputs[txo]->public_key);
        }
        //remove txo from database
        //create coin_record
        std::unique_ptr<CoinRecord> cr = std::make_unique<CoinRecord>(transactions[0]->version, utxos, amounts, public_keys);
        //_database->put_safely(1, 1);


        //_main_cache.insert(std::make_pair(1, 1));
    }*/

    for (int i = 0; i < transactions.size(); i++) {
        uint32_t txHash = RathCrypto::hash(Transaction::serialize(*transactions[i]));

        if (this->_main_cache_size >= this->_main_cache_capacity) {
            printf("flushing cache");
            this->flush_main_cache();
        }

        // mark any inputs marked in the main cache as spent
        // coin is an input
        // adding to map: []
        for (int k = 0; k < transactions[i]->transaction_inputs.size(); k++) {
            std::string serialCL = CoinLocator::serialize_from_construct(transactions[i]->transaction_inputs[k]->reference_transaction_hash,
                                                                         transactions[i]->transaction_inputs[k]->utxo_index);
            if (this->_main_cache.contains(serialCL)) {
                _main_cache[serialCL]->is_spent = true;
            } else {
                std::string serialCR = _database->get_safely(std::to_string(transactions[i]->transaction_inputs[k]->reference_transaction_hash));
                std::unique_ptr<CoinRecord> realCR = CoinRecord::deserialize(serialCR);
                auto position = std::find(realCR->utxo.cbegin(), realCR->utxo.cend(),transactions[i]->transaction_inputs[k]->utxo_index);
                if (position != realCR->utxo.cend()) {
                    uint32_t actualIndex = position - realCR->utxo.cbegin();
                    realCR->utxo.erase(realCR->utxo.cbegin() + actualIndex);
                    realCR->amounts.erase(realCR->utxo.cbegin() + actualIndex);
                    realCR->public_keys.erase(realCR->utxo.cbegin() + actualIndex);
                }
                _database->put_safely(std::to_string(transactions[i]->transaction_inputs[k]->reference_transaction_hash),
                                      CoinRecord::serialize(*realCR));
            }
        }

        std::vector<uint32_t> utxos;
        std::vector<uint32_t> amts;
        std::vector<uint32_t> pubKeys;

        for (int j = 0; j < transactions[i]->transaction_outputs.size(); j++) {
            utxos.push_back(j);
            amts.push_back(transactions[i]->transaction_outputs[j]->amount);
            pubKeys.push_back(transactions[i]->transaction_outputs[j]->public_key);

            std::string newCL = CoinLocator::serialize_from_construct(txHash, j);
            _main_cache[newCL] = std::make_unique<Coin>(std::move(transactions[i]->transaction_outputs[j]), false);
            _main_cache_size++;
        }
        std::unique_ptr<CoinRecord> newCR = std::make_unique<CoinRecord>(transactions[i]->version,
                                                                         utxos,
                                                                         amts,
                                                                         pubKeys);
        this->_database->put_safely(std::to_string(txHash), CoinRecord::serialize(*newCR));
    }
}
void CoinDatabase::store_transaction_in_mempool(std::unique_ptr<Transaction> transaction) {
    printf("in store_transaction_in_mempool");
    if (_mempool_capacity >= _mempool_size) {
        uint32_t hash = transaction->transaction_inputs[0]->reference_transaction_hash;
        _mempool_cache.insert(std::make_pair(hash, std::move(transaction)));
        _mempool_size++;

    }
}
void CoinDatabase::undo_blocks(std::vector<std::unique_ptr<Block>> blocks) {
    for (const std::unique_ptr<Block> & blk : blocks) {
        for (int i = 0; i < blk->transactions.size(); i++) {
            if (_database->get_safely(Transaction::serialize(*blk->transactions[i])) != "") {
                _database->delete_safely(Transaction::serialize(*blk->transactions[i]));
            }
            for (const std::unique_ptr<TransactionOutput> & txo : blk->transactions[i]->transaction_outputs) {
                std::string newCL = CoinLocator::serialize_from_construct(RathCrypto::hash(Transaction::serialize(*blk->transactions[i])),i);
                _main_cache.erase(newCL);
            }
        }
    }
}

void CoinDatabase::undo_coins(std::vector<std::unique_ptr<UndoBlock>> undo_blocks) {
    for (int i = 0; i < undo_blocks.size(); i++) {
        for (int j = 0; j < undo_blocks[i]->undo_coin_records.size(); i++) {
//            std::unique_ptr<UndoCoinRecord> newUCR = undo_blocks[i]->undo_coin_records[j]; // TODO: ask why this does not work
            CoinLocator newCL = CoinLocator(undo_blocks[i]->transaction_hashes[j],
                                            undo_blocks[i]->undo_coin_records[j]->utxo[0]);
            if (_main_cache.contains(CoinLocator::serialize(newCL))) {
                std::unique_ptr<Coin> newCoin = std::move(_main_cache.find(CoinLocator::serialize(newCL))->second);
                if (newCoin->is_spent) {
                    newCoin->is_spent = false;
                }
            } else if (_database->get_safely(std::to_string(newCL.transaction_hash)) != "") {
                // check if utxo is missing
                // if it is add it back
                auto db_coin_rec = CoinRecord::deserialize(
                        _database->get_safely(std::to_string(newCL.transaction_hash)));
                if (db_coin_rec->utxo.empty()) {
                    db_coin_rec->utxo = undo_blocks[i]->undo_coin_records[j]->utxo;
                }
            } else {
                std::unique_ptr<CoinRecord> newCR = std::make_unique<CoinRecord>(
                        undo_blocks[i]->undo_coin_records[j]->version,
                        undo_blocks[i]->undo_coin_records[j]->utxo,
                        undo_blocks[i]->undo_coin_records[j]->amounts,
                        undo_blocks[i]->undo_coin_records[j]->public_keys);
                _database->put_safely(std::to_string(undo_blocks[i]->transaction_hashes[j]),
                                      CoinRecord::serialize(*newCR));
            }
        }
    }
}
void CoinDatabase::flush_main_cache() {
    printf("in flush_main_cache");
    for (auto it = _main_cache.begin(); it != _main_cache.end(); it++) {
        if (it->second->is_spent) {
            _database->delete_safely(it->first);
        }
        _main_cache.erase(it->first);
    }
}
