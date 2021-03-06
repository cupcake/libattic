#ifndef THREADING_H_
#define THREADING_H_
#pragma once

#include <iostream>
#include <vector>
#include <boost/thread/thread.hpp>

#include "mutexclass.h"
#include "filemanager.h"
#include "credentialsmanager.h"
#include "entity.h"
#include "accesstoken.h"

namespace attic {

class ThreadWorker;

class ThreadPool{
public:                                                                                          
    ThreadPool();
    ~ThreadPool();

    int Initialize();
    int Shutdown();

    void SpinOffWorker(ThreadWorker* worker);

    unsigned WorkerCount() { return workers_.size(); }
private:                                                                                         
    std::vector<ThreadWorker*> workers_;

    unsigned int thread_count_;                                                                  
};                                                                                               

class ThreadManager {
    int ExtendPool(unsigned int stride);                                                        
public:
    ThreadManager(FileManager* fm,
                  CredentialsManager* cm,
                  const AccessToken& at,
                  const Entity& ent);
    ~ThreadManager();

    int Initialize(unsigned int pool_size);
    int Shutdown();

private:
    MutexClass  pool_mtx_; // This may not even be needed, is where is the possible contention?
    ThreadPool* thread_pool_;

    FileManager*            file_manager_;
    CredentialsManager*     credentials_manager_;
    AccessToken             access_token_;
    Entity                  entity_;
};


}//namespace
#endif

