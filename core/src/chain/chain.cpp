#include <chain.h>
#include <rathcrypto.h>

//SECTION 1
Chain::Chain() {
    this->_active_chain_length = 1;
    this->_active_chain_last_block = construct_genesis_block();
    this->_block_info_database = nullptr;
    this->_chain_writer = nullptr;
    this->_coin_database = nullptr;


}

//SECTION 2
void Chain::handle_block(std::unique_ptr<Block> block) {
    /*If the block is being appended to the active chain, validate the block using the coin_database. If the block is validated, update the UTXO and mempool in the coin_database. Validate_and_store_block in coin_databse can be called to accomplish both of these things with one function call. It is intended to validate the block, and store the block if valid. 
If the block is not being appended to the active chain, validate the block using the coin_database (can call validate_block in coin_database).
If the block has been validated (no matter if it was being appended to active chain or not), store the block on disk.
To do this, first, we will need to get the block’s height in the chain. If the block is being appended to the active chain, this is easy; we can just access the _active_chain_length field and add 1. However, if it is not being appended to the active chain, we must find the previous block’s height and then add 1 to that. To find the previous block’s height, we first find the hash of the previous block by looking at the block’s header. Then we call get_block_record on the coin_database to get the corresponding block record for the previous block. This will contain the previous block’s height.
Then, we create an undo block for this block. We recommend using a helper function called make_undo_block. After this, we store the block and the undo_block on disk using the chain_writer. Store_block, write_block, and write_undo_block may be helpful for this.
Now we have to update the fields of the chain object.
If the block was appended to the main chain, then we simply increment _active_chain_length and set _active_chain_last_block to this block.
If the block’s height is bigger than the _active_chain_length, it means a fork has surpassed the active chain. To deal with a fork we need to get every forked block on the forked chain until a common ancestor block with the main chain has been reached (get_forked_blocks_stack is intended for this purpose). Afterwards, we need to get every undo block on the active_chain from the last_block to the common ancestor block (get_undo_blocks_queue is intended for this). Now, we need to iterate through the undo blocks, applying each one to the coin_database to “reverse the UTXO”. This could be done through a helper function in coin_database. Then, every forked block needs to be added to the coin_database. This can be done through calling store_block multiple times.
Otherwise, we don’t need to update any fields.
*/

    bool returncode;
    bool onactchain;
    if (block->block_header->previous_block_hash == get_last_block_hash()) {
        onactchain = true;
       returncode = _coin_database->validate_and_store_block(block->get_transactions());
    } else {
        onactchain = false;
        std::vector<std::unique_ptr<Transaction>> temp = block->get_transactions();
       returncode = _coin_database->validate_block(temp);
    }

    if (returncode == true) {
        uint32_t chainlen;
        if (onactchain == true) {
            chainlen = get_active_chain_length() + 1;
        } else {
            chainlen = _block_info_database->get_block_record(block->block_header->previous_block_hash)->height + 1;
        }

        //make an undo block
        UndoBlock uBlock = make_undo_block(*block);
        _chain_writer->store_block(*block, uBlock, chainlen);

        if (onactchain == true) {
            _active_chain_length++;
            _active_chain_last_block = std::move(block);
        } else {
            if (chainlen > get_active_chain_length()) {
                std::vector<std::unique_ptr<Block>> forked_blocks = get_forked_blocks_stack(block->block_header->previous_block_hash);
                _coin_database->undo_coins(get_undo_blocks_queue(chainlen));
                //Then, every forked block needs to be added to the coin_database. This can be done through calling store_block multiple times.
                for (int i = 0; i < forked_blocks.size(); i++) {
                    _chain_writer->store_block(*forked_blocks[i], make_undo_block(*forked_blocks[i]), chainlen - i);
                }

            }
        }
    }

}
void Chain::handle_transaction(std::unique_ptr<Transaction> transaction) {
    _coin_database->validate_and_store_transaction(std::move(transaction));
}

//SECTION 3
uint32_t Chain::get_chain_length(uint32_t block_hash) {
    //call get_block_record on block_info_database get height else return 0
    std::unique_ptr<BlockRecord> rec = _block_info_database->get_block_record(block_hash);
    if (rec != nullptr) {
        return rec->height;
    } else {
        return 0;
    }
}

