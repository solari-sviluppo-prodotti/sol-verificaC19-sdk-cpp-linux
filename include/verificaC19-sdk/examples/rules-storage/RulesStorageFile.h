// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_STORAGE_FILE_H
#define RULES_STORAGE_FILE_H

#include <verificaC19-sdk/interfaces/RulesStorage.h>

#include <stdbool.h>
#include <time.h>

RulesStorage* RulesStorageFile_c_create();
typedef RulesStorage* (*pfRulesStorageFile_c_create)(void*);

#endif // #ifndef RULES_STORAGE_FILE_H
