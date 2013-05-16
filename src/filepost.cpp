#include "filepost.h"

#include <stdio.h>

#include "fileinfo.h"
#include "constants.h"
#include "errorcodes.h"
#include "crypto.h"

namespace attic { 

FilePost::FilePost(){
    set_type(cnst::g_attic_file_type);
}

FilePost::~FilePost() {}

void FilePost::InitializeFilePost(FileInfo* fi,  bool is_public) {
    if(fi) {
        // basic attic post info
        set_public(is_public);
        set_relative_path(fi->filepath());
        set_name(fi->filename());
        set_file_size(fi->file_size());
        // attic post key info
        set_key_data(fi->encrypted_key());
        set_iv_data(fi->file_credentials_iv());
        // set chunk info
        PushBackChunkPostId(fi->chunk_post_id()); // TODO :: change this to a mention
        FileInfo::ChunkMap* chunk_list = fi->GetChunkInfoList();
        if(chunk_list) {
            std::cout<<" CHUNK LIST SIZE : " << chunk_list->size() << std::endl;
            FileInfo::ChunkMap::iterator itr = chunk_list->begin();

            std::string identifier;
            for(;itr != chunk_list->end(); itr++) {
                identifier.clear();
                identifier = itr->second.checksum();
                PushBackChunkIdentifier(identifier);
            }
        }
        else {
            std::cout<<" INVALID CHUNK LIST : " << std::endl;
        }

        FileInfo::AliasMap* alias_list = fi->GetAliasMap();
        if(alias_list) {
            FileInfo::AliasMap::iterator itr = alias_list->begin();
            for(; itr != alias_list->end(); itr++) {
                past_aliases_.push_back(itr->first);
            }
        }

        set_folder_post(fi->folder_post_id());
    }
}

void FilePost::Serialize(Json::Value& root) {
    Json::Value content(Json::objectValue);
    content["name"] = name_;
    content["path"] = relative_path_;
    
    Json::Value chunkposts;//(Json::objectValue);
    SerializeChunkPosts(chunkposts);
    content["chunk_posts"] = chunkposts;

    Json::Value chunkids;
    SerializeChunkIds(chunkids);
    content["chunk_ids"] = chunkids;

    Json::Value aliases;
    SerializePastAliases(aliases);
    content["past_aliases"] = aliases;

    std::string key_data;
    crypto::Base64EncodeString(key_data_, key_data);
    content["kdata"] = key_data;

    std::string iv_data;
    crypto::Base64EncodeString(iv_data_, iv_data);
    content["vdata"] = iv_data;

    content["size"] = file_size_;
    content["folder_post"] = folder_post_;

    set_content("file_content", content);

    Post::Serialize(root);
}

void FilePost::SerializeChunkPosts(Json::Value& val) {
    jsn::SerializeVector(chunk_posts_, val);
}

void FilePost::SerializeChunkIds(Json::Value& val) { 
    jsn::SerializeVector(chunk_ids_, val);
}

void FilePost::SerializePastAliases(Json::Value& val) {
    jsn::SerializeVector(past_aliases_, val);
}

void FilePost::Deserialize(Json::Value& root) {
    std::cout<<" deserializing file post " << std::endl;
    Post::Deserialize(root);

    Json::Value content;
    get_content("file_content", content);

    name_           = content.get("name", "").asString();
    relative_path_  = content.get("path", "").asString();
    folder_post_    = content.get("folder_post", "").asString();

    std::string size = content.get("size", "").asString();
    file_size_ = atoi(size.c_str());

    DeserializeChunkPosts(content["chunk_posts"]);
    DeserializeChunkIds(content["chunk_ids"]);
    DeserializePastAliases(content["past_aliases"]);

    std::string key_data = content["kdata"].asString();
    std::string iv_data = content["vdata"].asString();

    crypto::Base64DecodeString(key_data, key_data_);
    crypto::Base64DecodeString(iv_data, iv_data_);


}

void FilePost::DeserializeChunkPosts(Json::Value& val) {
    jsn::DeserializeIntoVector(val, chunk_posts_);
}

void FilePost::DeserializeChunkIds(Json::Value& val) {
    jsn::DeserializeIntoVector(val, chunk_ids_);
}

void FilePost::DeserializePastAliases(Json::Value& val) {
    jsn::DeserializeIntoVector(val, past_aliases_);
    for(int i=0; i<past_aliases_.size();i++)
        std::cout<<past_aliases_[i]<<std::endl;
}

} //namespace
