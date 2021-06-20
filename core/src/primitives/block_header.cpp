#include <block_header.h>

BlockHeader::BlockHeader(uint8_t version, uint32_t previous_block_hash, uint32_t merkle_root, uint32_t difficulty_target,
                         uint32_t nonce, uint32_t timestamp) : version(version), previous_block_hash(previous_block_hash),
                                                               merkle_root(merkle_root), difficulty_target(difficulty_target), nonce(nonce), timestamp(timestamp) {}
