// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <rules-provider/RulesProviderTest.hpp>

namespace verificaC19Sdk {

class LoggerNull : public ILogger {

public:

	void error(const std::string& message, ...) const { }
	void info(const std::string& message, ...) const { }
	void debug(const std::string& message, ...) const { }

};

RulesProviderTest::RulesProviderTest(ILogger* logger) : m_logger(logger) {
	if (m_logger == NULL) {
		m_logger = new LoggerNull();
	}
}

void RulesProviderTest::refreshRules(IRulesStorage* rulesStorage) const {
	// insert rules used by test example
	m_logger->debug("Store test rules");
	rulesStorage->beginUpdatingRules();
	rulesStorage->storeRule(RULE_NAME_vaccine_start_day_complete, RULE_TYPE_EU_1_20_1528, "0");
	rulesStorage->storeRule(RULE_NAME_vaccine_end_day_complete, RULE_TYPE_EU_1_20_1528, "365");
	rulesStorage->storeRule(RULE_NAME_recovery_cert_start_day, RULE_TYPE_GENERIC, "0");
	rulesStorage->storeRule(RULE_NAME_recovery_cert_end_day, RULE_TYPE_GENERIC, "180");
	rulesStorage->storeRule(RULE_NAME_rapid_test_start_hours, RULE_TYPE_GENERIC, "0");
	rulesStorage->storeRule(RULE_NAME_rapid_test_end_hours, RULE_TYPE_GENERIC, "48");
	rulesStorage->storeRule(RULE_NAME_molecular_test_start_hours, RULE_TYPE_GENERIC, "0");
	rulesStorage->storeRule(RULE_NAME_molecular_test_end_hours, RULE_TYPE_GENERIC, "48");
	rulesStorage->commitUpdatedRules();
}

} // namespace verificaC19Sdk
