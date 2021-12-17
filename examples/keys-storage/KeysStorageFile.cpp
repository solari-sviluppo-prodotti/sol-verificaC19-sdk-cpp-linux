// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <cstring>
#include <sstream>
#include <fstream>
#include <KeysStorageFile.hpp>

namespace verificaC19Sdk {

KeysStorageFile::KeysStorageFile() : m_updatingMode(false) {
	std::ifstream fkeys;
	fkeys.open("keys.json");
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	std::stringstream skeys;
	skeys << fkeys.rdbuf();
	fkeys.close();
	std::string keys = skeys.str();
	std::string errors;
	reader->parse(keys.c_str(), keys.c_str() + keys.length(), &m_keys, &errors);
	delete reader;
}

std::string KeysStorageFile::getKey(const std::string& kid) const {
	std::string key;
	if (m_keys.isMember(kid)) {
		key = m_keys[kid].asString();
	}
	return key;
}

void KeysStorageFile::beginUpdatingKeys() {
	m_updatingMode = true;
	m_updatingKeys.clear();
}

void KeysStorageFile::storeKey(const std::string& kid, const std::string& ecx) {
	if (m_updatingMode) {
		m_updatingKeys[kid] = ecx;
	} else {
		// permit insert without "transaction" mode
		m_keys[kid] = ecx;
		m_keys["_lastUpdate"] = (int)time(NULL);
		std::ofstream fkeys;
		fkeys.open("keys.json");
		Json::StreamWriterBuilder builder;
		Json::StreamWriter* fastWriter = builder.newStreamWriter();
		fastWriter->write(m_keys, &fkeys);
		fkeys.close();
		delete fastWriter;
	}
}

void KeysStorageFile::commitUpdatedKeys() {
	m_keys = m_updatingKeys;
	m_updatingKeys.clear();
	m_updatingMode = false;
	m_keys["_lastUpdate"] = (int)time(NULL);
	std::ofstream fkeys;
	fkeys.open("keys.json");
	Json::StreamWriterBuilder builder;
	Json::StreamWriter* fastWriter = builder.newStreamWriter();
	fastWriter->write(m_keys, &fkeys);
	fkeys.close();
	delete fastWriter;
}

void KeysStorageFile::rollbackUpdatedKeys() {
	m_updatingKeys.clear();
	m_updatingMode = false;
}

void KeysStorageFile::setLastStoredKeyToken(std::string& token) {
	if (m_updatingMode) {
		m_updatingKeys["_lastToken"] = token;
	} else {
		m_keys["_lastToken"] = token;
		std::ofstream fkeys;
		fkeys.open("keys.json");
		Json::StreamWriterBuilder builder;
		Json::StreamWriter* fastWriter = builder.newStreamWriter();
		fastWriter->write(m_keys, &fkeys);
		fkeys.close();
		delete fastWriter;
	}
}

} // namespace verificaC19Sdk

void* KeysStorageFile_c_create() {
	return new verificaC19Sdk::KeysStorageFile();
}

void KeysStorageFile_c_release(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageFile* this_ = (verificaC19Sdk::KeysStorageFile*)keysStorageMemory;
	delete this_;
}

const char* KeysStorageFile_c_getKey(const void* keysStorageMemory, const char* kid) {
	verificaC19Sdk::KeysStorageFile* this_ = (verificaC19Sdk::KeysStorageFile*)keysStorageMemory;
	std::string ecx = this_->getKey(std::string(kid));
	char* p = (char*)malloc(ecx.length() + 1);
	strcpy(p, ecx.c_str());
	return p;
}

void KeysStorageFile_c_beginUpdatingKeys(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageFile* this_ = (verificaC19Sdk::KeysStorageFile*)keysStorageMemory;
	this_->beginUpdatingKeys();
}

void KeysStorageFile_c_storeKey(const void* keysStorageMemory, const char* kid, const char* ecx) {
	verificaC19Sdk::KeysStorageFile* this_ = (verificaC19Sdk::KeysStorageFile*)keysStorageMemory;
	this_->storeKey(std::string(kid), std::string(ecx));
}

void KeysStorageFile_c_commitUpdatedKeys(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageFile* this_ = (verificaC19Sdk::KeysStorageFile*)keysStorageMemory;
	this_->commitUpdatedKeys();
}

void KeysStorageFile_c_rollbackUpdatedKeys(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageFile* this_ = (verificaC19Sdk::KeysStorageFile*)keysStorageMemory;
	this_->rollbackUpdatedKeys();
}

bool KeysStorageFile_c_isUpdating(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageFile* this_ = (verificaC19Sdk::KeysStorageFile*)keysStorageMemory;
	return this_->isUpdating();
}

time_t KeysStorageFile_c_lastUpdate(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageFile* this_ = (verificaC19Sdk::KeysStorageFile*)keysStorageMemory;
	return this_->lastUpdate();
}

void KeysStorageFile_c_setLastStoredKeyToken(const void* keysStorageMemory, const char* token) {
	verificaC19Sdk::KeysStorageFile* this_ = (verificaC19Sdk::KeysStorageFile*)keysStorageMemory;
	std::string stoken = std::string(token);
	this_->setLastStoredKeyToken(stoken);
}

const char* KeysStorageFile_c_getLastStoredKeyToken(const void* keysStorageMemory) {
	verificaC19Sdk::KeysStorageFile* this_ = (verificaC19Sdk::KeysStorageFile*)keysStorageMemory;
	std::string token = this_->getLastStoredKeyToken();
	char* p = (char*)malloc(token.length() + 1);
	strcpy(p, token.c_str());
	return p;
}
