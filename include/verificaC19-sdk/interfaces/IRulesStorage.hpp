// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef IRULES_STORAGE_HPP
#define IRULES_STORAGE_HPP

#include <string>

namespace verificaC19Sdk {

#define RULE_NAME_vaccine_end_day_complete        "vaccine_end_day_complete"
#define RULE_NAME_vaccine_start_day_complete      "vaccine_start_day_complete"
#define RULE_NAME_vaccine_end_day_not_complete    "vaccine_end_day_not_complete"
#define RULE_NAME_vaccine_start_day_not_complete  "vaccine_start_day_not_complete"
#define RULE_NAME_rapid_test_start_hours          "rapid_test_start_hours"
#define RULE_NAME_rapid_test_end_hours            "rapid_test_end_hours"
#define RULE_NAME_molecular_test_start_hours      "molecular_test_start_hours"
#define RULE_NAME_molecular_test_end_hours        "molecular_test_end_hours"
#define RULE_NAME_recovery_cert_start_day         "recovery_cert_start_day"
#define RULE_NAME_recovery_pv_cert_start_day      "recovery_pv_cert_start_day"
#define RULE_NAME_recovery_cert_end_day           "recovery_cert_end_day"
#define RULE_NAME_recovery_pv_cert_end_day        "recovery_pv_cert_end_day"
#define RULE_NAME_black_list_uvci                 "black_list_uvci"
#define RULE_NAME_ios                             "ios"
#define RULE_NAME_android                         "android"
#define RULE_NAME_sdk                             "sdk"

#define RULE_NAME_vaccine_start_day_complete_IT      "vaccine_start_day_complete_IT"
#define RULE_NAME_vaccine_end_day_complete_IT        "vaccine_end_day_complete_IT"
#define RULE_NAME_vaccine_start_day_booster_IT       "vaccine_start_day_booster_IT"
#define RULE_NAME_vaccine_end_day_booster_IT         "vaccine_end_day_booster_IT"

#define RULE_NAME_vaccine_start_day_complete_NOT_IT  "vaccine_start_day_complete_NOT_IT"
#define RULE_NAME_vaccine_end_day_complete_NOT_IT    "vaccine_end_day_complete_NOT_IT"
#define RULE_NAME_vaccine_start_day_booster_NOT_IT   "vaccine_start_day_booster_NOT_IT"
#define RULE_NAME_vaccine_end_day_booster_NOT_IT     "vaccine_end_day_booster_NOT_IT"

#define RULE_NAME_recovery_cert_start_day_IT         "recovery_cert_start_day_IT"
#define RULE_NAME_recovery_cert_end_day_IT           "recovery_cert_end_day_IT"
#define RULE_NAME_recovery_cert_start_day_NOT_IT     "recovery_cert_start_day_NOT_IT"
#define RULE_NAME_recovery_cert_end_day_NOT_IT       "recovery_cert_end_day_NOT_IT"


#define RULE_TYPE_EU_1_20_1525     "EU/1/20/1525" // Janssen
#define RULE_TYPE_EU_1_21_1529     "EU/1/21/1529" // Vaxzevria (Astrazeneca)
#define RULE_TYPE_EU_1_20_1507     "EU/1/20/1507" // Spikevax (Moderna)
#define RULE_TYPE_EU_1_20_1528     "EU/1/20/1528" // Comirnaty (Pfizer Biontech)
#define RULE_TYPE_Sputnik_V        "Sputnik-V"
#define RULE_TYPE_GENERIC          "GENERIC"
#define RULE_TYPE_APP_MIN_VERSION  "APP_MIN_VERSION"
#define RULE_TYPE_SDK_MIN_VERSION  "SDK_MIN_VERSION"
#define RULE_TYPE_black_list_uvci  "black_list_uvci"

/**
 * Interface to implement Key Storage service to store and retrieve keys from storage archive
 */
class IRulesStorage {

public:

	/**
	 * Retrieve requested rule from storage archive
	 *
	 * @param name Rule name
	 *
	 * @param type Rule type
	 *
	 * @return Rule value
	 */
	virtual std::string getRule(const std::string& name, const std::string& type) const = 0;

	/**
	 * Start transaction with Rules storage archive
	 */
	virtual void beginUpdatingRules() = 0;

	/**
	 * Store rule in storage archive, if started transaction it must be stored in transaction
	 * mode, else it must be stored immediately to archive
	 *
	 * @param name Rule name
	 *
	 * @param type Rule type
	 *
	 * @param value Rule value
	 */
	virtual void storeRule(const std::string& name, const std::string& type, const std::string& value) = 0;

	/**
	 * End transaction with Rules storage service
	 */
	virtual void commitUpdatedRules() = 0;

	/**
	 * End transaction with Rules storage service
	 */
	virtual void rollbackUpdatedRules() = 0;

	/**
	 * Request to Rules storage service if update transaction is running
	 *
	 * @return true if transaction running, else false.
	 */
	virtual bool isUpdating() const = 0;

	/**
	 * Get time in seconds elapsed until last update
	 *
	 * @return Seconds elapsed until last update
	 */
	virtual time_t lastUpdate() const = 0;

};

} // namespace verificaC19Sdk

#endif // #ifndef IRULES_STORAGE_HPP
