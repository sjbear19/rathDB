/*
 * This file belongs to Brown University's computer
 * science department as part of csci1951L - Blockchains
 * and cryptocurrencies.
 *
 * This software was developed by Colby Anderson (HTA)
 * and Parker Ljung (TA) of csci1951L in Summer of 2021.
 */

#include <block_record.h>

BlockRecord::BlockRecord(std::shared_ptr<BlockHeader> block_header, uint32_t height, uint32_t num_transactions, std::unique_ptr<FileInfo> block_info,
                         std::unique_ptr<FileInfo> undo_info) : block_header(block_header), height(height), num_transactions(num_transactions) {

    block_file_stored = block_info->file_name;
    block_offset_start = block_info->start;
    block_offset_end = block_info->end;

    undo_file_stored = undo_info->file_name;
    undo_offset_start = undo_info->start;
    undo_offset_end = undo_info->end;
}

std::string BlockRecord::serialize(std::shared_ptr<BlockRecord> block_record) {
    ///Block Header
    auto* pblock_header = new PBlockHeader();
    pblock_header->set_version(block_record->block_header->version);
    pblock_header->set_previous_block_hash(block_record->block_header->previous_block_hash);
    pblock_header->set_merkle_root(block_record->block_header->merkle_root);
    pblock_header->set_difficulty_target(block_record->block_header->difficulty_target);
    pblock_header->set_nonce(block_record->block_header->nonce);
    pblock_header->set_timestamp(block_record->block_header->timestamp);

    ///PBlockRecord
    auto* pblock_record = new PBlockRecord();
    pblock_record->set_allocated_block_header(pblock_header);
    pblock_record->set_height(block_record->height);
    pblock_record->set_num_transactions(block_record->num_transactions);

    pblock_record->set_block_file_stored(block_record->block_file_stored);
    pblock_record->set_block_offset_start(block_record->block_offset_start);
    pblock_record->set_block_offset_end(block_record->block_offset_end);

    pblock_record->set_undo_file_stored(block_record->undo_file_stored);
    pblock_record->set_undo_offset_start(block_record->undo_offset_start);
    pblock_record->set_undo_offset_end(block_record->undo_offset_end);

    ///String
    std::string serialized_block_record;
    pblock_record->SerializeToString(&serialized_block_record);

    return serialized_block_record;
}

std::shared_ptr<BlockRecord> BlockRecord::deserialize(const std::string& serialized_block_record) {
    auto* pblock_record = new PBlockRecord();
    pblock_record->ParseFromString(serialized_block_record);
    ///BlockHeader
    std::unique_ptr<BlockHeader> block_header = std::make_unique<BlockHeader>( pblock_record->block_header().version(),
                                                                               pblock_record->block_header().previous_block_hash(),
                                                                               pblock_record->block_header().merkle_root(),
                                                                               pblock_record->block_header().difficulty_target(),
                                                                               pblock_record->block_header().nonce(),
                                                                               pblock_record->block_header().timestamp());
    ///FileInfos
    std::unique_ptr<FileInfo>  block_file_info = std::make_unique<FileInfo> (
            pblock_record->block_file_stored(),
            pblock_record->block_offset_start(),
            pblock_record->block_offset_end());

    std::unique_ptr<FileInfo> undo_file_info = std::make_unique<FileInfo>(
            pblock_record->undo_file_stored(),
            pblock_record->undo_offset_start(),
            pblock_record->undo_offset_end());

    ///BlockRecord
    std::shared_ptr<BlockRecord> block_record = std::shared_ptr<BlockRecord>( new BlockRecord(
            std::move(block_header),
            pblock_record->height(),
            pblock_record->num_transactions(),
            std::move(block_file_info),
            std::move(undo_file_info)));

    return block_record;
}
