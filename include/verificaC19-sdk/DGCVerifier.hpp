// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef DGC_VERIFIER_HPP
#define DGC_VERIFIER_HPP

#include <verificaC19-sdk/interfaces/IKeysStorage.hpp>
#include <verificaC19-sdk/interfaces/IRulesStorage.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>
#include <verificaC19-sdk/models/CertificateSimple.hpp>

namespace verificaC19Sdk {

#define DGCVerifier_SDK_Version "1.1.1"

/**
 * Implements Digital Certificate verification
 */
class DGCVerifier {

public:

	/**
	 * @param keysStorage Implementation of Keys Storage service, used to retrieve
	 *                    key needed to verify Digital Certificate validity
	 *
	 * @param rulesStorage Implementation of Rules Storage service, used to retrieve
	 *                     rules needed to verify Digital Certificate validity
	 *
	 * @param logger Optional implementation of Logger, if NULL no log will be done
	 */
	DGCVerifier(IKeysStorage* keysStorage, IRulesStorage* rulesStorage, ILogger* logger);

	/**
	 * Set scan mode, default scan mode at object construction is SCAN_MODE_3G
	 *
	 * @param scanMode Scan mode, SCAN_MODE_STRENGTHENED accept only vaccination and recovery
	 *                 certificates, SCAN_MODE_STANDARD accept also test certificates,
	 *                 SCAN_MODE_BOOSTER signals test needed for complete vaccination (not booster)
	 *                 or for recovery certificate
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

} // extern "C"

} // namespace verificaC19Sdk

extern "C" {

#include <verificaC19-sdk/DGCVerifier.h>

} // extern "C"

#endif // #ifndef DGC_VERIFIER_HPP
