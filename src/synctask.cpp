
#include "synctask.h"

#include "urlparams.h"
#include "constants.h"
#include "errorcodes.h"
#include "utils.h"

SyncTask::SyncTask( TentApp* pApp, 
                    FileManager* pFm, 
                    ConnectionManager* pCon, 
                    const AccessToken& at,
                    const std::string& entity,
                    const std::string& filepath,
                    const std::string& tempdir, 
                    const std::string& workingdir,
                    const std::string& configdir,
                    void (*callback)(int, void*))
                    :
                    TentTask( pApp,
                              pFm,
                              pCon,
                              at,
                              entity,
                              filepath,
                              tempdir,
                              workingdir,
                              configdir,
                              callback)
{

}

SyncTask::~SyncTask()
{

}

void SyncTask::RunTask()
{

    int status = SyncAtticPosts();

    Callback(status, NULL);
}

int SyncTask::SyncAtticPosts()
{
    // TODO :: this needs to be re-done.
    //
    //This should be renamed to sync Attic Manifest or SyncAtticMetaData
    // 1. Head request all posts of type attic
    // 2. Build FileInfo Objects for each
    // 3. Insert into Sqlite
    //
    // Create a new method PullAllFiles for the actual downloading of the files.
    // - this may already exist and need updating
    
    if(!GetTentApp())
        return ret::A_LIB_FAIL_INVALID_APP_INSTANCE;
    
    if(!GetFileManager())
        return ret::A_LIB_FAIL_INVALID_FILEMANAGER_INSTANCE;

    int postcount = GetAtticPostCount();

    if(postcount <= 0)
        return ret::A_FAIL_COULD_NOT_FIND_POSTS;

    std::string url; 
    GetEntity(url);
    url += "/tent/posts";

    UrlParams params;
    params.AddValue(std::string("post_types"), std::string(g_szAtticPostType));
    params.AddValue(std::string("limit"), std::string("200"));

    AccessToken* at = GetAccessToken();
    std::string response;
    ConnectionManager::GetInstance()->HttpGetWithAuth( url, 
                                                       &params,
                                                       response, 
                                                       at->GetMacAlgorithm(), 
                                                       at->GetAccessToken(), 
                                                       at->GetMacKey(), 
                                                       true);

    std::cout<< " RESPONSE : " << response << std::endl;

    Json::Value root;
    Json::Reader reader;

    if(!reader.parse(response, root))
        return -1;


    Json::ValueIterator itr = root.begin();

    int count = 0;
    for(;itr != root.end(); itr++)
    {
        Post p;
        //JsonSerializer::DeserializeObject(&p, (*itr).asString());
        
        // Deserialize directly into posts
        p.Deserialize(*itr);
        count++;
        std::cout<<p.GetPostType()<<std::endl;

        // if proper post type
        if(p.GetPostType().compare(g_szAtticPostType) == 0 && p.GetAttachmentCount() > 0)
        {
            // Check Attachment
            Post::AttachmentVec *pVec = p.GetAttachments();
            Post::AttachmentVec::iterator itr = pVec->begin();

            for(;itr != pVec->end(); itr++)
            {
                Attachment* pAtt = (*itr);
                if(pAtt)
                {
                    // Populate Manifest
                    if(!GetFileManager()->FindFileInManifest(pAtt->Name))
                    {

                        std::string path;
                        GetWorkingDirectory(path);
                        utils::CheckUrlAndAppendTrailingSlash(path);
                        path += pAtt->Name;

                        
                        char szLen[256];
                        memset(szLen, 0, sizeof(char)*256);                        
                        snprintf(szLen, (sizeof(char)*256),  "%u", pAtt->Size);



                       // TODO:: reimplement syncing with proper key stores
                       /* 
                        FileInfo* fi = GetFileManager()->CreateFileInfo( pAtt->Name,
                                                                       path,
                                                                       "",
                                                                       "0",
                                                                       szLen,
                                                                       p.GetID(),
                                                                       "0");

                        GetFileManager()->InsertToManifest(fi);
                        */

                    }
                }
            }
        }
    }

    std::cout<< " COUNT : " << count << std::endl;



    if(postcount > 200)
    {
               // Loop through and gather posts

    }

//    PullAllFiles();

    return ret::A_OK;
};

int SyncTask::GetAtticPostCount()                                                                  
{                                                                                        
    std::string url;
    GetEntity(url);

    //TODO :: make this provider agnostic                                               
    url += "/tent/posts/count";                                                          

    UrlParams params;                                                                    
    params.AddValue(std::string("post_types"), std::string(g_szAtticPostType));          

    AccessToken* at = GetAccessToken();
    std::string response;                                                                
    ConnectionManager::GetInstance()->HttpGetWithAuth( url,                              
                                                       &params,                          
                                                       response,                         
                                                       at->GetMacAlgorithm(),           
                                                       at->GetAccessToken(),            
                                                       at->GetMacKey(),                 
                                                       true);                            

    std::cout<< "RESPONSE : " << response << std::endl;                                  

    int count = -1;                                                                      
    count = atoi(response.c_str());                                                      

    return count;                                                                        
}                                                                                        
