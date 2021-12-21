// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_STORAGE_FILE_H
#define RULES_STORAGE_FILE_H

#include <verificaC19-sdk/interfaces/RulesStorage.h>

#include <stdbool.h>
#include <time.h>

RulesStorage* RulesStorageFile_c_create();
typedef RulesStorage* (*pfRulesStorageFile_c_create)(void*);

void RulesStorageFile_c_release(const RulesStorage* rulesStorageFile);
typedef void (*pfRulesStorageFile_c_release)(const RulesStorage*);

const char* RulesStorageFile_c_getRule(const RulesStorage* rulesStorageFile, const char* name, const char* type);
typedef const char* (*pfRulesStorageFile_c_getRule)(const RulesStorage*, const char*, const char*);

void RulesStorageFile_c_beginUpdatingRules(const RulesStorage* rulesStorageFile);
typedef void (*pfRulesStorageFile_c_beginUpdatingRules)(const RulesStorage*);

void RulesStorageFile_c_storeRule(const RulesStorage* rulesStorageFile, const char* name, const char* type, const char* value);
typedef void (*pfRulesStorageFile_c_storeRule)(const RulesStorage*, const char*, const char*, const char*);

void RulesStorageFile_c_commitUpdatedRules(const RulesStorage* rulesStorageFile);
typedef void (*pfRulesStorageFile_c_commitUpdatedRules)(const RulesStorage*);

void RulesStorageFile_c_rollbackUpdatedRules(const RulesStorage* rulesStorageFile);
typedef void (*pfRulesStorageFile_c_rollbackUpdatedRules)(const RulesStorage*);

bool RulesStorageFile_c_isUpdating(const RulesStorage* rulesStorageFile);
typedef bool (*pfRulesStorageFile_c_isUpdating)(const RulesStorage*);

time_t RulesStorageFile_c_lastUpdate(const RulesStorage* rulesStorageFile);
typedef time_t (*pfRulesStorageFile_c_lastUpdate)(const RulesStorage*);

#endif // #ifndef RULES_STORAGE_FILE_H
