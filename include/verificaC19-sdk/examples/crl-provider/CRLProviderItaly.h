// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CRL_PROVIDER_ITALY_H
#define CRL_PROVIDER_ITALY_H

#include <verificaC19-sdk/interfaces/CRLProvider.h>
#include <verificaC19-sdk/interfaces/CRLStorage.h>
#include <verificaC19-sdk/interfaces/Logger.h>

CRLProvider* CRLProviderItaly_c_create(Logger* logger);
typedef CRLProvider* (*pfCRLProviderItaly_c_create)(Logger*);

#endif // #ifndef CRL_PROVIDER_ITALY_H
