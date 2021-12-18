// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CERTIFICATE_SIMPLE_H
#define CERTIFICATE_SIMPLE_H

#include <verificaC19-sdk/models/CertificateStatus.h>

/**
 *
 * This data class represents the information contained in the scanned certification for calls by C language.
 *
 */
struct CertificateSimple_c {
	/**
	 * Person informations
	 */
	struct {
		/**
		 * The family name(s) of the person transliterated
		 */
		char* standardisedFamilyName;
		/**
		 * The family name(s) of the person addressed in the certificate
		 */
		char* familyName;
		/**
		 * The given name(s) of the person transliterated
		 */
		char* standardisedGivenName;
		/**
		 * The given name(s) of the person addressed in the certificate
		 */
		char* givenName;
	} person;
	/**
	 * ISO8601 Date of birth
	 */
	char* dateOfBirth;
	/**
	 * Certificate status
	 */
	enum CertificateStatus certificateStatus;
	/**
	 * Certificate identifier
	 */
	char* identifier;
	/**
	 * ISO8601 Date time of verification
	 */
	char* timeStamp;
};

#endif // #ifndef CERTIFICATE_SIMPLE_H
