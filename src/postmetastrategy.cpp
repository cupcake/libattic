#include "postmetastrategy.h"

#include "filehandler.h"
#include "folderhandler.h"
#include "posthandler.h"

namespace attic {

int PostMetaStrategy::Execute(FileManager* fm, CredentialsManager* cm) {
    int status = ret::A_OK;
    status = InitInstance(fm, cm);

    post_path_ = GetConfigValue("post_path");
    posts_feed_ = GetConfigValue("posts_feed");
    std::string filepath = GetConfigValue("filepath");
    std::string entity = GetConfigValue("entity");

    std::cout<<" posting meta data strategy ... " << filepath << std::endl;

    if(ValidMasterKey()) {
        std::cout<<" valid master key " << std::endl;
        FileHandler fh(file_manager_);
        if(!fh.DoesFileExist(filepath)) {
            std::cout<<" does file exist ... yes " << std::endl;
            // File doesn't exist yet, create meta post
            FileInfo fi;
            status = CreateFileEntry(filepath, fi);
            std::cout<<" create file entry : " << status << std::endl;
            if (status == ret::A_OK)
                status = CreateFileMetaPost(filepath, fi);
        }
        else {
           // no nead to throw an error, postfilestrategy should dif the hashes 
           // and really determine if it should acutally be re-uploaded
           //
           // status = ret::A_FAIL_FILE_ALREADY_EXISTS;
        }
    }
    else {
        status = ret::A_FAIL_INVALID_MASTERKEY;
    }
    return status;
}

int PostMetaStrategy::CreateFileEntry(const std::string& filepath, FileInfo& out) {
    int status = ret::A_OK;
    std::string mk;
    GetMasterKey(mk);
    FileHandler fh(file_manager_);
    if(!fh.CreateNewFile(filepath, mk, out))
        status = ret::A_FAIL_CREATE_FILE_INFO;
    return status;
}

int PostMetaStrategy::CreateFileMetaPost(const std::string& filepath, FileInfo& fi) {
    int status = ret::A_OK;
    FilePost fp;
    fp.InitializeFilePost(&fi, false);
    PostHandler<FilePost> ph(access_token_);
    Response response;
    if(ph.Post(posts_feed_, NULL, fp, response) == ret::A_OK) {
        FilePost post;
        jsn::DeserializeObject(&post, response.body);
        fi.set_post_id(post.id());
        FileHandler fh(file_manager_);
        fh.UpdateFilePostId(fi.filepath(), post.id());
        std::ostringstream lg;
        // REMOVE
        std::string b64_fi_key, b64_fi_iv;
        std::string b64_key_post, b64_key_pre;
        crypto::Base64EncodeString(fp.key_data(), b64_key_pre);
        crypto::Base64EncodeString(post.key_data(), b64_key_post);
        crypto::Base64EncodeString(fi.file_credentials_key(), b64_fi_key);
        lg << " fp : " << filepath << std::endl;
        lg << " post (pre) : " << b64_key_pre << std::endl;
        lg << " post (post) : " << b64_key_post << std::endl;
        lg << " fi : " << b64_fi_key << std::endl;
        std::string b64_iv_post, b64_iv_pre;
        crypto::Base64EncodeString(fp.iv_data(), b64_iv_pre);
        crypto::Base64EncodeString(post.iv_data(), b64_iv_post);
        crypto::Base64EncodeString(fi.file_credentials_iv(), b64_fi_iv);
        lg << " iv (pre) : " << b64_iv_pre << std::endl;
        lg << " iv (post) : " << b64_iv_post << std::endl;
        lg << " fi : " << b64_fi_iv << std::endl;
        lg << response.body << std::endl;
        std::cerr << lg.str() << std::endl;
    }
    return status;
}

bool PostMetaStrategy::RetrieveFolderPostId(const std::string& filepath, std::string& id_out) {
    // Get folderpath
    std::cout<<" incoming filepath : " << filepath << std::endl;
    size_t pos = filepath.rfind("/");
    if(pos != std::string::npos) {
        std::string folderpath = filepath.substr(0, pos);
        utils::CheckUrlAndRemoveTrailingSlash(folderpath);
        FolderHandler fh(file_manager_);
        Folder folder;
        std::cout<<" getting folder : " << folderpath << std::endl;
        if(fh.GetFolder(folderpath, folder)){
            if(!folder.folder_post_id().empty()) {
                id_out = folder.folder_post_id();
                return true;
            }
        }
    }
    return false;
}

bool PostMetaStrategy::ValidMasterKey() {
    std::string mk;
    GetMasterKey(mk);
    if(mk.empty()) {
        std::string error = "Invalid master key, it is empty!";
        log::LogString("019288--1908-50", error);
        return false;
    }
    return true;
}

void PostMetaStrategy::GetMasterKey(std::string& out) {
    MasterKey mKey;
    credentials_manager_->GetMasterKeyCopy(mKey);
    mKey.GetMasterKey(out);
}

} // namespace

