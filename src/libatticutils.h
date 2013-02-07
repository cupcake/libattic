
#ifndef LIBATTICUTILS_H_
#define LIBATTICUTILS_H_
#pragma once

#include <string>

#include "utils.h"

#include "constants.h"
#include "entitymanager.h"
#include "connectionmanager.h"
#include "credentialsmanager.h"
#include "filemanager.h"
#include "taskarbiter.h"
#include "uploadmanager.h"
#include "log.h"
//#include "entity.h"

// Inward facing utility methods used at libattic interface level

namespace liba
{
    int InitializeFileManager( FileManager** pFm, 
                               const std::string& manifestName,
                               const std::string& configDir, 
                               const std::string& tempDir)
    {
        int status = ret::A_OK;

        if(!(*pFm))
        {
            // Construct path
            std::string filepath(configDir);
            utils::CheckUrlAndAppendTrailingSlash(filepath);
            filepath.append(manifestName);

                (*pFm) = new FileManager(filepath, configDir);
                (*pFm)->SetTempDirectory(tempDir);

                if(!(*pFm)->StartupFileManager())
                {
                    status = ret::A_FAIL_TO_LOAD_FILE;
                }
        }
        else
            status = ret::A_FAIL_ATTEMPT_TO_REINIT;

        if(status != ret::A_OK) 
            alog::Log(Logger::ERROR, "Failed to initialize filemanager");

        return status;
    }

    int InitializeCredentialsManager( CredentialsManager** pCm,
                                      const std::string& configDir)
    {
        int status = ret::A_OK;

        if(!(*pCm))
        {
            (*pCm) = new CredentialsManager();
            (*pCm)->SetConfigDirectory(configDir);
            status = (*pCm)->Initialize();
        }
        else
            status = ret::A_FAIL_ATTEMPT_TO_REINIT;

        if(status != ret::A_OK)
            alog::Log(Logger::ERROR, "Failed to initialize credential manager");

        return status;
    }

    int InitializeEntityManager( EntityManager** pEm )
    {
        int status = ret::A_OK;
        if(!(*pEm))
        {
            (*pEm) = new EntityManager();
            status = (*pEm)->Initialize();
        }
        else
            status = ret::A_FAIL_ATTEMPT_TO_REINIT;

        if(status != ret::A_OK)
            alog::Log(Logger::ERROR, "Failed to initialize entity manager");

        return status;
    }

    int InitializeUploadManager( UploadManager** pUm,
                                 TentApp* pApp,
                                 FileManager* pFm,
                                 CredentialsManager* pCm,
                                 const AccessToken& at,
                                 const Entity& entity,
                                 const std::string& tempdir,
                                 const std::string& workingdir,
                                 const std::string& configdir)
    {
        int status = ret::A_OK;
        if(!(*pUm))
        {
            (*pUm) = new UploadManager(pApp,
                                        pFm,
                                        pCm,
                                        at,
                                        entity,
                                        tempdir,
                                        workingdir,
                                        configdir);

            status = (*pUm)->Initialize();
        }
        else
            status = ret::A_FAIL_ATTEMPT_TO_REINIT;

        if(status != ret::A_OK)
            alog::Log(Logger::ERROR, "Failed to initialize Upload Manager");

        return status;
    }

    int InitializeTaskArbiter(const unsigned int threadCount)
    {
        int status = ret::A_OK;
        status = TaskArbiter::GetInstance()->Initialize(threadCount);
        if(status != ret::A_OK)
            alog::Log(Logger::ERROR, "Failed to initialize task arbiter");

        return status;
    }

    int InitializeConnectionManager()
    {
        int status = ret::A_OK;
        status = ConnectionManager::GetInstance()->Initialize();
        if(status != ret::A_OK)
            alog::Log(Logger::ERROR, " failed to shutdown connection manager");

        return status;
    }


    int ShutdownTaskArbiter()
    {
        int status = ret::A_OK;
        status = TaskArbiter::GetInstance()->Shutdown();
        if(status != ret::A_OK)
            alog::Log(Logger::ERROR, "failed to shutdown task arbiter");

        return status;
    }

    int ShutdownFileManager( FileManager* pFm )
    {
        int status = ret::A_OK;
        // Blind shutdown
        if(pFm)
        {
            pFm->ShutdownFileManager();
            delete pFm;
            pFm = NULL;
        }
        else
            status = ret::A_FAIL_INVALID_FILEMANAGER_INSTANCE;

        if(status != ret::A_OK)
           alog::Log(Logger::ERROR, " failed to shutdown filemanger");

        return status;
    }

    int ShutdownCredentialsManager(CredentialsManager* pCm)
    {
        int status = ret::A_OK;
        if(pCm)
        {
            while(pCm->TryLock()) { sleep(0); }
            status = pCm->Shutdown();
            pCm->Unlock();

            delete pCm;
            pCm = NULL;
        }
        else
            status = ret::A_FAIL_INVALID_CREDENTIALSMANAGER_INSTANCE;

        if(status != ret::A_OK)
            alog::Log(Logger::ERROR, " failed to shutdown credentials manager");

        return status;
    }

    int ShutdownEntityManager(EntityManager* pEm)
    {
        int status = ret::A_OK;
        if(pEm)
        {
            while(pEm->TryLock()) { sleep(0); }
            status = pEm->Shutdown();
            pEm->Unlock();

            delete pEm;
            pEm = NULL;
        }
        else
            status = ret::A_FAIL_INVALID_ENTITYMANAGER_INSTANCE;

        if(status != ret::A_OK)
            alog::Log(Logger::ERROR, " failed to shutdown entity manager");

        return status;
    }

    int ShutdownAppInstance(TentApp* pApp)
    {
        int status = ret::A_OK;
        if(pApp)
        {
            delete pApp;
            pApp = NULL;
        }
        else
            status = ret::A_FAIL_INVALID_APP_INSTANCE;

        if(status != ret::A_OK)
            alog::Log(Logger::ERROR, " failed to shutdown app instance");

        return status;
    }

    int ShutdownConnectionManager()
    {
        int status = ret::A_OK;
        status = ConnectionManager::GetInstance()->Shutdown();

        if(status != ret::A_OK)
            alog::Log(Logger::ERROR, " failed to shutdown connection manager instance");

        return status;
    }

    int ShutdownUploadManager( UploadManager** pUm)
    {
        int status = ret::A_OK;
        if(!(*pUm))
            status = (*pUm)->Shutdown();
        else
            status = ret::A_FAIL_INVALID_UPLOADMANAGER_INSTANCE;

        if(status != ret::A_OK)
            alog::Log(Logger::ERROR, "Failed to Shutdown Upload Manager");

        return status;
    }

    int DeserializeIntoAccessToken(const std::string& body, AccessToken& out)
    {
        int status = ret::A_OK;
    
        if(!JsonSerializer::DeserializeObject(&out, body))
            status = ret::A_FAIL_TO_DESERIALIZE_OBJECT;          

        return status;
    }

    int WriteOutAccessToken(AccessToken& at, const std::string& outDir)
    {
        int status = ret::A_OK;
        std::string path = outDir;
        utils::CheckUrlAndAppendTrailingSlash(path);      
        path.append(cnst::g_szAuthTokenName);
        std::cout<<" OUT PATH : " << path << std::endl;
        status = at.SaveToFile(path);
        return status;
    }


}

#endif

