// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <cstring>
#include <RulesStorageMemory.hpp>

namespace verificaC19Sdk {

std::string RulesStorageMemory::getRule(const std::string& name, const std::string& type) const {
	std::string rule;
	std::pair<std::string, std::string> key(name, type);
	if (m_rules.find(key) != m_rules.end()) {
		rule = m_rules.find(key)->second;
	}
	return rule;
}
void RulesStorageMemory::beginUpdatingRules() {
	m_updatingMode = true;
	m_updatingRules.clear();
}

void RulesStorageMemory::storeRule(const std::string& name, const std::string& type, const std::string& value) {
	std::pair<std::string, std::string> key(name, type);
	if (m_updatingMode) {
		m_updatingRules.insert(std::pair<std::pair<std::string, std::string>, std::string>(key, value));
	} else {
		// permit insert without "transaction" mode
		m_rules.insert(std::pair<std::pair<std::string, std::string>, std::string>(key, value));
		m_lastUpdate = time(NULL);
	}
}

void RulesStorageMemory::commitUpdatedRules() {
	m_rules = m_updatingRules;
	m_updatingRules.clear();
	m_updatingMode = false;
	m_lastUpdate = time(NULL);
}

void RulesStorageMemory::rollbackUpdatedRules() {
	m_updatingRules.clear();
	m_updatingMode = false;
}

} // namespace verificaC19Sdk

RulesStorage* RulesStorageMemory_c_create() {
	return (RulesStorage*)new verificaC19Sdk::RulesStorageMemory();
}

void RulesStorageMemory_c_release(const RulesStorage* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageMemory* this_ = (verificaC19Sdk::RulesStorageMemory*)rulesStorageMemory;
	delete this_;
}

const char* RulesStorageMemory_c_getRule(const RulesStorage* rulesStorageMemory, const char* name, const char* type) {
	verificaC19Sdk::RulesStorageMemory* this_ = (verificaC19Sdk::RulesStorageMemory*)rulesStorageMemory;
	std::string value = this_->getRule(std::string(name), std::string(type));
	char* p = (char*)malloc(value.length() + 1);
	strcpy(p, value.c_str());
	return p;
}

void RulesStorageMemory_c_beginUpdatingRules(const RulesStorage* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageMemory* this_ = (verificaC19Sdk::RulesStorageMemory*)rulesStorageMemory;
	this_->beginUpdatingRules();
}

void RulesStorageMemory_c_storeRule(const RulesStorage* rulesStorageMemory, const char* name, const char* type, const char* value) {
	verificaC19Sdk::RulesStorageMemory* this_ = (verificaC19Sdk::RulesStorageMemory*)rulesStorageMemory;
	this_->storeRule(std::string(name), std::string(type), std::string(value));
}

void RulesStorageMemory_c_commitUpdatedRules(const RulesStorage* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageMemory* this_ = (verificaC19Sdk::RulesStorageMemory*)rulesStorageMemory;
	this_->commitUpdatedRules();
}

void RulesStorageMemory_c_rollbackUpdatedRules(const RulesStorage* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageMemory* this_ = (verificaC19Sdk::RulesStorageMemory*)rulesStorageMemory;
	this_->rollbackUpdatedRules();
}

bool RulesStorageMemory_c_isUpdating(const RulesStorage* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageMemory* this_ = (verificaC19Sdk::RulesStorageMemory*)rulesStorageMemory;
	return this_->isUpdating();
}

time_t RulesStorageMemory_c_lastUpdate(const RulesStorage* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageMemory* this_ = (verificaC19Sdk::RulesStorageMemory*)rulesStorageMemory;
	return this_->lastUpdate();
}
