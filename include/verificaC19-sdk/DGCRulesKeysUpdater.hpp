// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef DGC_RULES_KEYS_UPDATER_HPP
#define DGC_RULES_KEYS_UPDATER_HPP

#include <pthread.h>
#include <verificaC19-sdk/interfaces/IRulesKeysProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

namespace verificaC19Sdk {

/**
 * Implements Rules and Keys update service
 *
 * It updates Rules and / or Keys every `intervalSeconds` second, by asking
 * to storage service the time in seconds from last update.
 *
 * Rules and Keys update is made with "transaction" mode, to leave access to old
 * data until update is done.
 */
class DGCRulesKeysUpdater {

public:

	/**
	 * @param intervalSeconds Interval in second to check for updated Rules or Keys
	 *
	 * @param rulesProvider Implementation of Rules Provider service, used to retreive
	 *                     rules from backend provider
	 *
	 * @param rulesStorage Implementation of Rules Storage service, used to store
	 *                     retreived rules
	 *
	 * @param keysProvider Implementation of Keys Provider service, used to retreive
	 *                     keys from backend provider
	 *
	 * @param keysStorage Implementation of Keys Storage service, used to retreive
	 *                    key needed to verify Digital Certificate validity
	 *
	 * @param logger Optional implementation of Logger, if NULL no log will be done
	 */
	DGCRulesKeysUpdater(int intervalSeconds,
			IRulesProvider* rulesProvider, IRulesStorage* rulesStorage,
			IKeysProvider* keysProvider, IKeysStorage* keysStorage, ILogger* logger);

	/**
	 * @param intervalSeconds Interval in second to check for updated Rules or Keys
	 *
	 * @param rulesKeysProvider Implementation of Rules and Keys Provider service,
	 *                          used to retreive rules and keys from backend provider
	 *
	 * @param rulesStorage Implementation of Rules Storage service, used to store
	 *                     retreived rules
	 *
	 * @param keysStorage Implementation of Keys Storage service, used to retreive
	 *                    key needed to verify Digital Certificate validity
	 *
	 * @param logger Optional implementation of Logger, if NULL no log will be done
	 */
	DGCRulesKeysUpdater(int intervalSeconds,
			IRulesKeysProvider* rulesKeysProvider, IRulesStorage* rulesStorage,
			IKeysStorage* keysStorage, ILogger* logger);

	virtual ~DGCRulesKeysUpdater();

	/**
	 * Force update of rules
	 */
	void forceUpdateRules();

	/**
	 * Force update of keys
	 */
	void forceUpdateKeys();

	/**
	 * Force update of rules and keys
	 */
	void forceUpdateAll();

private:

	void updateAll() const;
	void updateRules() const;
	void updateKeys() const;
	static void* updaterThread(void* p);
	void updaterThread();

	pthread_t m_threadUpdater;

	int m_intervalSeconds;
	IKeysProvider* m_keysProvider;
	IKeysStorage* m_keysStorage;
	IRulesProvider* m_rulesProvider;
	IRulesStorage* m_rulesStorage;
	IRulesKeysProvider* m_rulesKeysProvider;
	ILogger* m_logger;
	bool m_threadLoop;
	bool m_forceUpdateRules;
	bool m_forceUpdateKeys;

}; // class DGCRulesKeysUpdater

} // namespace verificaC19Sdk

#endif // #ifndef DGC_RULES_KEYS_UPDATER_HPP