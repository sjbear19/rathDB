#include <block_info_database.h>

void BlockInfoDatabase::store_block_record(uint32_t hash, const BlockRecord& record) {
	_database->put_safely(std::to_string(hash), BlockRecord::serialize(record));
}
std::unique_ptr<BlockRecord> BlockInfoDatabase::get_block_record(uint32_t block_hash) {
	_database->get_safely(std::to_string(block_hash));
}
