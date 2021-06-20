/*
 * This file belongs to Brown University's computer
 * science department as part of csci1951L - Blockchains
 * and cryptocurrencies.
 *
 * This software was developed by Colby Anderson (HTA)
 * and Parker Ljung (TA) of csci1951L in Summer of 2021.
 */

#include <undo_block.h>

/// Serializes an UndoBlock. Necessary to write UndoBlocks to an
/// "undo" file
std::string UndoBlock::serialize(std::shared_ptr<UndoBlock> undo_block) {
    auto* pundo_block = new PUndoBlock();

    for (int i = 0; i < undo_block->transaction_hashes.size(); i++) {
        pundo_block->add_transaction_hashes(undo_block->transaction_hashes[i]);
        ///handle undo_coin_record
        PUndoCoinRecord* pundo_coin_record = pundo_block->add_undo_coin_records();
        pundo_coin_record->set_version(undo_block->undo_coin_records[i]->version);
        pundo_coin_record->set_is_coin_base(undo_block->undo_coin_records[i]->is_coin_base);
        pundo_coin_record->set_height(undo_block->undo_coin_records[i]->height);
        for (int i = 0; i < undo_block->undo_coin_records[i]->utxo.size(); i++) {
            pundo_coin_record->add_utxo(undo_block->undo_coin_records[i]->utxo[i]);
            pundo_coin_record->add_amounts(undo_block->undo_coin_records[i]->amounts[i]);
            pundo_coin_record->add_public_keys(undo_block->undo_coin_records[i]->public_keys[i]);
        }
    }

    std::string serialized_undo_block;
    pundo_block->SerializeToString(&serialized_undo_block);
    return serialized_undo_block;
}

/// Deserializes an UndoBlock. Necessary to read UndoBlocks
/// from an "undo" file
std::shared_ptr<UndoBlock> UndoBlock::deserialize(std::string serialized_undo_block) {
    auto* pundo_block = new PUndoBlock();

    pundo_block->ParseFromString(serialized_undo_block);

    std::vector<uint32_t> transaction_hashes;
    std::vector<std::shared_ptr<UndoCoinRecord>> undo_coin_records;

    for (int i = 0; i < pundo_block->transaction_hashes_size(); i++) {
        transaction_hashes.push_back(pundo_block->transaction_hashes(i));
        PUndoCoinRecord pundo_coin_record = pundo_block->undo_coin_records(i);
        ///Handle UndoCoinRecord
        std::vector<uint32_t> utxo;
        std::vector<uint32_t> amounts;
        std::vector<uint32_t> public_keys;
        for (int i = 0; i < pundo_coin_record.utxo_size(); i++) {
            utxo.push_back(pundo_coin_record.utxo(i));
            amounts.push_back(pundo_coin_record.amounts(i));
            public_keys.push_back(pundo_coin_record.public_keys(i));
        }
        std::shared_ptr<UndoCoinRecord> undo_coin_record = std::shared_ptr<UndoCoinRecord>( new UndoCoinRecord(pundo_coin_record.version(),
                                                                                                               pundo_coin_record.is_coin_base(),
                                                                                                               pundo_coin_record.height(),
                                                                                                               utxo,
                                                                                                               amounts,
                                                                                                               public_keys));
        undo_coin_records.push_back(undo_coin_record);
    }

    std::shared_ptr<UndoBlock> undo_block = std::shared_ptr<UndoBlock>(new UndoBlock(transaction_hashes, undo_coin_records));
    return undo_block;
}

