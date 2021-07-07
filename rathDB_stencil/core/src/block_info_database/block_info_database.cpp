#include <block_info_database.h>
UndoBlock::UndoBlock(std::vector<uint32_t> transaction_hashes_, std::vector<std::shared_ptr<UndoCoinRecord>>  undo_coin_records_) {
	transaction_hashes = transaction_hashes_;
	undo_coin_records = undo_coin_records_;
}
void BlockInfoDatabase::store_block_record(uint32_t hash, std::shared_ptr<BlockRecord> record) {
	this->_database.put_safely(hash, record);
}
BlockRecord BlockInfoDatabase::get_block_record(uint32_t block_hash) {
	this->_database.get_safely(block_hash);
}
