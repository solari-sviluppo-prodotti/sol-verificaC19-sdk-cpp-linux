// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_STORAGE_FILE_H
#define KEYS_STORAGE_FILE_H

#include <stdbool.h>
#include <time.h>

void* KeysStorageFile_c_create();
typedef void* (*pfKeysStorageFile_c_create)();

void KeysStorageFile_c_release(const void* keysStorageMemory);
typedef void (*pfKeysStorageFile_c_release)(const void*);

const char* KeysStorageFile_c_getKey(const void* keysStorageMemory, const char* kid);
typedef const char* (*pfKeysStorageFile_c_getKey)(const void*, const char*);

void KeysStorageFile_c_beginUpdatingKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageFile_c_beginUpdatingKeys)(const void*);

void KeysStorageFile_c_storeKey(const void* keysStorageMemory, const char* kid, const char* ecx);
typedef void (*pfKeysStorageFile_c_storeKey)(const void*, const char*, const char*);

void KeysStorageFile_c_commitUpdatedKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageFile_c_commitUpdatedKeys)(const void*);

void KeysStorageFile_c_rollbackUpdatedKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageFile_c_rollbackUpdatedKeys)(const void*);

bool KeysStorageFile_c_isUpdating(const void* keysStorageMemory);
typedef bool (*pfKeysStorageFile_c_isUpdating)(const void*);

time_t KeysStorageFile_c_lastUpdate(const void* keysStorageMemory);
typedef time_t (*pfKeysStorageFile_c_lastUpdate)(const void*);

void KeysStorageFile_c_setLastStoredKeyToken(const void* keysStorageMemory, const char* token);
typedef void (*pfKeysStorageFile_c_setLastStoredKeyToken)(const void*, const char*);

const char* KeysStorageFile_c_getLastStoredKeyToken(const void* keysStorageMemory);
typedef const char* (*pfKeysStorageFile_c_getLastStoredKeyToken)(const void*);

#endif // #ifndef KEYS_STORAGE_FILE_H
