#include <chain_writer.h>

const std::string ChainWriter::_file_extension = ".dat";
const std::string ChainWriter::_block_filename = "block";
const uint16_t ChainWriter::_max_block_file_size = 1000;
const std::string ChainWriter::_undo_filename = "undo_block";
const uint16_t ChainWriter::_max_undo_file_size = 1000;