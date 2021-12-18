// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_STORAGE_MEMORY_H
#define RULES_STORAGE_MEMORY_H

#include <stdbool.h>

void* RulesStorageMemory_c_create();
typedef void* (*pfRulesStorageMemory_c_create)(void*);

void RulesStorageMemory_c_release(const void* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_release)(const void*);

const char* RulesStorageMemory_c_getRule(const void* rulesStorageMemory, const char* name, const char* type);
typedef const char* (*pfRulesStorageMemory_c_getRule)(const void*, const char*, const char*);

void RulesStorageMemory_c_beginUpdatingRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_beginUpdatingRules)(const void*);

void RulesStorageMemory_c_storeRule(const void* rulesStorageMemory, const char* name, const char* type, const char* value);
typedef void (*pfRulesStorageMemory_c_storeRule)(const void*, const char*, const char*, const char*);

void RulesStorageMemory_c_commitUpdatedRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_commitUpdatedRules)(const void*);

void RulesStorageMemory_c_rollbackUpdatedRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_rollbackUpdatedRules)(const void*);

bool RulesStorageMemory_c_isUpdating(const void* rulesStorageMemory);
typedef bool (*pfRulesStorageMemory_c_isUpdating)(const void*);

time_t RulesStorageMemory_c_lastUpdate(const void* rulesStorageMemory);
typedef time_t (*pfRulesStorageMemory_c_lastUpdate)(const void*);

#endif // #ifndef RULES_STORAGE_MEMORY_H
