// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <cstring>
#include <sstream>
#include <fstream>
#include <RulesStorageFile.hpp>

namespace verificaC19Sdk {

RulesStorageFile::RulesStorageFile() : m_updatingMode(false) {
	std::ifstream frules;
	frules.open("rules.json");
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	std::stringstream srules;
	srules << frules.rdbuf();
	frules.close();
	std::string rules = srules.str();
	std::string errors;
	reader->parse(rules.c_str(), rules.c_str() + rules.length(), &m_rules, &errors);
	delete reader;
}

std::string RulesStorageFile::getRule(const std::string& name, const std::string& type) const {
	std::string rule;
	std::string key = name + "-" + type;
	if (m_rules.isMember(key)) {
		rule = m_rules[key].asString();
	}
	return rule;
}
void RulesStorageFile::beginUpdatingRules() {
	m_updatingMode = true;
	m_updatingRules.clear();
}

void RulesStorageFile::storeRule(const std::string& name, const std::string& type, const std::string& value) {
	std::string key = name + "-" + type;
	if (m_updatingMode) {
		m_updatingRules[key] = value;
	} else {
		// permit insert without "transaction" mode
		m_rules[key] = value;
		m_rules["_lastUpdate"] = (int)time(NULL);
		std::ofstream frules;
		frules.open("rules.json");
		Json::StreamWriterBuilder builder;
		Json::StreamWriter* fastWriter = builder.newStreamWriter();
		fastWriter->write(m_rules, &frules);
		frules.close();
		delete fastWriter;
	}
}

void RulesStorageFile::commitUpdatedRules() {
	m_rules = m_updatingRules;
	m_updatingRules.clear();
	m_updatingMode = false;
	m_rules["_lastUpdate"] = (int)time(NULL);
	std::ofstream frules;
	frules.open("rules.json");
	Json::StreamWriterBuilder builder;
	Json::StreamWriter* fastWriter = builder.newStreamWriter();
	fastWriter->write(m_rules, &frules);
	frules.close();
}

void RulesStorageFile::rollbackUpdatedRules() {
	m_updatingRules.clear();
	m_updatingMode = false;
}

} // namespace verificaC19Sdk

void* RulesStorageFile_c_create() {
	return new verificaC19Sdk::RulesStorageFile();
}

void RulesStorageFile_c_release(const void* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageFile* this_ = (verificaC19Sdk::RulesStorageFile*)rulesStorageMemory;
	delete this_;
}

const char* RulesStorageFile_c_getRule(const void* rulesStorageMemory, const char* name, const char* type) {
	verificaC19Sdk::RulesStorageFile* this_ = (verificaC19Sdk::RulesStorageFile*)rulesStorageMemory;
	std::string value = this_->getRule(std::string(name), std::string(type));
	char* p = (char*)malloc(value.length() + 1);
	strcpy(p, value.c_str());
	return p;
}

void RulesStorageFile_c_beginUpdatingRules(const void* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageFile* this_ = (verificaC19Sdk::RulesStorageFile*)rulesStorageMemory;
	this_->beginUpdatingRules();
}

void RulesStorageFile_c_storeRule(const void* rulesStorageMemory, const char* name, const char* type, const char* value) {
	verificaC19Sdk::RulesStorageFile* this_ = (verificaC19Sdk::RulesStorageFile*)rulesStorageMemory;
	this_->storeRule(std::string(name), std::string(type), std::string(value));
}

void RulesStorageFile_c_commitUpdatedRules(const void* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageFile* this_ = (verificaC19Sdk::RulesStorageFile*)rulesStorageMemory;
	this_->commitUpdatedRules();
}

void RulesStorageFile_c_rollbackUpdatedRules(const void* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageFile* this_ = (verificaC19Sdk::RulesStorageFile*)rulesStorageMemory;
	this_->rollbackUpdatedRules();
}

bool RulesStorageFile_c_isUpdating(const void* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageFile* this_ = (verificaC19Sdk::RulesStorageFile*)rulesStorageMemory;
	return this_->isUpdating();
}

time_t RulesStorageFile_c_lastUpdate(const void* rulesStorageMemory) {
	verificaC19Sdk::RulesStorageFile* this_ = (verificaC19Sdk::RulesStorageFile*)rulesStorageMemory;
	return this_->lastUpdate();
}
