
#ifndef PROFILE_H_
#define PROFILE_H_
#pragma once

#include <vector>
#include <string>

#include "jsonserializable.h"
#include "permissions.h"

class AtticProfileInfo : public JsonSerializable
{
public:
    AtticProfileInfo();
    ~AtticProfileInfo();

    void Serialize(Json::Value& root);
    void Deserialize(Json::Value& root);

    void GetSalt(std::string& out) const { out = m_Salt; }
    void GetMasterKey(std::string& out) const { out = m_MasterKey; }
    void GetIv(std::string& out) const { out = m_Iv; }

    void SetSalt(const std::string& salt) { m_Salt = salt; }
    void SetMasterKey(const std::string& mk) { m_MasterKey = mk; }
    void SetIv(const std::string& iv) { m_Iv = iv; }
    
private:
    Permissions m_Permissions;  // `json:"permissions"`

    std::string m_MasterKey;    // `json:"mk"`   // The encrypted Master Key
    std::string m_Iv;           // `json:"mk_iv"` //The IV used to encrypt the master key
    std::string m_Salt;         // `json:"salt"` // The encrypted salt

};

class CoreProfileInfo : public JsonSerializable
{
public:
    CoreProfileInfo();
    ~CoreProfileInfo();

    void Serialize(Json::Value& root);
    void Deserialize(Json::Value& root);

private:
    typedef std::vector<std::string> ServerList;
    typedef std::vector<std::string> LicenseList;

    Permissions m_Permissions; // `json:"permissions"
    std::string m_Entity;     //`json:"entity"`   //The canonical entity identitifier.
    ServerList  m_Licenses;   //`json:"licenses"` //The licenses the entity publishes content under.
    LicenseList m_Servers;    //`json:"servers"`  //The canonical API roots that can be used to interact with the entity.
};

class BasicProfileInfo : public JsonSerializable
{
public:
    BasicProfileInfo();
    ~BasicProfileInfo();

    void Serialize(Json::Value& root);
    void Deserialize(Json::Value& root);

private:
    Permissions     m_Permissions;   //`json:"permissions"
    std::string     m_Name;          //`json:"name"`       // Name to be displayed publicly.
    std::string     m_AvatarUrl;     //`json:"avatar_url"` // URL to avatar to be displayed publicly.
    std::string     m_Birthdate;     //`json:"birthdate"`  // Date of birth in one of these formats: YYYY-MM-DD, YYYY-MM-DD
    std::string     m_Location;      //`json:"location"`   // Location to be displayed publicly.
    std::string     m_Gender;        //`json:"gender"`     // Gender to be displayed publicly.
    std::string     m_Bio;           //`json:"bio"`        // Biography/self-description to be displayed publicly.
};

class Profile : public JsonSerializable
{
public:
    Profile();
    ~Profile();

    void Serialize(Json::Value& root);
    void Deserialize(Json::Value& root);
    
    AtticProfileInfo* GetAtticInfo() const { return m_pAtticInfo; }
    CoreProfileInfo* GetCoreInfo() const { return m_pCoreInfo; }
    BasicProfileInfo* GetBasicInfo() const { return m_pBasicInfo; }

    void SetAtticInfo(AtticProfileInfo* pInfo) { m_pAtticInfo = pInfo; }
    void SetCoreProfileInfo(CoreProfileInfo* pInfo) { m_pCoreInfo = pInfo; }
    void SetBasicProfileInfo(BasicProfileInfo* pInfo) { m_pBasicInfo = pInfo; }

private:
    AtticProfileInfo*    m_pAtticInfo;
    CoreProfileInfo*     m_pCoreInfo;
    BasicProfileInfo*    m_pBasicInfo;
};

#endif
