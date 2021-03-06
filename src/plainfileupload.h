#ifndef PLAINFILEUPLOAD_H_
#define PLAINFILEUPLOAD_H_
#pragma once

#include <map>
#include <string>
#include "connection.h"
#include "accesstoken.h"
#include "downloadpost.h"

namespace attic {


class PlainFileUpload {
    void LoadMimeTypes();
    void DetermineContentType(const std::string& filepath, std::string& out);
    bool BeginRequest();
    bool UploadPostBody(DownloadPost& out);
    bool UploadFile(const std::string& filepath,
                    const std::string& filename,
                    const std::string& filesize);
    bool EndRequest();
    bool InterpretResponse(DownloadPost& out);

    bool WriteFileToConnection(const std::string& filepath);
    bool GenerateDownloadPost(const std::string& filepath, DownloadPost& out);
    bool Push(boost::asio::streambuf& request);
public:
    PlainFileUpload(const AccessToken& at, 
                    const std::string& temp_dir);
    ~PlainFileUpload();

    bool Upload(const std::string& url, 
                const std::string& filepath,
                DownloadPost& out);

private:
    std::string temp_dir_;
    // key, extension, value mimetype
    typedef std::map<std::string, std::string> MimeMap;
    MimeMap mime_map_;
    AccessToken access_token_;
    std::string url_;
    std::string boundary_;

    std::string filepath_;

    boost::asio::io_service io_service_;
    Connection* con_;
};

}//namespace 

#endif

