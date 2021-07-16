#include <chain_writer.h>
#include <stdio.h>
#include <filesystem>
#include <rathcrypto.h>

const std::string ChainWriter::_file_extension = "data";
const std::string ChainWriter::_block_filename = "blocks";
const uint16_t ChainWriter::_max_block_file_size = 1000;
const std::string ChainWriter::_undo_filename = "undo_blocks";
const uint16_t ChainWriter::_max_undo_file_size = 1000;
const std::string ChainWriter::_data_directory = "data";

ChainWriter::ChainWriter() {
    _current_block_file_number = 0;
    _current_block_offset = 0;
    _current_undo_file_number = 0;
    _current_undo_offset = 0;
    std::filesystem::create_directory(_data_directory);
}
std::unique_ptr<BlockRecord> ChainWriter::store_block(const Block& block, const UndoBlock& uBlock, uint32_t height) {
   std::unique_ptr<BlockHeader> bh = std::make_unique<BlockHeader>(block.block_header->version,block.block_header->previous_block_hash,
                                       block.block_header->merkle_root, block.block_header->difficulty_target,
                                       block.block_header->nonce, block.block_header->timestamp);
std::unique_ptr<FileInfo> v1 = write_block(Block::serialize(block));
std::unique_ptr<FileInfo> v2 = write_undo_block(UndoBlock::serialize(uBlock));
std::unique_ptr<BlockRecord> retblock = std::make_unique<BlockRecord>(std::move(bh),block.transactions.size(), height, *v1,*v2);
return std::move(retblock);
}

std::unique_ptr<FileInfo> ChainWriter::write_block(std::string serialized_block) {
    //find file to write to, check if full
    //if full, open new file
    //else write to original
    if (size(serialized_block) + _current_block_offset >= _max_block_file_size) {
        _current_block_offset = 0;
        _current_block_file_number++;
    }

    const char* filename = (_data_directory + "/" + _block_filename + "_" + std::to_string(_current_block_file_number) + "." + _file_extension).c_str();
    const char* type_convert = serialized_block.c_str();

    auto file = fopen(filename, "a+");
    size_t fwrite_output = fwrite(type_convert, strlen(type_convert), 1, file);
    fclose(file);

    std::unique_ptr<FileInfo> to_return = std::make_unique<FileInfo>(filename, _current_block_offset, _current_block_offset + fwrite_output);
    _current_block_offset = _current_block_offset + fwrite_output;

    return std::move(to_return);
}
std::unique_ptr<FileInfo> ChainWriter::write_undo_block(std::string serialized_block) {
    //find file to write to, check if full
    //if full, open new file
    //else write to original
    if (size(serialized_block) + _current_undo_offset >= _max_undo_file_size) {
        _current_undo_offset = 0;
        _current_block_file_number++;
    }

    const char* filename = (_data_directory + "/" + _undo_filename + "_" + std::to_string(_current_undo_file_number) + "." + _file_extension).c_str();
    const char* type_convert = serialized_block.c_str();

    auto file = fopen(filename, "a+");
    size_t fwrite_output = fwrite(type_convert, strlen(type_convert), 1, file);
    fclose(file);

    std::unique_ptr<FileInfo> to_return = std::make_unique<FileInfo>(filename, _current_undo_offset, _current_undo_offset + fwrite_output);
    _current_undo_offset = _current_undo_offset + fwrite_output;

    return std::move(to_return);
}

/*
std::string ChainWriter::read_block(const FileInfo& block_location) {
    auto file = fopen((block_location.file_name).c_str(), "r");
    fseek(file, block_location.start, SEEK_SET);

    size_t size = (block_location.end - block_location.start) + 1;
    char buf[size];
    fread(buf, (block_location.end - block_location.start), 1, file);
    fclose(file);
    std::string ret = buf;
    return ret;
}
std::string ChainWriter::read_undo_block(const FileInfo& undo_block_location) {

    auto file = fopen((undo_block_location.file_name).c_str(), "r");
    fseek(file, undo_block_location.start, SEEK_SET);

    char buf[(undo_block_location.end - undo_block_location.start)];
    fread(buf, (undo_block_location.end - undo_block_location.start), 1, file);
    std::string ret = buf;
    return ret;
}*/








