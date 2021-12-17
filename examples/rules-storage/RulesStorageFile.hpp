// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_STORAGE_FILE_HPP
#define RULES_STORAGE_FILE_HPP

#include <stdbool.h>
#include <time.h>

#include <verificaC19-sdk/interfaces/IRulesStorage.hpp>

#ifdef __cplusplus

#include <json/json.h>

#include <string>

namespace verificaC19Sdk {

class RulesStorageFile : public IRulesStorage {

public:

	RulesStorageFile();
	std::string getRule(const std::string& name, const std::string& type) const;
	void beginUpdatingRules();
	void storeRule(const std::string& name, const std::string& type, const std::string& value);
	void commitUpdatedRules();
	void rollbackUpdatedRules();
	bool isUpdating() { return m_updatingMode; }
	time_t lastUpdate() { return time(NULL) - m_rules["_lastUpdate"].asInt(); }

private:

	bool m_updatingMode;
	Json::Value m_rules;
	Json::Value m_updatingRules;

};

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

void* RulesStorageFile_c_create();
typedef void* (*pfRulesStorageFile_c_create)(void*);

void RulesStorageFile_c_release(const void* rulesStorageMemory);
typedef void (*pfRulesStorageFile_c_release)(const void*);

const char* RulesStorageFile_c_getRule(const void* rulesStorageMemory, const char* name, const char* type);
typedef const char* (*pfRulesStorageFile_c_getRule)(const void*, const char*, const char*);

void RulesStorageFile_c_beginUpdatingRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageFile_c_beginUpdatingRules)(const void*);

void RulesStorageFile_c_storeRule(const void* rulesStorageMemory, const char* name, const char* type, const char* value);
typedef void (*pfRulesStorageFile_c_storeRule)(const void*, const char*, const char*, const char*);

void RulesStorageFile_c_commitUpdatedRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageFile_c_commitUpdatedRules)(const void*);

void RulesStorageFile_c_rollbackUpdatedRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageFile_c_rollbackUpdatedRules)(const void*);

bool RulesStorageFile_c_isUpdating(const void* rulesStorageMemory);
typedef bool (*pfRulesStorageFile_c_isUpdating)(const void*);

time_t RulesStorageFile_c_lastUpdate(const void* rulesStorageMemory);
typedef time_t (*pfRulesStorageFile_c_lastUpdate)(const void*);

#ifdef __cplusplus
}
#endif


#endif // #ifndef RULES_STORAGE_FILE_HPP
