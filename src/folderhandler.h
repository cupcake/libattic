#ifndef FOLDERHANDLER_H_
#define FOLDERHANDLER_H_
#pragma once

#include <string>
#include <deque>
#include "folder.h"
#include "folderpost.h"
#include "filemanager.h"

#include "accesstoken.h"

namespace attic {

class FolderHandler {
    int RetrieveSubFolders(Folder& folder, std::deque<Folder>& out);
    int RetrieveFilesInFolder(Folder& folder, std::deque<FileInfo>& out);
    int RetrieveAllFilesAndFoldersInFolder(Folder& folder, 
                                           std::deque<FileInfo>& file_out,
                                           std::deque<Folder>& folder_out);
    void SeparatePath(const std::string& full_path, std::deque<std::string>& names);

    bool CreateDirectoryTree(FolderPost& fp);
public:
    FolderHandler(FileManager* fm);
    ~FolderHandler();

    // Attempts to create a new folder entry, post and all, will check
    // for existing folder entry in local cache returning a bool upon
    // success or failure, and a folder on success
    bool AttemptCreateNewFolderEntry(const std::string& foldername, 
                                     const std::string& entity_url,
                                     const std::string& parent_post_id,
                                     const std::string& posts_feed,
                                     const AccessToken& at,
                                     Folder& out);

    bool ValidateFolder(FolderPost& fp);
    // Validates that the posts exist within the local cache, does not update them
    bool ValidateFolder(FolderPost& fp,
                        const std::string& entity_url,
                        const std::string& post_path,
                        const AccessToken& at);



    // Validate an absolute folderpath
    bool ValidateFolderPath(const std::string& folderpath, 
                            const std::string& entity_url,
                            const std::string& posts_feed,
                            const std::string& post_path,
                            const AccessToken& at);

    // Validates that the posts exist within the local cache, does not update them
    bool ValidateFolderTree(const std::string& folder_post_id,
                            const std::string& entity_url,
                            const std::string& post_path,
                            const AccessToken& at);

    bool RetrieveFolders(const std::string& folderpath, 
                         const std::string& working_directory,
                         std::deque<std::string>& out);
 
    bool InsertFolder(const FolderPost& fp);
    bool SetFolderPostId(Folder& folder, const std::string& post_id);
    bool SetFolderParentPostId(Folder& folder, const std::string& post_id);
    
    void RenameFolder(const std::string& old_folderpath, 
                      const std::string& new_folderpath,
                      std::deque<FileInfo>& file_list,
                      std::deque<Folder>& folder_list);

    void DeleteFolder(const std::string& folderpath);

    void DeleteFolder(const std::string& folderpath, 
                      std::deque<FileInfo>& file_out,
                      std::deque<Folder>& folder_out);

    void MarkFolderDeleted(FolderPost& fp);

    bool GetFolderById(const std::string& folder_id, Folder& out);
    bool GetFolderByAbsolutePath(const std::string& folderpath, Folder& out);
    bool IsFolderInCache(const std::string& folder_name, const std::string& parent_post_id);
    bool IsFolderInCacheWithId(const std::string& post_id);
    bool IsFolderDeleted(const std::string& post_id);
    bool SetFolderDeleted(const std::string& post_id, bool del);

    int CreateFolderPost(Folder& folder, 
                         const std::string& posts_feed,
                         const AccessToken& at,
                         std::string& id_out);

    bool UpdateFolderPost(Folder& folder, 
                          const std::string& post_id,
                          const std::string& entity_url,
                          const std::string& post_path,
                          const AccessToken& at);

    bool UnDeleteFolderPost(Folder& folder, 
                            const std::string& post_id,
                            const std::string& entity_url,
                            const std::string& post_path,
                            const AccessToken& at);
 
    int RetrieveFolderPost(const std::string& post_id, 
                           const std::string& entity_url,
                           const std::string& post_path,
                           const AccessToken& at,
                           FolderPost& out);
 
    int PostFolderPost(const std::string& post_id, 
                       const std::string& entity_url,
                       const std::string& post_path,
                       const AccessToken& at,
                       FolderPost& fp);
 
private:
    FileManager* file_manager_;
};

}//namespace
#endif

