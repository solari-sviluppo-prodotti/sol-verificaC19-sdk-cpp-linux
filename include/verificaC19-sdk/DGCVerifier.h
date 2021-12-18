// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef DGC_VERIFIER_H
#define DGC_VERIFIER_H

#include <verificaC19-sdk/models/CertificateSimple.h>

#define SCAN_MODE_2G   "2G"
#define SCAN_MODE_3G   "3G"

/**
 * DGCVerifier constructor interface for C and for dynamic loading
 */
void* DGCVerifier_c_create(void* keysStorage, void* rulesStorage, void* logger);
typedef void* (*pfDGCVerifier_c_create)(void*, void*, void*);

/**
 * DGCVerifier destructor interface for C and for dynamic loading
 */
void DGCVerifier_c_release(void* dgcVerifier);
typedef void (*pfDGCVerifier_c_release)(void*);

/**
 * DGCVerifier verifyMinSdkVersion interface for C and for dynamic loading
 */
bool DGCVerifier_c_verifyMinSdkVersion(void* dgcVerifier);
typedef bool (*pfDGCVerifier_c_verifyMinSdkVersion)(void*);

/**
 * DGCVerifier setScanMode interface for C and for dynamic loading
 */
void DGCVerifier_c_setScanMode(void* dgcVerifier, const char* scanMode);
typedef void (*pfDGCVerifier_c_setScanMode)(void*, const char*);

/**
 * DGCVerifier verify interface for C and for dynamic loading
 */
struct CertificateSimple_c* DGCVerifier_c_verify(void* dgcVerifier, const char* dgcQr);
typedef struct CertificateSimple_c* (*pfDGCVerifier_c_verify)(void*, const char*);

/**
 * Release object created by DGCVerifier_c_verify
 */
void CertificateSimple_c_release(struct CertificateSimple_c* certificate);
typedef void (*pfCertificateSimple_c_release)(struct CertificateSimple_c*);

#endif // #ifndef DGC_VERIFIER_H
