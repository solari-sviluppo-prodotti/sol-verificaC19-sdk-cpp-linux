// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef DGC_RULES_KEYS_UPDATER_HPP
#define DGC_RULES_KEYS_UPDATER_HPP

#include <verificaC19-sdk/interfaces/IRulesKeysProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

#ifdef __cplusplus

#include <pthread.h>

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
	 * @param intervalSeconds Interval in seconds to check for updated Rules or Keys,
	 *                        will be automatically corrected to 4 hours if lesser than 4 hours
	 *                        or to 24 hours if greater than 24 hours
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
	 * @param intervalSeconds Interval in seconds to check for updated Rules or Keys,
	 *                        will be automatically corrected to 4 hours if lesser than 4 hours
	 *                        or to 24 hours if greater than 24 hours
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

	/**
	 * Check if rules and keys are updated or need to wait for update
	 * Useful for first startup to wait to have almost a first set of rules and keys
	 */
	bool isUpdated() const;

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

extern "C" {

/**
 * DGCRulesKeysUpdater constructor interface for dynamic loading
 */
DGCRulesKeysUpdater* DGCRulesKeysUpdaterRulesAndKeys_create(int intervalSeconds,
		IRulesProvider* rulesProvider, IRulesStorage* rulesStorage,
		IKeysProvider* keysProvider, IKeysStorage* keysStorage, ILogger* logger);
typedef DGCRulesKeysUpdater* (*pfDGCRulesKeysUpdaterRulesAndKeys_create)(int, IRulesProvider*, IRulesStorage*,
		IKeysProvider*, IKeysStorage*, ILogger*);

/**
 * DGCRulesKeysUpdater constructor interface for dynamic loading
 */
DGCRulesKeysUpdater* DGCRulesKeysUpdaterRulesKeys_create(int intervalSeconds,
		IRulesKeysProvider* rulesKeysProvider, IRulesStorage* rulesStorage,
		IKeysStorage* keysStorage, ILogger* logger);
typedef DGCRulesKeysUpdater* (*pfDGCRulesKeysUpdaterRulesKeys_create)(int, IRulesKeysProvider*,
		IRulesStorage*, IKeysStorage*, ILogger*);

/**
 * DGCRulesKeysUpdater destructor interface for dynamic loading
 */
void DGCRulesKeysUpdater_release(DGCRulesKeysUpdater* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_release)(DGCRulesKeysUpdater*);

/**
 * DGCRulesKeysUpdater forceUpdateRules interface for dynamic loading
 */
void DGCRulesKeysUpdater_forceUpdateRules(DGCRulesKeysUpdater* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_forceUpdateRules)(DGCRulesKeysUpdater*);

/**
 * DGCRulesKeysUpdater forceUpdateKeys interface for dynamic loading
 */
void DGCRulesKeysUpdater_forceUpdateKeys(DGCRulesKeysUpdater* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_forceUpdateKeys)(DGCRulesKeysUpdater*);

/**
 * DGCRulesKeysUpdater forceUpdateAll interface for dynamic loading
 */
void DGCRulesKeysUpdater_forceUpdateAll(DGCRulesKeysUpdater* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_forceUpdateAll)(DGCRulesKeysUpdater*);

/**
 * DGCRulesKeysUpdater isUpdated interface for dynamic loading
 */
bool DGCRulesKeysUpdater_isUpdated(DGCRulesKeysUpdater* dgcRulesKeysUpdater);
typedef bool (*pfDGCRulesKeysUpdater_isUpdated)(DGCRulesKeysUpdater*);

}

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

/**
 * DGCRulesKeysUpdater constructor interface for C and for dynamic loading
 */
void* DGCRulesKeysUpdaterRulesAndKeys_c_create(int intervalSeconds,
		void* rulesProvider, void* rulesStorage,
		void* keysProvider, void* keysStorage, void* logger);
typedef void* (*pfDGCRulesKeysUpdaterRulesAndKeys_c_create)(int, void*, void*,
		void*, void*, void*);

/**
 * DGCRulesKeysUpdater constructor interface for C and for dynamic loading
 */
void* DGCRulesKeysUpdaterRulesKeys_c_create(int intervalSeconds,
		void* rulesKeysProvider, void* rulesStorage,
		void* keysStorage, void* logger);
typedef void* (*pfDGCRulesKeysUpdaterRulesKeys_c_create)(int, void*,
		void*, void*, void*);

/**
 * DGCRulesKeysUpdater destructor interface for C and for dynamic loading
 */
void DGCRulesKeysUpdater_c_release(void* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_c_release)(void*);

/**
 * DGCRulesKeysUpdater forceUpdateRules interface for C and for dynamic loading
 */
void DGCRulesKeysUpdater_c_forceUpdateRules(void* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_c_forceUpdateRules)(void*);

/**
 * DGCRulesKeysUpdater forceUpdateKeys interface for C and for dynamic loading
 */
void DGCRulesKeysUpdater_c_forceUpdateKeys(void* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_c_forceUpdateKeys)(void*);

/**
 * DGCRulesKeysUpdater forceUpdateAll interface for C and for dynamic loading
 */
void DGCRulesKeysUpdater_c_forceUpdateAll(void* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_c_forceUpdateAll)(void*);


/**
 * DGCRulesKeysUpdater isUpdated interface for C and for dynamic loading
 */
bool DGCRulesKeysUpdater_c_isUpdated(void* dgcRulesKeysUpdater);
typedef bool (*pfDGCRulesKeysUpdater_c_isUpdated)(void*);

#ifdef __cplusplus
}
#endif

#endif // #ifndef DGC_RULES_KEYS_UPDATER_HPP
