#ifndef CHUNKPOST_H_
#define CHUNKPOST_H_
#pragma once

#include <string>
#include <vector>
#include <map>

#include "post.h"
#include "chunkinfo.h"
#include "fileinfo.h"

namespace attic { 

class ChunkPost : public Post {
public:
    typedef std::map<unsigned int, ChunkInfo> ChunkInfoList;
    ChunkPost();
    ~ChunkPost();

    virtual void Serialize(Json::Value& root);  
    virtual void Deserialize(Json::Value& root);

    bool HasChunk(const std::string& name);
    ChunkInfo* GetChunkInfo(const std::string& name);
    bool GetChunkInfo(const std::string& name, ChunkInfo& out);
    void PushBackChunkInfo(const ChunkInfo& ci, const unsigned int position);

    unsigned int chunk_info_list_size() { return chunk_info_list_.size(); }
    ChunkInfoList* chunk_info_list() { return &chunk_info_list_; }
    int group() { return group_; }

    int set_chunk_info_list(FileInfo::ChunkMap& list);
    void set_group(const int g) { group_ = g; }
private:
    // TODO :: optimization, allocate chunk info on heap, both maps have ptr; test laster for speed and memory
    //         consumption
    FileInfo::ChunkMap  chunk_map_;
    ChunkInfoList chunk_info_list_; // Key by position
    int group_;
};

}//namespace
#endif

