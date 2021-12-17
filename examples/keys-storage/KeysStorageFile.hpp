// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_STORAGE_FILE_HPP
#define KEYS_STORAGE_FILE_HPP

#include <stdbool.h>
#include <time.h>

#include <verificaC19-sdk/interfaces/IKeysStorage.hpp>

#ifdef __cplusplus

#include <json/json.h>

#include <string>

namespace verificaC19Sdk {

class KeysStorageFile : public IKeysStorage {

public:

	KeysStorageFile();
	std::string getKey(const std::string& kid) const;
	void beginUpdatingKeys();
	void storeKey(const std::string& kid, const std::string& ecx);
	void commitUpdatedKeys();
	void rollbackUpdatedKeys();
	bool isUpdating() { return m_updatingMode; }
	time_t lastUpdate() { return time(NULL) - m_keys["_lastUpdate"].asInt(); }
	void setLastStoredKeyToken(std::string& token);
	std::string getLastStoredKeyToken() const { return m_keys["_lastToken"].asString(); };

private:

	bool m_updatingMode;
	Json::Value m_keys;
	Json::Value m_updatingKeys;

};

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

void* KeysStorageFile_c_create();
typedef void* (*pfKeysStorageFile_c_create)();

void KeysStorageFile_c_release(const void* keysStorageMemory);
typedef void (*pfKeysStorageFile_c_release)(const void*);

const char* KeysStorageFile_c_getKey(const void* keysStorageMemory, const char* kid);
typedef const char* (*pfKeysStorageFile_c_getKey)(const void*, const char*);

void KeysStorageFile_c_beginUpdatingKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageFile_c_beginUpdatingKeys)(const void*);

void KeysStorageFile_c_storeKey(const void* keysStorageMemory, const char* kid, const char* ecx);
typedef void (*pfKeysStorageFile_c_storeKey)(const void*, const char*, const char*);

void KeysStorageFile_c_commitUpdatedKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageFile_c_commitUpdatedKeys)(const void*);

void KeysStorageFile_c_rollbackUpdatedKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageFile_c_rollbackUpdatedKeys)(const void*);

bool KeysStorageFile_c_isUpdating(const void* keysStorageMemory);
typedef bool (*pfKeysStorageFile_c_isUpdating)(const void*);

time_t KeysStorageFile_c_lastUpdate(const void* keysStorageMemory);
typedef time_t (*pfKeysStorageFile_c_lastUpdate)(const void*);

void KeysStorageFile_c_setLastStoredKeyToken(const void* keysStorageMemory, const char* token);
typedef void (*pfKeysStorageFile_c_setLastStoredKeyToken)(const void*, const char*);

const char* KeysStorageFile_c_getLastStoredKeyToken(const void* keysStorageMemory);
typedef const char* (*pfKeysStorageFile_c_getLastStoredKeyToken)(const void*);

#ifdef __cplusplus
}
#endif

#endif // #ifndef KEYS_STORAGE_FILE_HPP
