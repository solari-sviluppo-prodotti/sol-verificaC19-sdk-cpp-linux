// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_STORAGE_MEMORY_H
#define RULES_STORAGE_MEMORY_H

#include <verificaC19-sdk/interfaces/RulesStorage.h>

#include <stdbool.h>
#include <time.h>

RulesStorage* RulesStorageMemory_c_create();
typedef RulesStorage* (*pfRulesStorageMemory_c_create)(void*);

#endif // #ifndef RULES_STORAGE_MEMORY_H
