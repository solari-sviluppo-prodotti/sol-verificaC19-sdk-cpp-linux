// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <cstring>
#include <rules-storage/RulesStorageMemory.hpp>

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

