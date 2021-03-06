#ifndef ERROR_CODES_H_
#define ERROR_CODES_H_
#pragma once

namespace attic { namespace ret {

enum eCode {
    A_OK = 0,
    A_FAIL,
    A_FAIL_OPEN_FILE,
    A_FAIL_ENCRYPT,
    A_FAIL_DECRYPT,
    A_FAIL_HMAC, // 5
    A_FAIL_HMAC_VERIFY,
    A_FAIL_COMPRESS,   
    A_FAIL_DECOMPRESS,
    A_FAIL_VERIFY_CHUNKS,
    A_FAIL_WRITE_CHUNK, // 10 
    A_FAIL_TO_SERIALIZE_OBJECT, 
    A_FAIL_TO_DESERIALIZE_OBJECT,
    A_FAIL_INVALID_CSTR,
    A_FAIL_TO_LOAD_FILE,
    A_FAIL_TO_WRITE_OUT_MANIFEST, // 15
    A_FAIL_FILE_NOT_IN_MANIFEST,
    A_FAIL_TO_QUERY_MANIFEST,
    A_FAIL_TO_LOAD_MANIFEST,
    A_FAIL_TO_CLOSE_MANIFEST,
    A_FAIL_COULD_NOT_FIND_POSTS,// 20
    A_FAIL_JSON_PARSE,
    A_FAIL_CURL_PERF, 
    A_FAIL_SCRYPT_INVALID_SALT_SIZE,
    A_FAIL_EMPTY_PASSPHRASE,
    A_FAIL_REGISTER_PASSPHRASE,// 25
    A_FAIL_ENTITY_ALREADY_LOADED,
    A_FAIL_INVALID_PHRASE_TOKEN, 
    A_FAIL_SENTINEL_MISMATCH,
    A_FAIL_NEED_ENTER_PASSPHRASE,
    A_FAIL_NON_200,// 30
    A_FAIL_INVALID_MASTERKEY,
    A_FAIL_KEYSIZE_MISMATCH, 
    A_FAIL_IVSIZE_MISMATCH,
    A_FAIL_INVALID_APP_INSTANCE,
    A_FAIL_INVALID_FILEMANAGER_INSTANCE, // 35
    A_FAIL_INVALID_CONNECTIONMANAGER_INSTANCE,
    A_FAIL_INVALID_CREDENTIALSMANAGER_INSTANCE, 
    A_FAIL_INVALID_ENTITYMANAGER_INSTANCE,
    A_FAIL_INVALID_UPLOADMANAGER_INSTANCE,
    A_FAIL_ATTEMPT_TO_REINIT, // 40
    A_FAIL_DUPLICATE_ENTRY,
    A_FAIL_RUNNING_SINGLE_INSTANCE,
    A_FAIL_CREATE_THREAD, 
    A_FAIL_NO_CREDENTIALS,
    A_FAIL_INVALID_PTR, // 45// to be used sparingly as a placeholder please
    A_FAIL_ATTEMPT_DOUBLE_FREE, 
    A_FAIL_HEX_ENCODE, 
    A_FAIL_HEX_DECODE, 
    A_FAIL_INVALID_FILE_KEY,
    A_FAIL_LOAD_APP_DATA, // 50
    A_FAIL_TIMED_OUT,
    A_FAIL_LIB_INIT, 
    A_FAIL_SUBSYSTEM_NOT_INITIALIZED,
    A_FAIL_EMPTY_STRING,
    A_FAIL_TCP_ENDPOINT_NOT_FOUND, // 55
    A_FAIL_SSL_HANDSHAKE,
    A_FAIL_HTTP_RESPONSE, 
    A_FAIL_EMPTY_ATTACHMENTS, 
    A_FAIL_EXCEPTION,
    A_FAIL_INVALID_POST_ID, // 60
    A_FAIL_PATH_DOESNT_EXIST,
    A_FAIL_FS_ERROR,
    A_FAIL_CREATE_DIRECTORY,
    A_FAIL_INVALID_PROFILE,
    A_FAIL_PULL_DELETED_FILE, // 65
    A_FAIL_INVALID_CHUNK_HASH,
    A_FAIL_INVALID_FILE_INFO,
    A_FAIL_INVALID_CLIENT,
    A_FAIL_INVALID_IOSERVICE,
    A_FAIL_SOCKET_WRITE, // 70
    A_FAIL_ZERO_SIZE,
    A_FAIL_EMPTY_CHUNK_POST,
    A_FAIL_INVALID_FILEPATH,
    A_FAIL_INVALID_FOLDERPATH,
    A_FAIL_CHUNK_TRANSFORM, // 75
    A_FAIL_INVALID_FOLDER_POST,
    A_FAIL_UPDATE_MANIFEST,
    A_FAIL_ATTACHMENT_VERIFICATION,
    A_FAIL_FILE_IN_USE,
    A_FAIL_FOLDER_NOT_IN_MANIFEST, // 80
    A_FAIL_VALIDATE_DIRECTORY,
    A_FAIL_SERVICE_NOT_RUNNING,
    A_FAIL_PULL_DELETED_FOLDER,
    A_FAIL_FILE_ALREADY_EXISTS,
    A_FAIL_CREATE_FILE_INFO, // 85
    A_FAIL_ZERO_CHUNK_COUNT,
    A_FAIL_MISSING_CHUNK,
    A_FAIL_DUPLICATE_UPLOAD_ATTEMPT,
    A_FAIL_INVALID_IMPL,
    A_FAIL_NULL_SOCKET,
    A_FAIL_MOVE_PATH,
    A_FAIL_DIFFERING_WORK_DIRECTORY,
    A_FAIL_RETRIEVE_PARENT_PATH,
    A_FAIL_RENAME_FILE,
    A_FAIL_RENAME_FOLDER,
    A_FAIL_OTHER     
};


}} //namespace
#endif

