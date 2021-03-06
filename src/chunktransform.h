#ifndef CHUNKTRANSFORM_H_
#define CHUNKTRASNFORM_H_
#pragma once

#include <string>

namespace attic {

class ChunkInfo;

class ChunkTransform {
    void GenerateChunkName();
    void Compress(const std::string& in, std::string& out);
    bool Encrypt(const std::string& in, std::string& out);
    void Encode(const std::string& in, std::string& out);
    void GenerateVerificationHash(std::string& out);
    void Compose(const std::string& in, std::string& out);

    void Decode(const std::string& in, std::string& out);
    bool Decrypt(const std::string& in, std::string& out);
    void Decompress(const std::string& in, std::string& out);
    void Decompose(const std::string& in, std::string& out);
public:
    ChunkTransform(const std::string& chunk, const std::string& file_key);
    ~ChunkTransform();

    void Reset();

    // Outbound transformation
    bool TransformOut();    
    // Inbound transformation
    bool TransformIn(const ChunkInfo* ci);

    const std::string& file_key() const             { return file_key_; }
    const std::string& chunk_iv() const             { return chunk_iv_; }
    const std::string& name() const                 { return name_; }
    const std::string& plaintext_hash() const       { return plaintext_hash_; }
    const std::string& ciphertext_hash() const      { return ciphertext_hash_; }
    const std::string& verification_hash() const    { return verification_hash_; }
    const std::string& data() const                 { return data_; }
    const std::string& finalized_data() const       { return finalized_data_; }
private:
    // meta data
    std::string chunk_iv_;
    std::string file_key_;
    std::string name_;
    std::string plaintext_hash_;
    std::string ciphertext_hash_;
    std::string verification_hash_;

    // core data
    std::string data_;
    std::string finalized_data_;
};

/*
 * Chunk Data composition v1
 *  Format Version              : 1 byte
 *  Uncompressed chunk len      : 4 bytes(byte array)
 *  Compression Level           : 1 byte (0-9)
 *  Iv len                      : 4 bytes (byte array)
 *  Iv                          : (variable)
 *  Data len                    : 4 bytes (byte array)
 *  Data                        : (variable)
 */

} //namespace
#endif

