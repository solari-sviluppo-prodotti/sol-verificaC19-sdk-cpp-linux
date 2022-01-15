// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <string.h>
#include <stdlib.h>
#include <verificaC19-sdk/interfaces/IKeysStorage.hpp>

extern "C" {

#include <verificaC19-sdk/interfaces/KeysStorage.h>

}

void KeysStorage_c_release(const KeysStorage* keysStorage) {
	verificaC19Sdk::IKeysStorage* this_ = (verificaC19Sdk::IKeysStorage*)keysStorage;
	delete this_;
}

const char* KeysStorage_c_getKey(const KeysStorage* keysStorage, const char* kid) {
	verificaC19Sdk::IKeysStorage* this_ = (verificaC19Sdk::IKeysStorage*)keysStorage;
	std::string ecx = this_->getKey(std::string(kid));
	char* p = (char*)malloc(ecx.length() + 1);
	strcpy(p, ecx.c_str());
	return p;
}

void KeysStorage_c_beginUpdatingKeys(const KeysStorage* keysStorage) {
	verificaC19Sdk::IKeysStorage* this_ = (verificaC19Sdk::IKeysStorage*)keysStorage;
	this_->beginUpdatingKeys();
}

void KeysStorage_c_storeKey(const KeysStorage* keysStorage, const char* kid, const char* ecx) {
	verificaC19Sdk::IKeysStorage* this_ = (verificaC19Sdk::IKeysStorage*)keysStorage;
	this_->storeKey(std::string(kid), std::string(ecx));
}

void KeysStorage_c_commitUpdatedKeys(const KeysStorage* keysStorage) {
	verificaC19Sdk::IKeysStorage* this_ = (verificaC19Sdk::IKeysStorage*)keysStorage;
	this_->commitUpdatedKeys();
}

void KeysStorage_c_rollbackUpdatedKeys(const KeysStorage* keysStorage) {
	verificaC19Sdk::IKeysStorage* this_ = (verificaC19Sdk::IKeysStorage*)keysStorage;
	this_->rollbackUpdatedKeys();
}

bool KeysStorage_c_isUpdating(const KeysStorage* keysStorage) {
	verificaC19Sdk::IKeysStorage* this_ = (verificaC19Sdk::IKeysStorage*)keysStorage;
	return this_->isUpdating();
}

time_t KeysStorage_c_lastUpdate(const KeysStorage* keysStorage) {
	verificaC19Sdk::IKeysStorage* this_ = (verificaC19Sdk::IKeysStorage*)keysStorage;
	return this_->lastUpdate();
}

void KeysStorage_c_setLastStoredKeyToken(const KeysStorage* keysStorage, const char* token) {
	verificaC19Sdk::IKeysStorage* this_ = (verificaC19Sdk::IKeysStorage*)keysStorage;
	std::string stoken = std::string(token);
	this_->setLastStoredKeyToken(stoken);
}

const char* KeysStorage_c_getLastStoredKeyToken(const KeysStorage* keysStorage) {
	verificaC19Sdk::IKeysStorage* this_ = (verificaC19Sdk::IKeysStorage*)keysStorage;
	std::string token = this_->getLastStoredKeyToken();
	char* p = (char*)malloc(token.length() + 1);
	strcpy(p, token.c_str());
	return p;
}
