#include "renamestrategy.h"

#include "errorcodes.h"
#include "filesystem.h"
#include "credentialsmanager.h"
#include "fileinfo.h"
#include "utils.h"
#include "netlib.h"
#include "jsonserializable.h"
#include "renamehandler.h"

namespace attic {

RenameStrategy::RenameStrategy() {}
RenameStrategy::~RenameStrategy() {}

int RenameStrategy::Execute(FileManager* pFileManager,
                            CredentialsManager* pCredentialsManager) {
    int status = ret::A_OK;
    status = InitInstance(pFileManager, pCredentialsManager);
    // Initialize meta post
    post_path_ = GetConfigValue("post_path");
    posts_feed_ = GetConfigValue("posts_feed");
    std::string filetype = GetConfigValue("file_type");
    if(filetype == "file") {
        std::cout<<" RENAME FILE " << std::endl;
        status = RenameFile();
    }
    else if(filetype == "folder") {
        std::cout<<" RENAME FOLDER  " << std::endl;
        status = RenameFolder();
    }
    return status;
}

int RenameStrategy::RenameFile() {
    int status = ret::A_OK;
    std::string old_filepath = GetConfigValue("original_filepath");
    std::string new_filepath = GetConfigValue("new_filepath");
    std::string entity = GetConfigValue("entity");

    RenameHandler rh(file_manager_);
    status = rh.RenameFileLocalCache(old_filepath, new_filepath);
    if(status == ret::A_OK) {
        FileInfo* fi = RetrieveFileInfo(new_filepath);
        if(fi) {
            FilePost p;
            status = RetrieveFilePost(fi->post_id(), p);
            if(status == ret::A_OK) {
                FilePost new_p;
                rh.UpdateFileMetaPost(p, *fi, new_p);
                status = UpdateFileMetaPost(fi->post_id(), new_p);
            }
        }
        else {
            std::cout<<" COULD NOT FIND FILE FOR PATH : " << new_filepath << std::endl;
            status = ret::A_FAIL_INVALID_FILE_INFO;
        }
    }

    return status;
}

int RenameStrategy::RenameFolder() {
    int status = ret::A_OK;
    std::string old_folderpath = GetConfigValue("original_folderpath"); 
    std::string new_folderpath = GetConfigValue("new_folderpath");
    std::string entity = GetConfigValue("entity");

    std::cout<<" Original folderpath : " << old_folderpath << std::endl;

    std::string old_relative;
    file_manager_->GetRelativePath(old_folderpath, old_relative);
    utils::CheckUrlAndAppendTrailingSlash(old_relative);

    std::string parent;
    fs::GetParentPath(old_folderpath, parent);
    std::string relative;
    file_manager_->GetRelativePath(parent, relative);
    utils::CheckUrlAndAppendTrailingSlash(relative);
    std::string folder_path = relative + new_folderpath;

    // Get parent, append new name
    std::cout<<" new folderpath : " << folder_path << std::endl;

    Folder folder;
    if(file_manager_->GetFolderEntry(old_folderpath, folder)) {
        status = UpdateFolderMetaPost(folder_path, folder);
        if(status == ret::A_OK){
            if(file_manager_->UpdateFolderEntry(folder)) {
                // Update folder contents
                file_manager_->UpdateFolderContents(folder);
                // Retrieve Contents file info, and update
                std::deque<FileInfo> fi_list;
                file_manager_->GetAllFileInfoForFolder(folder.manifest_id(), fi_list);

                std::cout<<" NUMBER OF FILES IN FOLDER : " << fi_list.size() << std::endl;

                // Update meta post
                std::deque<FileInfo>::iterator itr = fi_list.begin();
                for(;itr != fi_list.end(); itr++) {
                    status = UpdateFileMetaPost((*itr).post_id(),
                                                (*itr).filename(),
                                                (*itr).filepath());
                }
            }
            else { 
                std::cout<<" FAILED TO UPDATE FOLDER ENTRY : " << std::endl;
                status = ret::A_FAIL_UPDATE_MANIFEST;
            }
        }
    }

    return status;
}

int RenameStrategy::RetrieveFolderPost(const std::string& post_id, FolderPost& fp) {
    int status = ret::A_OK;

    std::cout<<" RETRIEVE FOLDER POST " << std::endl;

    std::string posturl;
    utils::FindAndReplace(post_path_, "{post}", post_id, posturl);
    std::cout<<" POST URL : " << posturl << std::endl;

    Response response;
    netlib::HttpGet(posturl,
                    NULL,
                    &access_token_,
                    response);
    std::cout<<" CODE : " << response.code << std::endl;
    std::cout<<" BODY : " << response.body << std::endl;
    if(response.code == 200) {
        jsn::DeserializeObject(&fp, response.body);
    }
    else {
        status = ret::A_FAIL_NON_200;
    }

    return status;
}

int RenameStrategy::UpdateFolderMetaPost(const std::string& folderpath, Folder& folder) { 
    int status = ret::A_OK;
    FolderPost fp;
    status = RetrieveFolderPost(folder.folder_post_id(), fp);
    if(status == ret::A_OK) {
        std::cout<<" SETTING FOLDERPATH : " << folderpath << std::endl;
        folder.set_folderpath(folderpath);
        fp.set_folder(folder);

        Parent parent;
        parent.version = fp.version()->id();
        fp.PushBackParent(parent);
        
        std::string body;
        jsn::SerializeObject(&fp, body);

        std::string posturl;
        utils::FindAndReplace(post_path_, "{post}", folder.folder_post_id(), posturl);
        std::cout<<" POST URL : " << posturl << std::endl;
        Response resp;
        netlib::HttpPut(posturl,
                        fp.type(),
                        NULL,
                        body,
                        &access_token_,
                        resp);

        if(resp.code == 200) {

        }
        else {
            status = ret::A_FAIL_NON_200;
        }
        std::cout<<" code : " << resp.code << std::endl;
        std::cout<<" body : " << resp.body << std::endl;
    }

    return status;
}

int RenameStrategy::UpdateFileMetaPost(const std::string& post_id, const FilePost& fp) {
    int status = ret::A_OK;
    std::string body;
    FilePost p = fp;
    jsn::SerializeObject(&p, body);

    std::string posturl;
    utils::FindAndReplace(post_path_, "{post}", post_id, posturl);
    std::cout<<" POST URL : " << posturl << std::endl;
    Response resp;
    netlib::HttpPut(posturl,
                    fp.type(),
                    NULL,
                    body,
                    &access_token_,
                    resp);

    if(resp.code == 200) {

    }
    else {
        status = ret::A_FAIL_NON_200;
    }
    std::cout<<" code : " << resp.code << std::endl;
    std::cout<<" body : " << resp.body << std::endl;

    return status;
}

int RenameStrategy::UpdateFileMetaPost(const std::string& post_id, 
                                       const std::string& new_filepath,
                                       const std::string& new_relative_path) {
    int status = ret::A_OK;
    FilePost fp;
    status = RetrieveFilePost(post_id, fp);
    if(status == ret::A_OK) {
        std::cout<<" FILENAME : " << new_filepath << std::endl;
        fp.PushBackAlias(fp.relative_path()); // Push back old path
        fp.set_relative_path(new_relative_path);
        fp.set_name(new_filepath);

        Parent parent;
        parent.version = fp.version()->id();
        fp.PushBackParent(parent);
        
        std::string body;
        jsn::SerializeObject(&fp, body);

        std::string posturl;
        utils::FindAndReplace(post_path_, "{post}", post_id, posturl);
        std::cout<<" POST URL : " << posturl << std::endl;
        Response resp;
        netlib::HttpPut(posturl,
                        fp.type(),
                        NULL,
                        body,
                        &access_token_,
                        resp);

        if(resp.code == 200) {

        }
        else {
            status = ret::A_FAIL_NON_200;
        }
        std::cout<<" code : " << resp.code << std::endl;
        std::cout<<" body : " << resp.body << std::endl;
    }
    return status;
}

int RenameStrategy::RetrieveFilePost(const std::string& post_id, FilePost& fp) {
    int status = ret::A_OK;
    std::string posturl;
    utils::FindAndReplace(post_path_, "{post}", post_id, posturl);
    std::cout<<" POST URL : " << posturl << std::endl;

    Response response;
    netlib::HttpGet(posturl,
                    NULL,
                    &access_token_,
                    response);
    if(response.code == 200) {
        jsn::DeserializeObject(&fp, response.body);
    }
    else {
        status = ret::A_FAIL_NON_200;
    }

    return status;
}

FileInfo* RenameStrategy::RetrieveFileInfo(const std::string& filepath) {
    FileInfo* fi = file_manager_->GetFileInfo(filepath);
    if(!fi)
        fi = file_manager_->CreateFileInfo();
    return fi;
}


} //namespace

