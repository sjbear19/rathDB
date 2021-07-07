#include <chain_writer.h>

const std::string ChainWriter::_file_extension = "data";
const std::string ChainWriter::_block_filename = "blocks";
const uint16_t ChainWriter::_max_block_file_size = 1000;
const std::string ChainWriter::_undo_filename = "undo_blocks";
const uint16_t ChainWriter::_max_undo_file_size = 1000;
const std::string ChainWriter::_data_directory = "data";

ChainWriter::ChainWriter() {}
std::shared_ptr<BlockRecord> ChainWriter::store_block(std::shared_ptr<Block> block, uint32_t height) {}
std::unique_ptr<FileInfo> ChainWriter::write_block(std::string serialized_block) {}
std::unique_ptr<FileInfo> ChainWriter::write_undo_block(std::string serialized_block) {}
std::string ChainWriter::read_block(std::unique_ptr<FileInfo> block_location) {}
std::string ChainWriter::read_undo_block(std::unique_ptr<FileInfo> undo_block_location) {}

