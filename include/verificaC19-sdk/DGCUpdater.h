// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef DGC_UPDATER_H
#define DGC_UPDATER_H

#include <verificaC19-sdk/interfaces/KeysProvider.h>
#include <verificaC19-sdk/interfaces/KeysStorage.h>
#include <verificaC19-sdk/interfaces/RulesProvider.h>
#include <verificaC19-sdk/interfaces/RulesStorage.h>
#include <verificaC19-sdk/interfaces/CRLProvider.h>
#include <verificaC19-sdk/interfaces/CRLStorage.h>
#include <verificaC19-sdk/interfaces/AllProvider.h>
#include <verificaC19-sdk/interfaces/Logger.h>

typedef struct {} Updater;

/**
 * DGCRulesKeysUpdater constructor interface for C and for dynamic loading
 */
Updater* DGCUpdater_c_create(int intervalSeconds,
		RulesProvider* rulesProvider, RulesStorage* rulesStorage,
		KeysProvider* keysProvider, KeysStorage* keysStorage,
		CRLProvider* crlProvider, CRLStorage* crlStorage,
		Logger* logger);
typedef Updater* (*pfDGCUpdater_c_create)(int, RulesProvider*, RulesStorage*,
		KeysProvider*, KeysStorage*, CRLProvider*, CRLStorage*, Logger*);

/**
 * DGCRulesKeysUpdater constructor interface for C and for dynamic loading
 */
Updater* DGCUpdaterAll_c_create(int intervalSeconds,
		AllProvider* allProvider, RulesStorage* rulesStorage,
		KeysStorage* keysStorage, CRLStorage* crlStorage, Logger* logger);
typedef Updater* (*pfDGCUpdaterAll_c_create)(int, AllProvider*,
		RulesStorage*, KeysStorage*, CRLStorage*, Logger*);

/**
 * DGCRulesKeysUpdater destructor interface for C and for dynamic loading
 */
void DGCUpdater_c_release(Updater* dgcsUpdater);
typedef void (*pfDGCUpdater_c_release)(Updater*);

/**
 * DGCUpdater forceUpdateRules interface for C and for dynamic loading
 */
void DGCUpdater_c_forceUpdateRules(Updater* dgcUpdater);
typedef void (*pfDGCUpdater_c_forceUpdateRules)(Updater*);

/**
 * DGCUpdater forceUpdateKeys interface for C and for dynamic loading
 */
void DGCUpdater_c_forceUpdateKeys(Updater* dgcUpdater);
typedef void (*pfDGCUpdater_c_forceUpdateKeys)(Updater*);

/**
 * DGCUpdater forceUpdateCRL interface for C and for dynamic loading
 */
void DGCUpdater_c_forceUpdateCRL(Updater* dgcUpdater);
typedef void (*pfDGCUpdater_c_forceUpdateCRL)(Updater*);

/**
 * DGCUpdater forceUpdateAll interface for C and for dynamic loading
 */
void DGCUpdater_c_forceUpdateAll(Updater* dgcUpdater);
typedef void (*pfDGCUpdater_c_forceUpdateAll)(Updater*);


/**
 * DGCUpdater isUpdated interface for C and for dynamic loading
 */
bool DGCUpdater_c_isUpdated(Updater* dgcUpdater);
typedef bool (*pfDGCUpdater_c_isUpdated)(Updater*);

#endif // #ifndef DGC_UPDATER_H
