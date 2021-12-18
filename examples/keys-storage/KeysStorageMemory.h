// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_STORAGE_MEMORY_H
#define KEYS_STORAGE_MEMORY_H

#include <stdbool.h>
#include <time.h>

void* KeysStorageMemory_c_create();
typedef void* (*pfKeysStorageMemory_c_create)();

void KeysStorageMemory_c_release(const void* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_release)(const void*);

const char* KeysStorageMemory_c_getKey(const void* keysStorageMemory, const char* kid);
typedef const char* (*pfKeysStorageMemory_c_getKey)(const void*, const char*);

void KeysStorageMemory_c_beginUpdatingKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_beginUpdatingKeys)(const void*);

void KeysStorageMemory_c_storeKey(const void* keysStorageMemory, const char* kid, const char* ecx);
typedef void (*pfKeysStorageMemory_c_storeKey)(const void*, const char*, const char*);

void KeysStorageMemory_c_commitUpdatedKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_commitUpdatedKeys)(const void*);

void KeysStorageMemory_c_rollbackUpdatedKeys(const void* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_rollbackUpdatedKeys)(const void*);

bool KeysStorageMemory_c_isUpdating(const void* keysStorageMemory);
typedef bool (*pfKeysStorageMemory_c_isUpdating)(const void*);

time_t KeysStorageMemory_c_lastUpdate(const void* keysStorageMemory);
typedef time_t (*pfKeysStorageMemory_c_lastUpdate)(const void*);

void KeysStorageMemory_c_setLastStoredKeyToken(const void* keysStorageMemory, const char* token);
typedef void (*pfKeysStorageMemory_c_setLastStoredKeyToken)(const void*, const char*);

const char* KeysStorageMemory_c_getLastStoredKeyToken(const void* keysStorageMemory);
typedef const char* (*pfKeysStorageMemory_c_getLastStoredKeyToken)(const void*);

#endif // #ifndef KEYS_STORAGE_MEMORY_H
