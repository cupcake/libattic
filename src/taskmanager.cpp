#include "taskmanager.h"

#include "taskarbiter.h"
#include "tentapp.h"
#include "credentialsmanager.h"
#include "filemanager.h"


TaskManager::TaskManager( TentApp* pApp, 
                          FileManager* pFm, 
                          CredentialsManager* pCm,
                          const AccessToken& at,
                          const Entity& entity,
                          const std::string& tempdir, 
                          const std::string& workingdir,
                          const std::string& configdir
                        )
{
    m_pApp = pApp;
    m_pFileManager = pFm;
    m_pCredentialsManager = pCm;

    m_AccessToken = at;
    m_Entity = entity;

    m_TempDir = tempdir;
    m_WorkingDir = workingdir;
    m_ConfigDir = configdir;
}

TaskManager::~TaskManager()
{

}

int TaskManager::Initialize()
{
    int status = ret::A_OK;
    status = m_TaskFactory.Initialize();

    EventSystem::GetInstance()->RegisterForEvent(this, Event::REQUEST_PULL);
    EventSystem::GetInstance()->RegisterForEvent(this, Event::REQUEST_PUSH);
    EventSystem::GetInstance()->RegisterForEvent(this, Event::REQUEST_DELETE);
    EventSystem::GetInstance()->RegisterForEvent(this, Event::REQUEST_SYNC_POST);

    return status;
}

int TaskManager::Shutdown()
{
    int status = ret::A_OK;
    status = m_TaskFactory.Shutdown();

    return status;
}

void TaskManager::OnEventRaised(const Event& event)
{
    std::cout<<" TASK MANAGER EVENT RAISED " << std::endl;
    switch(event.type) {
        case Event::REQUEST_PULL:
            {
                DownloadFile(event.value, event.callback);
                break;
            }
        case Event::REQUEST_PUSH:
            {
                UploadFile(event.value, event.callback);
                break;
            }
        case Event::REQUEST_DELETE:
            {
                DeleteFile(event.value, event.callback);
                break;
            }
        case Event::REQUEST_SYNC_POST:
            {
                std::cout<<" creating request sync task " << std::endl;
                SyncFile(event.value, event.callback);
                break;
            }
        default:
            std::cout<<"received unknown event"<<std::endl;
    }

}

void TaskManager::OnTaskCreate(Task* t)
{
    std::cout<<" On Task Create " << std::endl;
    // Get Task type
    // if pull or push task
    // extract curl instance ptr
}

void TaskManager::OnTaskInsert(Task* t)
{
    std::cout<<" On Task Insert " << std::endl;

    // Maybe move spin off task to spin off?
    //status = TaskArbiter::GetInstance()->SpinOffTask(t);
}
int TaskManager::SyncFile(const std::string& postid, void (*callback)(int, void*))
{
    return CreateAndSpinOffTask( Task::SYNC_FILE_TASK,
                                 postid,
                                 callback);
}

int TaskManager::CreateAndSpinOffTask( Task::TaskType tasktype, 
                                       const std::string& filepath, 
                                       void (*callback)(int, void*))
{
    int status = ret::A_OK;

    Task* t = m_TaskFactory.GetTentTask( tasktype,
                                         m_pApp,
                                         m_pFileManager,
                                         m_pCredentialsManager,
                                         TaskArbiter::GetInstance(),
                                         &m_TaskFactory,
                                         m_AccessToken,
                                         m_Entity,
                                         filepath,
                                         m_TempDir,
                                         m_WorkingDir,
                                         m_ConfigDir,
                                         callback,
                                         this);

    status = TaskArbiter::GetInstance()->SpinOffTask(t);
    return status;

}

int TaskManager::UploadFile(const std::string& filepath, void (*callback)(int, void*)) {
    return CreateAndSpinOffTask( Task::PUSH, filepath, callback);
}

int TaskManager::DownloadFile(const std::string& filepath, void (*callback)(int, void*)) {
    return CreateAndSpinOffTask( Task::PULL, filepath, callback);
}

int TaskManager::DeleteFile(const std::string& filepath, void (*callback)(int, void*)) {
    return CreateAndSpinOffTask( Task::DELETE, filepath, callback);
}

int TaskManager::DownloadAllFiles(void (*callback)(int, void*)) {
    return CreateAndSpinOffTask(Task::PULLALL, "", callback);
}

int TaskManager::SyncFiles(void (*callback)(int, void*)) {
    return CreateAndSpinOffTask(Task::SYNC, "", callback);
}

int TaskManager::PollFiles(void (*callback)(int, void*)) {
    return CreateAndSpinOffTask( Task::POLL, "", callback);
}

int TaskManager::DeleteAllPosts(void (*callback)(int, void*)) {
    return CreateAndSpinOffTask(Task::DELETEALLPOSTS, "", callback);
}

int TaskManager::QueryManifest(void(*callback)(int, char**, int, int))
{
    int status = ret::A_OK;

    Task* t = m_TaskFactory.GetManifestTask( Task::QUERYMANIFEST,
                                             m_pFileManager,
                                             callback,
                                             this);

    status = TaskArbiter::GetInstance()->SpinOffTask(t);
    return status;
}

int TaskManager::TaskCount(const Task::TaskType type)
{
    return m_TaskFactory.GetNumberOfActiveTasks(type);
}

