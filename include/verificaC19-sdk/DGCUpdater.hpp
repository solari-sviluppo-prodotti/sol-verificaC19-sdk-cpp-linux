// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef DGC_UPDATER_HPP
#define DGC_UPDATER_HPP

#include <verificaC19-sdk/interfaces/IAllProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

#include <pthread.h>

namespace verificaC19Sdk {

/**
 * Implements Rules, Keys and CRL update service
 *
 * It updates Rules and / or Keys and / or CRL every `intervalSeconds` second, by asking
 * to storage service the time in seconds from last update.
 *
 * Rules, Keys and CRL update is made with "transaction" mode, to leave access to old
 * data until update is done.
 */
class DGCUpdater {

public:

	/**
	 * @param intervalSeconds Interval in seconds to check for updated Rules or Keys,
	 *                        will be automatically corrected to 4 hours if lesser than 4 hours
	 *                        or to 24 hours if greater than 24 hours
	 *
	 * @param rulesProvider Implementation of Rules Provider service, used to retrieve
	 *                     rules from backend provider
	 *
	 * @param rulesStorage Implementation of Rules Storage service, used to store
	 *                     retrieved rules
	 *
	 * @param keysProvider Implementation of Keys Provider service, used to retrieve
	 *                     keys from backend provider
	 *
	 * @param keysStorage Implementation of Keys Storage service, used to retrieve
	 *                    key needed to verify Digital Certificate validity
	 *
	 * @param crlProvider Implementation of CRL Provider service, used to retrieve
	 *                    CRL from backend provider
	 *
	 * @param crlStorage Implementation of CRL Storage service, used to retrieve
	 *                   CRL needed to verify Digital Certificate validity
	 *
	 * @param logger Optional implementation of Logger, if NULL no log will be done
	 */
	DGCUpdater(int intervalSeconds,
			IRulesProvider* rulesProvider, IRulesStorage* rulesStorage,
			IKeysProvider* keysProvider, IKeysStorage* keysStorage,
			ICRLProvider* crlProvider, ICRLStorage* crlStorage,
			ILogger* logger);

	/**
	 * @param intervalSeconds Interval in seconds to check for updated Rules or Keys,
	 *                        will be automatically corrected to 4 hours if lesser than 4 hours
	 *                        or to 24 hours if greater than 24 hours
	 *
	 * @param allProvider Implementation of Rules, Keys and CRLProvider service,
	 *                    used to retrieve rules, keys and CRL from backend provider
	 *
	 * @param rulesStorage Implementation of Rules Storage service, used to store
	 *                     retrieved rules
	 *
	 * @param keysStorage Implementation of Keys Storage service, used to retrieve
	 *                    key needed to verify Digital Certificate validity
	 *
	 * @param crlStorage Implementation of CRL Storage service, used to retrieve
	 *                   CRL needed to verify Digital Certificate validity
	 *
	 * @param logger Optional implementation of Logger, if NULL no log will be done
	 */
	DGCUpdater(int intervalSeconds,
			IAllProvider* allProvider, IRulesStorage* rulesStorage,
			IKeysStorage* keysStorage, ICRLStorage* crlStorage, ILogger* logger);

	virtual ~DGCUpdater();

	/**
	 * Force update of rules
	 */
	void forceUpdateRules();

	/**
	 * Force update of keys
	 */
	void forceUpdateKeys();

	/**
	 * Force update of CRL
	 */
	void forceUpdateCRL();

	/**
	 * Force update of rules and keys
	 */
	void forceUpdateAll();

	/**
	 * Check if rules and keys are updated or need to wait for update
	 * Useful for first startup to wait to have almost a first set of rules and keys
	 */
	bool isUpdated() const;

private:

	void updateAll() const;
	void updateRules() const;
	void updateKeys() const;
	void updateCRL() const;
	static void* updaterThread(void* p);
	void updaterThread();

	pthread_t m_threadUpdater;

	int m_intervalSeconds;
	IKeysProvider* m_keysProvider;
	IKeysStorage* m_keysStorage;
	IRulesProvider* m_rulesProvider;
	IRulesStorage* m_rulesStorage;
	ICRLProvider* m_crlProvider;
	ICRLStorage* m_crlStorage;
	IAllProvider* m_allProvider;
	ILogger* m_logger;
	bool m_threadLoop;
	bool m_forceUpdateRules;
	bool m_forceUpdateKeys;
	bool m_forceUpdateCRL;

}; // class DGCRulesKeysUpdater

extern "C" {

/**
 * DGCRulesKeysUpdater constructor interface for dynamic loading
 */
DGCUpdater* DGCUpdater_create(int intervalSeconds,
		IRulesProvider* rulesProvider, IRulesStorage* rulesStorage,
		IKeysProvider* keysProvider, IKeysStorage* keysStorage,
		ICRLProvider* crlProvider, ICRLStorage* crlStorage,
		ILogger* logger);
typedef DGCUpdater* (*pfDGCUpdater_create)(int, IRulesProvider*, IRulesStorage*,
		IKeysProvider*, IKeysStorage*, ICRLProvider*, ICRLStorage*, ILogger*);

/**
 * DGCRulesKeysUpdater constructor interface for dynamic loading
 */
DGCUpdater* DGCUpdaterAll_create(int intervalSeconds,
		IAllProvider* allProvider, IRulesStorage* rulesStorage,
		IKeysStorage* keysStorage, ICRLStorage* crlStorage, ILogger* logger);
typedef DGCUpdater* (*pfDGCUpdaterAll_create)(int, IAllProvider*,
		IRulesStorage*, IKeysStorage*, ICRLStorage*, ILogger*);

/**
 * DGCRUpdater destructor interface for dynamic loading
 */
void DGCUpdater_release(DGCUpdater* dgcUpdater);
typedef void (*pfDGCUpdater_release)(DGCUpdater*);

/**
 * DGCUpdater forceUpdateRules interface for dynamic loading
 */
void DGCUpdater_forceUpdateRules(DGCUpdater* dgcUpdater);
typedef void (*pfDGCUpdater_forceUpdateRules)(DGCUpdater*);

/**
 * DGCUpdater forceUpdateKeys interface for dynamic loading
 */
void DGCUpdater_forceUpdateKeys(DGCUpdater* dgcUpdater);
typedef void (*pfDGCUpdater_forceUpdateKeys)(DGCUpdater*);

/**
 * DGCUpdater forceUpdateCRL interface for dynamic loading
 */
void DGCUpdater_forceUpdateCRL (DGCUpdater* dgcUpdater);
typedef void (*pfDGCUpdater_forceUpdateCRL)(DGCUpdater*);

/**
 * DGCUpdater forceUpdateAll interface for dynamic loading
 */
void DGCUpdater_forceUpdateAll(DGCUpdater* dgcUpdater);
typedef void (*pfDGCUpdater_forceUpdateAll)(DGCUpdater*);

/**
 * DGCUpdater isUpdated interface for dynamic loading
 */
bool DGCUpdater_isUpdated(DGCUpdater* dgcUpdater);
typedef bool (*pfDGCUpdater_isUpdated)(DGCUpdater*);

} // extern "C"

} // namespace verificaC19Sdk

#endif // #ifndef DGC_UPDATER_HPP
