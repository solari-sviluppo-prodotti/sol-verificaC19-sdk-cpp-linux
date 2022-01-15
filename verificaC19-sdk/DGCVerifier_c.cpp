// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <string.h>
#include <stdlib.h>
#include <verificaC19-sdk/DGCVerifier.hpp>

extern "C" {

#include <verificaC19-sdk/DGCVerifier.h>

}

Verifier* DGCVerifier_c_create(KeysStorage* keysStorage, RulesStorage* rulesStorage, CRLStorage* crlStorage, Logger* logger) {
	return (Verifier*)DGCVerifier_create((verificaC19Sdk::IKeysStorage*)keysStorage,
			(verificaC19Sdk::IRulesStorage*)rulesStorage,
			(verificaC19Sdk::ICRLStorage*)crlStorage,
			(verificaC19Sdk::ILogger*)logger);
}

void DGCVerifier_c_release(Verifier* dgcVerifier) {
	DGCVerifier_release((verificaC19Sdk::DGCVerifier*)dgcVerifier);
}

bool DGCVerifier_c_verifyMinSdkVersion(Verifier* dgcVerifier) {
	return DGCVerifier_verifyMinSdkVersion((verificaC19Sdk::DGCVerifier*)dgcVerifier);
}

struct CertificateSimple_c* DGCVerifier_c_verify(Verifier* dgcVerifier, const char* dgcQr, const char* scanMode) {
	verificaC19Sdk::CertificateSimple certificate = DGCVerifier_verify((verificaC19Sdk::DGCVerifier*)dgcVerifier, std::string(dgcQr), std::string(scanMode));
	struct CertificateSimple_c* certificate_c = (struct CertificateSimple_c*)calloc(1, sizeof(struct CertificateSimple_c));
	certificate_c->person.standardisedFamilyName = (char*)calloc(1, certificate.person.standardisedFamilyName.length() + 1);
	strcpy(certificate_c->person.standardisedFamilyName, certificate.person.standardisedFamilyName.c_str());
	certificate_c->person.familyName = (char*)calloc(1, certificate.person.familyName.length() + 1);
	strcpy(certificate_c->person.familyName, certificate.person.familyName.c_str());
	certificate_c->person.standardisedGivenName = (char*)calloc(1, certificate.person.standardisedGivenName.length() + 1);
	strcpy(certificate_c->person.standardisedGivenName, certificate.person.standardisedGivenName.c_str());
	certificate_c->person.givenName = (char*)calloc(1, certificate.person.givenName.length() + 1);
	strcpy(certificate_c->person.givenName, certificate.person.givenName.c_str());
	certificate_c->dateOfBirth = (char*)calloc(1, certificate.dateOfBirth.length() + 1);
	strcpy(certificate_c->dateOfBirth, certificate.dateOfBirth.c_str());
	certificate_c->certificateStatus = certificate.certificateStatus;
	certificate_c->identifier = (char*)calloc(1, certificate.identifier.length() + 1);
	strcpy(certificate_c->identifier, certificate.identifier.c_str());
	certificate_c->timeStamp = (char*)calloc(1, certificate.timeStamp.length() + 1);
	strcpy(certificate_c->timeStamp, certificate.timeStamp.c_str());
	return certificate_c;
}

void CertificateSimple_c_release(struct CertificateSimple_c* certificate) {
	free(certificate->timeStamp);
	free(certificate->identifier);
	free(certificate->dateOfBirth);
	free(certificate->person.givenName);
	free(certificate->person.standardisedGivenName);
	free(certificate->person.familyName);
	free(certificate->person.standardisedFamilyName);
	free(certificate);
}
