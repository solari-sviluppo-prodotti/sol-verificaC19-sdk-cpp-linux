// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_STORAGE_MEMORY_HPP
#define KEYS_STORAGE_MEMORY_HPP

#include <stdbool.h>
#include <time.h>

#include <verificaC19-sdk/interfaces/IKeysStorage.hpp>

#ifdef __cplusplus

#include <string>
#include <map>

namespace verificaC19Sdk {

class KeysStorageMemory : public IKeysStorage {

public:

	KeysStorageMemory() : m_lastUpdate(0), m_updatingMode(false) {}
	std::string getKey(const std::string& kid) const;
	void beginUpdatingKeys();
	void storeKey(const std::string& kid, const std::string& ecx);
	void commitUpdatedKeys();
	void rollbackUpdatedKeys();
	bool isUpdating() { return m_updatingMode; }
	time_t lastUpdate() { return time(NULL) - m_lastUpdate; }
	void setLastStoredKeyToken(std::string& token) { m_lastToken = token; };
	std::string getLastStoredKeyToken() const { return m_lastToken; };

private:

	time_t m_lastUpdate;
	bool m_updatingMode;
	std::map<std::string, std::string> m_keys;
	std::map<std::string, std::string> m_updatingKeys;
	std::string m_lastToken;

};

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

void* KeysStorageMemory_c_create();
typedef void* (*pfKeysStorageMemory_c_create)();

void KeysStorageMemory_c_release(const void* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_release)(const void*);

const char* KeysStorageMemory_c_getKey(const void* keysStorageMemory, const char* kid);
typedef const char* (*pfKeysStorageMemory_c_getKey)(const void*, const char*);

void KeysStorageMemory_c_beginUpdatingKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_beginUpdatingKeys)(const void*);

void KeysStorageMemory_c_storeKey(const void* keysStorageMemory, const char* kid, const char* ecx);
typedef void (*pfKeysStorageMemory_c_storeKey)(const void*, const char*, const char*);

void KeysStorageMemory_c_commitUpdatedKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_commitUpdatedKeys)(const void*);

void KeysStorageMemory_c_rollbackUpdatedKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_rollbackUpdatedKeys)(const void*);

bool KeysStorageMemory_c_isUpdating(const void* keysStorageMemory);
typedef bool (*pfKeysStorageMemory_c_isUpdating)(const void*);

time_t KeysStorageMemory_c_lastUpdate(const void* keysStorageMemory);
typedef time_t (*pfKeysStorageMemory_c_lastUpdate)(const void*);

void KeysStorageMemory_c_setLastStoredKeyToken(const void* keysStorageMemory, const char* token);
typedef void (*pfKeysStorageMemory_c_setLastStoredKeyToken)(const void*, const char*);

const char* KeysStorageMemory_c_getLastStoredKeyToken(const void* keysStorageMemory);
typedef const char* (*pfKeysStorageMemory_c_getLastStoredKeyToken)(const void*);

#ifdef __cplusplus
}
#endif

#endif // #ifndef KEYS_STORAGE_MEMORY_HPP
