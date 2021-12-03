// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_STORAGE_MEMORY_HPP
#define RULES_STORAGE_MEMORY_HPP

#include <stdbool.h>
#include <time.h>

#include <verificaC19-sdk/interfaces/IRulesStorage.hpp>

#ifdef __cplusplus

#include <string>
#include <map>

namespace verificaC19Sdk {

class RulesStorageMemory : public IRulesStorage {

public:

	RulesStorageMemory() : m_lastUpdate(0), m_updatingMode(false) {}
	std::string getRule(const std::string& name, const std::string& type) const;
	void beginUpdatingRules();
	void storeRule(const std::string& name, const std::string& type, const std::string& value);
	void commitUpdatedRules();
	void rollbackUpdatedRules();
	bool isUpdating() { return m_updatingMode; }
	time_t lastUpdate() { return time(NULL) - m_lastUpdate; }

private:

	time_t m_lastUpdate;
	bool m_updatingMode;
	std::map<std::pair<std::string, std::string>, std::string> m_rules;
	std::map<std::pair<std::string, std::string>, std::string> m_updatingRules;

};

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

void* RulesStorageMemory_c_create();
typedef void* (*pfRulesStorageMemory_c_create)(void*);

void RulesStorageMemory_c_release(const void* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_release)(const void*);

const char* RulesStorageMemory_c_getRule(const void* rulesStorageMemory, const char* name, const char* type);
typedef const char* (*pfRulesStorageMemory_c_getRule)(const void*, const char*, const char*);

void RulesStorageMemory_c_beginUpdatingRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_beginUpdatingRules)(const void*);

void RulesStorageMemory_c_storeRule(const void* rulesStorageMemory, const char* name, const char* type, const char* value);
typedef void (*pfRulesStorageMemory_c_storeRule)(const void*, const char*, const char*, const char*);

void RulesStorageMemory_c_commitUpdatedRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_commitUpdatedRules)(const void*);

void RulesStorageMemory_c_rollbackUpdatedRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_rollbackUpdatedRules)(const void*);

bool RulesStorageMemory_c_isUpdating(const void* rulesStorageMemory);
typedef bool (*pfRulesStorageMemory_c_isUpdating)(const void*);

time_t RulesStorageMemory_c_lastUpdate(const void* rulesStorageMemory);
typedef time_t (*pfRulesStorageMemory_c_lastUpdate)(const void*);

#ifdef __cplusplus
}
#endif


#endif // #ifndef RULES_STORAGE_MEMORY_HPP
