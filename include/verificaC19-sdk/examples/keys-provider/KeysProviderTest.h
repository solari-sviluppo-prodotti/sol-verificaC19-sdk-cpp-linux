// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_PROVIDER_TEST_H
#define KEYS_PROVIDER_TEST_H

#include <verificaC19-sdk/interfaces/KeysProvider.h>
#include <verificaC19-sdk/interfaces/KeysStorage.h>
#include <verificaC19-sdk/interfaces/Logger.h>

KeysProvider* KeysProviderTest_c_create(Logger* logger);
typedef KeysProvider* (*pfKeysProviderTest_c_create)(Logger*);

#endif // #ifndef KEYS_PROVIDER_TEST_H
