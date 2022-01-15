// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <cstring>
#include <keys-storage/KeysStorageMemory.hpp>

namespace verificaC19Sdk {

std::string KeysStorageMemory::getKey(const std::string& kid) const {
	std::string key;
	if (m_keys.find(kid) != m_keys.end()) {
		key = m_keys.find(kid)->second;
	}
	return key;
}

void KeysStorageMemory::beginUpdatingKeys() {
	m_updatingMode = true;
	m_updatingKeys = m_keys;
}

void KeysStorageMemory::storeKey(const std::string& kid, const std::string& ecx) {
	if (m_updatingMode) {
		m_updatingKeys.insert(std::pair<std::string, std::string>(kid, ecx));
	} else {
		// permit insert without "transaction" mode
		m_keys.insert(std::pair<std::string, std::string>(kid, ecx));
		m_lastUpdate = time(NULL);
	}
}

void KeysStorageMemory::commitUpdatedKeys() {
	m_keys = m_updatingKeys;
	m_updatingKeys.clear();
	m_updatingMode = false;
	m_lastUpdate = time(NULL);
}

void KeysStorageMemory::rollbackUpdatedKeys() {
	m_updatingKeys.clear();
	m_updatingMode = false;
}

} // namespace verificaC19Sdk

