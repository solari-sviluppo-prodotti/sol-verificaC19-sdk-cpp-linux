// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CRL_STORAGE_H
#define CRL_STORAGE_H

typedef struct {} CRLStorage;

void CRLStorage_c_release(const CRLStorage* crlStorage);
typedef void (*pfCRLStorage_c_release)(const CRLStorage*);

bool CRLStorage_c_hasCertificateHash(const CRLStorage* crlStorage, const char* certificateHash);
typedef bool (*pfCRLStorage_c_hasCertificateHash)(const CRLStorage*, const char*);

void CRLStorage_c_beginUpdatingCRL(const CRLStorage* crlStorage);
typedef void (*pfCRLStorage_c_beginUpdatingCRL)(const CRLStorage*);

void CRLStorage_c_storeChunk(const CRLStorage* crlStorage, const char* chunk);
typedef void (*pfCRLStorage_c_storeChunk)(const CRLStorage*, const char*);

void CRLStorage_c_clearCRL(const CRLStorage* crlStorage);
typedef void (*pfCRLStorage_c_clearCRL)(const CRLStorage*);

void CRLStorage_c_commitUpdatedCRL(const CRLStorage* crlStorage);
typedef void (*pfCRLStorage_c_commitUpdatedCRL)(const CRLStorage*);

void CRLStorage_c_rollbackUpdatedCRL(const CRLStorage* crlStorage);
typedef void (*pfCRLStorage_c_rollbackUpdatedCRL)(const CRLStorage*);

bool CRLStorage_c_isUpdating(const CRLStorage* crlStorage);
typedef bool (*pfCRLStorage_c_isUpdating)(const CRLStorage*);

int CRLStorage_c_version(const CRLStorage* crlStorage);
typedef int (*pfCRLStorage_c_version)(const CRLStorage*);

const char* CRLStorage_c_downloadInfo(const CRLStorage* crlStorage);
typedef const char* (*pfCRLStorage_c_downloadInfo)(const CRLStorage*);

time_t CRLStorage_c_lastUpdate(const CRLStorage* crlStorage);
typedef time_t (*pfCRLStorage_c_lastUpdate)(const CRLStorage*);

#endif // #ifndef CRL_STORAGE_H
