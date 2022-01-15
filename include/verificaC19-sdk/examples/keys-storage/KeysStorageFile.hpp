// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_STORAGE_FILE_HPP
#define KEYS_STORAGE_FILE_HPP

#include <stdbool.h>
#include <time.h>

#include <verificaC19-sdk/interfaces/IKeysStorage.hpp>

#include <json/json.h>

#include <string>

namespace verificaC19Sdk {

/**
 * Implements storage to store keys to json file
 */
class KeysStorageFile : public IKeysStorage {

public:

	KeysStorageFile();
	std::string getKey(const std::string& kid) const;
	void beginUpdatingKeys();
	void storeKey(const std::string& kid, const std::string& ecx);
	void commitUpdatedKeys();
	void rollbackUpdatedKeys();
	bool isUpdating() const { return m_updatingMode; }
	time_t lastUpdate() const { return time(NULL) - m_keys["_lastUpdate"].asInt(); }
	void setLastStoredKeyToken(std::string& token);
	std::string getLastStoredKeyToken() const { return m_keys["_lastToken"].asString(); };

private:

	bool m_updatingMode;
	Json::Value m_keys;
	Json::Value m_updatingKeys;

};

} // namespace verificaC19Sdk

#endif // #ifndef KEYS_STORAGE_FILE_HPP
