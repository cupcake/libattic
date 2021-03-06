#ifndef CONFIGHANDLER_H_
#define CONFIGHANDLER_H_
#pragma once

#include <string>
#include "filemanager.h"
#include "entity.h"
#include "configpost.h"
#include "folder.h"

namespace attic { 

class ConfigHandler {
public:
    ConfigHandler(FileManager* fm);
    ~ConfigHandler();

    bool CreateConfigPost(const Entity& ent, const AccessToken* at, ConfigPost& out);
    bool UpdateConfigPost(const Entity& ent, const AccessToken* at, ConfigPost& post);
    bool RetrieveConfigPost(const Entity& ent, const AccessToken* at, ConfigPost& out);
    void LoadConfigPost(ConfigPost& in);
    int GetConfigPostCount(const Entity& ent, const AccessToken* at);

    int CreateWorkingDirectory(const std::string& filepath,
                               const Entity& ent, 
                               const AccessToken* at);


    int AddRootDirToConfigPost(const std::string& alias, 
                               const std::string& postid,
                               const Entity& ent, 
                               const AccessToken* at);
                

    int CreateFolderPost(Folder& folder, 
                         const Entity& ent, 
                         const AccessToken* at,
                         std::string& id_out);

    bool LoadIntoFirstDirectory(const std::string& folderpath); // Temporary One off till there is
                                                                // a need to, or interface for 
                                                                // interactive directory mapping.
                                                                // TODO :: that.

    bool MapDirectoryToRoot(const std::string& key, const std::string& path);
    bool CheckForUnmappedRootDirectories(std::deque<std::string>& key_out);

private:
    FileManager* file_manager_;

};

} // namespace

#endif

