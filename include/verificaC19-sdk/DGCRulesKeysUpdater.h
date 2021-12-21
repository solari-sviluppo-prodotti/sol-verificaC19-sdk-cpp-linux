// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef DGC_RULES_KEYS_UPDATER_H
#define DGC_RULES_KEYS_UPDATER_H

#include <verificaC19-sdk/interfaces/KeysProvider.h>
#include <verificaC19-sdk/interfaces/KeysStorage.h>
#include <verificaC19-sdk/interfaces/RulesProvider.h>
#include <verificaC19-sdk/interfaces/RulesStorage.h>
#include <verificaC19-sdk/interfaces/RulesKeysProvider.h>
#include <verificaC19-sdk/interfaces/Logger.h>

typedef struct {} RulesKeysUpdater;

/**
 * DGCRulesKeysUpdater constructor interface for C and for dynamic loading
 */
RulesKeysUpdater* DGCRulesKeysUpdaterRulesAndKeys_c_create(int intervalSeconds,
		RulesProvider* rulesProvider, RulesStorage* rulesStorage,
		KeysProvider* keysProvider, KeysStorage* keysStorage, Logger* logger);
typedef RulesKeysUpdater* (*pfDGCRulesKeysUpdaterRulesAndKeys_c_create)(int, RulesProvider*, RulesStorage*,
		KeysProvider*, KeysStorage*, Logger*);

/**
 * DGCRulesKeysUpdater constructor interface for C and for dynamic loading
 */
RulesKeysUpdater* DGCRulesKeysUpdaterRulesKeys_c_create(int intervalSeconds,
		RulesKeysProvider* rulesKeysProvider, RulesStorage* rulesStorage,
		KeysStorage* keysStorage, Logger* logger);
typedef RulesKeysUpdater* (*pfDGCRulesKeysUpdaterRulesKeys_c_create)(int, RulesKeysProvider*,
		RulesStorage*, KeysStorage*, Logger*);

/**
 * DGCRulesKeysUpdater destructor interface for C and for dynamic loading
 */
void DGCRulesKeysUpdater_c_release(RulesKeysUpdater* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_c_release)(RulesKeysUpdater*);

/**
 * DGCRulesKeysUpdater forceUpdateRules interface for C and for dynamic loading
 */
void DGCRulesKeysUpdater_c_forceUpdateRules(RulesKeysUpdater* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_c_forceUpdateRules)(RulesKeysUpdater*);

/**
 * DGCRulesKeysUpdater forceUpdateKeys interface for C and for dynamic loading
 */
void DGCRulesKeysUpdater_c_forceUpdateKeys(RulesKeysUpdater* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_c_forceUpdateKeys)(RulesKeysUpdater*);

/**
 * DGCRulesKeysUpdater forceUpdateAll interface for C and for dynamic loading
 */
void DGCRulesKeysUpdater_c_forceUpdateAll(RulesKeysUpdater* dgcRulesKeysUpdater);
typedef void (*pfDGCRulesKeysUpdater_c_forceUpdateAll)(RulesKeysUpdater*);


/**
 * DGCRulesKeysUpdater isUpdated interface for C and for dynamic loading
 */
bool DGCRulesKeysUpdater_c_isUpdated(RulesKeysUpdater* dgcRulesKeysUpdater);
typedef bool (*pfDGCRulesKeysUpdater_c_isUpdated)(RulesKeysUpdater*);

#endif // #ifndef DGC_RULES_KEYS_UPDATER_H
