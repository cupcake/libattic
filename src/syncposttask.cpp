
#include "syncposttask.h"

#include "atticpost.h"
#include "urlparams.h"
#include "constants.h"
#include "errorcodes.h"
#include "utils.h"

SyncPostsTask::SyncPostsTask( TentApp* pApp, 
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

SyncPostsTask::~SyncPostsTask()
{

}

void SyncPostsTask::RunTask()
{

    int status = SyncAtticPosts();

    Callback(status, NULL);
}

int SyncPostsTask::SyncAtticPosts()
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

    std::cout << " NUMBER OF POSTS : " << postcount << std::endl;

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
        AtticPost p;
        //JsonSerializer::DeserializeObject(&p, (*itr).asString());
        
        //std::cout<< "POST : " << (*itr).asString() << std::endl;

        // Deserialize directly into posts
        p.Deserialize(*itr);
        count++;

        std::cout<< "POST TYPE : " << std::endl;
        std::string posttype;
        p.GetPostType(posttype);
        std::cout<<posttype<<std::endl;
        
        // if proper post type
        if(posttype.compare(g_szAtticPostType) == 0)
        {


            while(GetFileManager()->TryLock()) { /* Spinlock, temporary */ sleep(0);}

            //FileInfo* fi = GetFileManager()->CreateFileInfo();

            /*
            std::string &filename,          
            std::string &filepath,          
            std::string &chunkName,         
            std::string &chunkCount,        
            std::string &fileSize,          
            std::string &postId,            
            std::string &postVersion,       
            std::string &key,               
            std::string &iv
            */

            //GetFileManager()->InsertToManifest(fi);
            
            // Do a comparison with sqlite db, perhaps prune on server side 


            GetFileManager()->Unlock();

            
            
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

int SyncPostsTask::GetAtticPostCount()                                                                  
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
