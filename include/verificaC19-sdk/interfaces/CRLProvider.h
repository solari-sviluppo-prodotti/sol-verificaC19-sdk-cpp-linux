// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CRL_PROVIDER_H
#define CRL_PROVIDER_H

#include <verificaC19-sdk/interfaces/CRLStorage.h>

typedef struct {} CRLProvider;

void CRLProvider_c_release(const CRLProvider* crlProvider);
typedef void (*pfCRLProvider_c_release)(const CRLProvider*);

void CRLProvider_c_refreshCRL(const CRLProvider* crlProvider, CRLStorage* crlStorage);
typedef void (*pfCRLProvider_c_refreshKeys)(const CRLProvider*, CRLStorage*);

#endif // #ifndef CRL_PROVIDER_H
