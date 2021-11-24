// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_STORAGE_MEMORY_HPP
#define KEYS_STORAGE_MEMORY_HPP

#include <string>
#include <map>
#include <verificaC19-sdk/interfaces/IKeysStorage.hpp>

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

#endif // #ifndef KEYS_STORAGE_MEMORY_HPP
