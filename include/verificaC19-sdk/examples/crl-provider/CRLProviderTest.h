// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CRL_PROVIDER_TEST_H
#define CRL_PROVIDER_TEST_H

#include <verificaC19-sdk/interfaces/CRLProvider.h>
#include <verificaC19-sdk/interfaces/CRLStorage.h>
#include <verificaC19-sdk/interfaces/Logger.h>

CRLProvider* CRLProviderTest_c_create(Logger* logger);
typedef CRLProvider* (*pfCRLProviderTest_c_create)(Logger*);

#endif // #ifndef CRL_PROVIDER_TEST_H
