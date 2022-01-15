// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CRL_STORAGE_FILE_H
#define CRL_STORAGE_FILE_H

#include <verificaC19-sdk/interfaces/CRLStorage.h>

#include <stdbool.h>
#include <time.h>

CRLStorage* CRLStorageFile_c_create();
typedef CRLStorage* (*pfCRLStorageFile_c_create)();

#endif // #ifndef CRL_STORAGE_FILE_H
