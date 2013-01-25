
#ifndef PULLALLTASK_H_
#define PULLALLTASK_H_
#pragma once

#include "tenttask.h"


class PullAllTask : public TentTask
{
public:
    PullAllTask( TentApp* pApp, 
                 FileManager* pFm, 
                 CredentialsManager* pCm,
                 TaskArbiter* pTa,
                 TaskFactory* pTf,
                 const AccessToken& at,
                 const std::string& entity,
                 const std::string& filepath,
                 const std::string& tempdir, 
                 const std::string& workingdir,
                 const std::string& configdir,
                 void (*callback)(int, void*));

    ~PullAllTask();

    void RunTask();

    void PullAllCb(int a, void* b);

private:
    unsigned int m_CallbackCount;
    unsigned int m_CallbackHit;

};

#endif
