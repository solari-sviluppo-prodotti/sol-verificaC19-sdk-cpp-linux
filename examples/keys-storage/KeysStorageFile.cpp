// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <cstring>
#include <sstream>
#include <fstream>
#include <keys-storage/KeysStorageFile.hpp>

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
	m_updatingKeys = m_keys;
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

