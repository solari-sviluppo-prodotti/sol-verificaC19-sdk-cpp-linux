// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_STORAGE_MEMORY_H
#define KEYS_STORAGE_MEMORY_H

#include <verificaC19-sdk/interfaces/KeysStorage.h>

#include <stdbool.h>
#include <time.h>

KeysStorage* KeysStorageMemory_c_create();
typedef KeysStorage* (*pfKeysStorageMemory_c_create)();

#endif // #ifndef KEYS_STORAGE_MEMORY_H
