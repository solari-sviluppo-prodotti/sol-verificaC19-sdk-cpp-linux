// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <cstring>
#include <KeysStorageMemory.hpp>

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
	m_updatingKeys.clear();
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

void* KeysStorageMemory_c_create() {
	return new verificaC19Sdk::KeysStorageMemory();
}

void KeysStorageMemory_c_release(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageMemory* this_ = (verificaC19Sdk::KeysStorageMemory*)keysStorageMemory;
	delete this_;
}

const char* KeysStorageMemory_c_getKey(const void* keysStorageMemory, const char* kid) {
	verificaC19Sdk::KeysStorageMemory* this_ = (verificaC19Sdk::KeysStorageMemory*)keysStorageMemory;
	std::string ecx = this_->getKey(std::string(kid));
	char* p = (char*)malloc(ecx.length() + 1);
	strcpy(p, ecx.c_str());
	return p;
}

void KeysStorageMemory_c_beginUpdatingKeys(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageMemory* this_ = (verificaC19Sdk::KeysStorageMemory*)keysStorageMemory;
	this_->beginUpdatingKeys();
}

void KeysStorageMemory_c_storeKey(const void* keysStorageMemory, const char* kid, const char* ecx) {
	verificaC19Sdk::KeysStorageMemory* this_ = (verificaC19Sdk::KeysStorageMemory*)keysStorageMemory;
	this_->storeKey(std::string(kid), std::string(ecx));
}

void KeysStorageMemory_c_commitUpdatedKeys(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageMemory* this_ = (verificaC19Sdk::KeysStorageMemory*)keysStorageMemory;
	this_->commitUpdatedKeys();
}

void KeysStorageMemory_c_rollbackUpdatedKeys(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageMemory* this_ = (verificaC19Sdk::KeysStorageMemory*)keysStorageMemory;
	this_->rollbackUpdatedKeys();
}

bool KeysStorageMemory_c_isUpdating(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageMemory* this_ = (verificaC19Sdk::KeysStorageMemory*)keysStorageMemory;
	return this_->isUpdating();
}

time_t KeysStorageMemory_c_lastUpdate(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageMemory* this_ = (verificaC19Sdk::KeysStorageMemory*)keysStorageMemory;
	return this_->lastUpdate();
}

void KeysStorageMemory_c_setLastStoredKeyToken(const void* keysStorageMemory, const char* token) {
	verificaC19Sdk::KeysStorageMemory* this_ = (verificaC19Sdk::KeysStorageMemory*)keysStorageMemory;
	std::string stoken = std::string(token);
	this_->setLastStoredKeyToken(stoken);
}

const char* KeysStorageMemory_c_getLastStoredKeyToken(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageMemory* this_ = (verificaC19Sdk::KeysStorageMemory*)keysStorageMemory;
	std::string token = this_->getLastStoredKeyToken();
	char* p = (char*)malloc(token.length() + 1);
	strcpy(p, token.c_str());
	return p;
}
