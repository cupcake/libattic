#ifndef FILETABLE_H_
#define FILETABLE_H_
#pragma once

#include <string>
#include "constants.h"
#include "tablehandler.h"
#include "fileinfo.h"

namespace attic {
/* FileTable deals with all file meta data operations
 * TODO :: move away from using the filepath as a key, go by post id
 *
 * */
class FileTable : public TableHandler {
    friend class Manifest;

    void ExtractFileInfoResults(const SelectResult& res, const int step, FileInfo& out);

    FileTable(sqlite3* db) : TableHandler(db, cnst::g_filetable) {}
    FileTable(const FileTable& rhs) : TableHandler(rhs.db(), rhs.table_name()) {}
    FileTable operator=(const FileTable& rhs) { return *this; }
public:
    typedef std::deque<FileInfo> FileInfoList;
    
    ~FileTable() {}

    bool CreateTable();
    bool IsFileInManifest(const std::string& filepath); // depricated
    bool IsFileInManifestWithId(const std::string& post_id);

    bool InsertFileInfo(const std::string& filename, 
            const std::string& filepath,
            const unsigned int chunk_count,
            const std::string& chunk_data,
            const unsigned int file_size,
            const std::string& post_id,
            const std::string& post_version,
            const std::string& encrypted_key,
            const std::string& iv,
            bool deleted,
            const std::string& folder_post_id,
            const std::string& plaintext_hash,
            bool shared);

    bool set_file_deleted_for_id(const std::string& post_id, const int val);
    bool set_filepath_for_id(const std::string& post_id, const std::string& filepath);
    bool set_filename_for_id(const std::string& post_id, const std::string& filename);
    bool set_folder_post_id(const std::string& post_id, const std::string& folder_post_id);
    bool set_file_version_for_id(const std::string& post_id, const std::string& version); 
    bool set_file_shared(const std::string& post_id, int shared);

    bool QueryForFile(const std::string& filepath, FileInfo& out);
    bool QueryForFile(const std::string& filename, 
            const std::string& folder_post_id,
            FileInfo& out);
    bool QueryForFileByPostId(const std::string& post_id, FileInfo& out);
    bool QueryAllFiles(FileInfoList& out);
    bool QueryAllFilesForFolder(const std::string& folderid, FileInfoList& out);

    bool MarkAllFilesDeletedInFolder(const std::string& folderid);
};

} //namespace
#endif