std::unique_ptr<Block> Chain::get_block(uint32_t block_hash) {
    std::unique_ptr<BlockRecord> return_block = _block_info_database->get_block_record(block_hash);
    FileInfo fInfo = FileInfo(return_block->block_file_stored, return_block->block_offset_start, return_block->block_offset_end);
    std::string to_deserialize = _chain_writer->read_block(fInfo);
    return Block::deserialize(to_deserialize);
}
std::vector<std::unique_ptr<Block>> Chain::get_active_chain(uint32_t start, uint32_t end) { //fix this
   std::unique_ptr<Block> last_block = get_last_block();
   //iterate back to end
   for (int i = 0; i < (get_active_chain_length() - end); i++) {
    last_block = get_block(last_block->block_header->previous_block_hash);
   }

    //add each block before to vector
   std::vector<std::unique_ptr<Block>> v;
   for (int i = 0; i < end - start; i++) {
       v.emplace_back(std::move(last_block));
    last_block = get_block(last_block->block_header->previous_block_hash);
   }

   return v;
}
std::vector<uint32_t> Chain::get_active_chain_hashes(uint32_t start, uint32_t end) {
    uint32_t current_hash = get_last_block_hash();
    std::unique_ptr<Block> last_block = get_last_block();
    //iterate back to end
    for (int i = 0; i < (get_active_chain_length() - end); i++) {
        current_hash = last_block->block_header->previous_block_hash;
        last_block = get_block(last_block->block_header->previous_block_hash);

    }

    //add each block before to vector
    std::vector<uint32_t> v;
    for (int i = 0; i < end - start; i++) {
        v.emplace_back(current_hash);
        current_hash = last_block->block_header->previous_block_hash;
        last_block = get_block(last_block->block_header->previous_block_hash);
    }
    return v;
}

//SECTION 4
std::unique_ptr<Block> Chain::get_last_block() {
    //std::unique_ptr<Block> return_block = make_unique<Block>(_active_chain_last_block->block_header, _active_chain_last_block->get_transactions());

    //return return_block;
}
uint32_t Chain::get_last_block_hash() {
    std::string lbs = Block::serialize(*get_last_block());
    return RathCrypto::hash(lbs);
}
uint32_t Chain::get_active_chain_length() const {
    return _active_chain_length;
}

//SECTION 5
//std::vector<std::pair<uint32_t, uint8_t>> Chain::get_all_utxo(uint32_t public_key) {}

//PRIVATE METHODS
std::vector<std::unique_ptr<UndoBlock>> Chain::get_undo_blocks_queue(uint32_t branching_height) { // should change this one up

    std::vector<std::unique_ptr<UndoBlock>> v;

    std::unique_ptr<BlockRecord> last_block = _block_info_database->get_block_record(get_last_block_hash());

    for (int i = 0; i < branching_height; i++) {
        std::unique_ptr<FileInfo> uFI = std::make_unique<FileInfo>(last_block->undo_file_stored, last_block->undo_offset_start, last_block->undo_offset_end);
        std::unique_ptr<UndoBlock> uBlock = UndoBlock::deserialize(_chain_writer->read_undo_block(*uFI));
        v.push_back(std::move(uBlock));

        last_block = _block_info_database->get_block_record(last_block->block_header->previous_block_hash);
    }

    return v;
}
std::vector<std::unique_ptr<Block>> Chain::get_forked_blocks_stack(uint32_t starting_hash) {
/*
    std::vector<std::shared_ptr<Block>> block_stack;
    uint32_t current_block_hash = starting_hash;
    std::unordered_map<uint32_t,bool> main_hashes;
    for (auto k : _last_seven_on_active_chain){
        main_hashes[k] = true;
    }
    bool finished = false;
    while (!finished) {
        std::unique_ptr<BlockRecord> current_block_record = Chain::_block_info_database->get_block_record(
                current_block_hash);
        if (main_hashes.contains(current_block_hash)) {
            finished = true;
        } else {
            std::unique_ptr<Block> forked_block = get_block(current_block_hash);
            block_stack.push_back(std::move(forked_block));
        }
        current_block_hash = current_block_record->block_header->previous_block_hash;
    }
    return block_stack;
    */

    std::vector<std::shared_ptr<Block>> retblocks;
    uint32_t index = starting_hash;


}
std::unique_ptr<Block> Chain::construct_genesis_block() {
    std::unique_ptr<BlockHeader> blk_header = std::make_unique<BlockHeader>(1, 0, 0, 0, 0, 0);

    std::vector<std::unique_ptr<TransactionInput>> tInputs;
    std::vector<std::unique_ptr<TransactionOutput>> tOutputs;

    std::unique_ptr<TransactionOutput> to1 = std::make_unique<TransactionOutput>(100, 12345);
    std::unique_ptr<TransactionOutput> to2 = std::make_unique<TransactionOutput>(200, 67891);
    std::unique_ptr<TransactionOutput> to3 = std::make_unique<TransactionOutput>(300, 23456);

    tOutputs.push_back(std::move(to1));
    tOutputs.push_back(std::move(to2));
    tOutputs.push_back(std::move(to3));


    std::unique_ptr<Transaction> transaction = std::make_unique<Transaction>(std::move(tInputs), std::move(tOutputs), 0, 0);
    std::vector<std::unique_ptr<Transaction>> v;
    v.push_back(std::move(transaction));

    std::unique_ptr<Block> return_block = std::make_unique<Block>(std::move(blk_header), std::move(v));
    return  return_block;
}

UndoBlock Chain::make_undo_block(const Block& new_block) {


    return UndoBlock(std::vector(), std::vector());
}
