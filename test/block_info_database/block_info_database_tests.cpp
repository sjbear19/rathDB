/*
 * This file belongs to Brown University's computer
 * science department as part of csci1951L - Blockchains
 * and cryptocurrencies.
 *
 * This software was developed by Colby Anderson (HTA)
 * and Parker Ljung (TA) of csci1951L in Summer of 2021.
 */

#include <gtest/gtest.h>
#include <block_record.h>

#include <memory>


std::shared_ptr<BlockRecord> make_dummy_block_record() {
    std::shared_ptr<BlockHeader> block_header = std::make_shared<BlockHeader>(1, 2, 3, 4, 5, 6);
    std::unique_ptr<FileInfo>  block_file_info = std::make_unique<FileInfo> ("block",2,3);
    std::unique_ptr<FileInfo>  undo_file_info = std::make_unique<FileInfo> ("undo",4,5);

    std::shared_ptr<BlockRecord> block_record = std::shared_ptr<BlockRecord>( new BlockRecord(
            std::move(block_header),
            5,
            100,
            std::move(block_file_info),
            std::move(undo_file_info)
    ));
    return std::move(block_record);
}

TEST(BlockRecord, SerializeBlockRecord) {
    std::shared_ptr<BlockRecord> b = make_dummy_block_record();
    std::string s = BlockRecord::serialize(b);
    std::shared_ptr<BlockRecord> block_record = BlockRecord::deserialize(s);

    ///Checking Blockheader
    EXPECT_EQ(b->block_header->version, block_record->block_header->version);
    EXPECT_EQ(b->block_header->previous_block_hash, block_record->block_header->previous_block_hash);
    EXPECT_EQ(b->block_header->merkle_root, block_record->block_header->merkle_root);
    EXPECT_EQ(b->block_header->difficulty_target, block_record->block_header->difficulty_target);
    EXPECT_EQ(b->block_header->nonce, block_record->block_header->nonce);
    EXPECT_EQ(b->block_header->timestamp, block_record->block_header->timestamp);

    ///Checking Block info
    EXPECT_EQ(b->block_file_stored, block_record->block_file_stored);
    EXPECT_EQ(b->block_offset_start, block_record->block_offset_start);
    EXPECT_EQ(b->block_offset_end, block_record->block_offset_end);

    ///Checking Undo info
    EXPECT_EQ(b->undo_file_stored, block_record->undo_file_stored);
    EXPECT_EQ(b->undo_offset_start, block_record->undo_offset_start);
    EXPECT_EQ(b->undo_offset_end, block_record->undo_offset_end);
}
