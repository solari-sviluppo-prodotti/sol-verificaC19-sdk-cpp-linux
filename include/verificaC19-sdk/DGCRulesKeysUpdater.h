// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef DGC_RULES_KEYS_UPDATER_H
#define DGC_RULES_KEYS_UPDATER_H

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

#endif // #ifndef DGC_RULES_KEYS_UPDATER_H
