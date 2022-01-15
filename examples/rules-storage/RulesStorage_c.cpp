// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <string.h>
#include <stdlib.h>
#include <verificaC19-sdk/interfaces/IRulesStorage.hpp>

extern "C" {

#include <verificaC19-sdk/interfaces/RulesStorage.h>

}

void RulesStorage_c_release(const RulesStorage* rulesStorage) {
	verificaC19Sdk::IRulesStorage* this_ = (verificaC19Sdk::IRulesStorage*)rulesStorage;
	delete this_;
}

const char* RulesStorage_c_getRule(const RulesStorage* rulesStorage, const char* name, const char* type) {
	verificaC19Sdk::IRulesStorage* this_ = (verificaC19Sdk::IRulesStorage*)rulesStorage;
	std::string value = this_->getRule(std::string(name), std::string(type));
	char* p = (char*)malloc(value.length() + 1);
	strcpy(p, value.c_str());
	return p;
}

void RulesStorage_c_beginUpdatingRules(const RulesStorage* rulesStorage) {
	verificaC19Sdk::IRulesStorage* this_ = (verificaC19Sdk::IRulesStorage*)rulesStorage;
	this_->beginUpdatingRules();
}

void RulesStorage_c_storeRule(const RulesStorage* rulesStorage, const char* name, const char* type, const char* value) {
	verificaC19Sdk::IRulesStorage* this_ = (verificaC19Sdk::IRulesStorage*)rulesStorage;
	this_->storeRule(std::string(name), std::string(type), std::string(value));
}

void RulesStorage_c_commitUpdatedRules(const RulesStorage* rulesStorage) {
	verificaC19Sdk::IRulesStorage* this_ = (verificaC19Sdk::IRulesStorage*)rulesStorage;
	this_->commitUpdatedRules();
}

void RulesStorage_c_rollbackUpdatedRules(const RulesStorage* rulesStorage) {
	verificaC19Sdk::IRulesStorage* this_ = (verificaC19Sdk::IRulesStorage*)rulesStorage;
	this_->rollbackUpdatedRules();
}

bool RulesStorage_c_isUpdating(const RulesStorage* rulesStorage) {
	verificaC19Sdk::IRulesStorage* this_ = (verificaC19Sdk::IRulesStorage*)rulesStorage;
	return this_->isUpdating();
}

time_t RulesStorage_c_lastUpdate(const RulesStorage* rulesStorage) {
	verificaC19Sdk::IRulesStorage* this_ = (verificaC19Sdk::IRulesStorage*)rulesStorage;
	return this_->lastUpdate();
}
