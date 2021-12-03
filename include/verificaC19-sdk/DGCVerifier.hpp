// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef DGC_VERIFIER_HPP
#define DGC_VERIFIER_HPP

#include <verificaC19-sdk/interfaces/IKeysStorage.hpp>
#include <verificaC19-sdk/interfaces/IRulesStorage.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>
#include <verificaC19-sdk/models/CertificateSimple.hpp>

#define SCAN_MODE_2G   "2G"
#define SCAN_MODE_3G   "3G"

#ifdef __cplusplus

namespace verificaC19Sdk {

#define DGCVerifier_SDK_Version "1.0.4"

/**
 * Implements Digital Certificate verification
 */
class DGCVerifier {

public:

	/**
	 * @param keysStorage Implementation of Keys Storage service, used to retreive
	 *                    key needed to verify Digital Certificate validity
	 *
	 * @param rulesStorage Implementation of Rules Storage service, used to retreive
	 *                     rules needed to verify Digital Certificate validity
	 *
	 * @param logger Optional implementation of Logger, if NULL no log will be done
	 */
	DGCVerifier(IKeysStorage* keysStorage, IRulesStorage* rulesStorage, ILogger* logger);

	/**
	 * Set scan mode, default scan mode at object construction is SCAN_MODE_3G
	 *
	 * @param scanMode Scan mode, SCAN_MODE_2G accept only vaccination and recovery
	 *                 certificatesl, SCAN_MODE_3G accept also test certificates
	 */
	void setScanMode(const std::string& scanMode);

	/**
	 * Check if minimum SDK version rule is respected
	 *
	 * @return true if minimum SDK version rule is respected, false if needs to update SDK.
	 */
	bool verifyMinSdkVersion() const;

	/**
	 * Verify Digital Certificate
	 *
	 * @param dgcQr Digital Certificate to verify (raw qr code data starting with HC1:)
	 *
	 * @return CertificateSimple Result of Digital Certification verify
	 */
	CertificateSimple verify(const std::string& dgcQr) const;

private:

	std::string m_scanMode;
	IKeysStorage* m_keysStorage;
	IRulesStorage* m_rulesStorage;
	ILogger* m_logger;

}; // class DGCVerifier

extern "C" {

/**
 * DGCVerifier constructor interface for dynamic loading
 */
DGCVerifier* DGCVerifier_create(IKeysStorage* keysStorage, IRulesStorage* rulesStorage, ILogger* logger);
typedef DGCVerifier* (*pfDGCVerifier_create)(IKeysStorage*, IRulesStorage*, ILogger*);

/**
 * DGCVerifier destructor interface for dynamic loading
 */
void DGCVerifier_release(DGCVerifier* dgcVerifier);
typedef void (*pfDGCVerifier_release)(DGCVerifier*);

/**
 * DGCVerifier verifyMinSdkVersion interface for dynamic loading
 */
bool DGCVerifier_verifyMinSdkVersion(DGCVerifier* dgcVerifier);
typedef bool (*pfDGCVerifier_verifyMinSdkVersion)(DGCVerifier*);

/**
 * DGCVerifier setScanMode interface for dynamic loading
 */
void DGCVerifier_setScanMode(DGCVerifier* dgcVerifier, const std::string& scanMode);
typedef void (*pfDGCVerifier_setScanMode)(DGCVerifier*, const std::string&);

/**
 * DGCVerifier verify interface for dynamic loading
 */
CertificateSimple DGCVerifier_verify(DGCVerifier* dgcVerifier, const std::string& dgcQr);
typedef CertificateSimple (*pfDGCVerifier_verify)(DGCVerifier*, const std::string);

}

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif // #ifndef DGC_VERIFIER_HPP
