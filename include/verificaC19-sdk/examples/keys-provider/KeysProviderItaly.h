// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_PROVIDER_ITALY_H
#define KEYS_PROVIDER_ITALY_H

#include <verificaC19-sdk/interfaces/KeysProvider.h>
#include <verificaC19-sdk/interfaces/KeysStorage.h>
#include <verificaC19-sdk/interfaces/Logger.h>

KeysProvider* KeysProviderItaly_c_create(Logger* logger);
typedef KeysProvider* (*pfKeysProviderItaly_c_create)(Logger*);

#endif // #ifndef KEYS_PROVIDER_ITALY_H
