// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_STORAGE_MEMORY_HPP
#define RULES_STORAGE_MEMORY_HPP

#include <stdbool.h>
#include <time.h>

#include <verificaC19-sdk/interfaces/IRulesStorage.hpp>

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

extern "C" {

#include <examples/rules-storage/RulesStorageMemory.h>

} // extern "C"

#endif // #ifndef RULES_STORAGE_MEMORY_HPP
