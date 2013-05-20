#ifndef FOLDERTASK_H_
#define FOLDERTASK_H_
#pragma once

#include "tenttask.h"
#include "filepost.h"
#include "folderpost.h"

namespace attic {

class FolderTask : public TentTask{
    int CreateFolderPost(Folder& folder, std::string& id_out);
    int CreateFolder();

    int DeleteFolder();
    int MarkFolderPostDeleted(Folder& folder);
    int MarkFilePostDeleted(FileInfo& fi);

    int RenameFolder();

    int RetrieveFilePost(const std::string& post_id, FilePost& out);
    int PostFilePost(const std::string& post_id, FilePost& fp);
    int RetrieveFolderPost(const std::string& post_id, FolderPost& out);
    int PostFolderPost(const std::string& post_id, FolderPost& fp);
public:
    FolderTask(FileManager* pFm,
               CredentialsManager* pCm,
               const AccessToken& at,
               const Entity& entity,
               const TaskContext& context);

    ~FolderTask();

    virtual void OnStart() {} 
    virtual void OnPaused() {}
    virtual void OnFinished() {}

    void RunTask();
};

}//namespace
#endif

