// Manifest file structure
// Within the manifest file itself the structure of data to read in
// is as follows. Line by line
//  Manifest specific: entry count, 
//      entry : file name, file path, chunk name, chunk count, file size
//      entry : ...

#ifndef MANIFEST_H_
#define MANIFEST_H_
#pragma once

#include <fstream>
#include <string>
#include <map>

class FileInfo;

class Manifest
{

public:
    Manifest();
    ~Manifest();

    bool LoadManifestFile(std::string &szFilePath);
    bool WriteOutManifest();    

private:
    std::map<std::string, FileInfo*>    m_entries;  

    std::ifstream       m_ifStream;
    std::ofstream       m_ofStream;

    // Manifest specific data
    std::string         m_filePath;     // path to manifest file
    std::string         m_fileName;     // filename of the manifest
    unsigned int        m_entryCount;   // Number of entries in the manifest
};

#endif

