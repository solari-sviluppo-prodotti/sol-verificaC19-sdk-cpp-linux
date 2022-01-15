// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_STORAGE_FILE_HPP
#define RULES_STORAGE_FILE_HPP

#include <stdbool.h>
#include <time.h>

#include <verificaC19-sdk/interfaces/IRulesStorage.hpp>

#include <json/json.h>

#include <string>

namespace verificaC19Sdk {

/**
 * Implements storage to store rules to json file
 */
class RulesStorageFile : public IRulesStorage {

public:

	RulesStorageFile();
	std::string getRule(const std::string& name, const std::string& type) const;
	void beginUpdatingRules();
	void storeRule(const std::string& name, const std::string& type, const std::string& value);
	void commitUpdatedRules();
	void rollbackUpdatedRules();
	bool isUpdating() const { return m_updatingMode; }
	time_t lastUpdate() const { return time(NULL) - m_rules["_lastUpdate"].asInt(); }

private:

	bool m_updatingMode;
	Json::Value m_rules;
	Json::Value m_updatingRules;

};

} // namespace verificaC19Sdk

#endif // #ifndef RULES_STORAGE_FILE_HPP
