// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CERTIFICATE_SIMPLE_HPP
#define CERTIFICATE_SIMPLE_HPP

#include <verificaC19-sdk/models/CertificateStatus.hpp>

#ifdef __cplusplus

#include <string>

namespace verificaC19Sdk {

/**
 * Represent a Person
 */
class SimplePersonModel {
public:
	/**
	 * The family name(s) of the person transliterated
	 */
	std::string standardisedFamilyName;
	/**
	 * The family name(s) of the person addressed in the certificate
	 */
	std::string familyName;
	/**
	 * The given name(s) of the person transliterated
	 */
	std::string standardisedGivenName;
	/**
	 * The given name(s) of the person addressed in the certificate
	 */
	std::string givenName;
};

/**
 *
 * This data class represents the information contained in the scanned certification in an easier
 * and shorter model than [CertificateModel].
 *
 */
class CertificateSimple {
public:
	CertificateSimple() : certificateStatus(NOT_EU_DCC) {};
	/**
	 * Person informations
	 */
	SimplePersonModel person;
	/**
	 * ISO8601 Date of birth
	 */
	std::string dateOfBirth;
	/**
	 * Certificate status
	 */
	CertificateStatus certificateStatus;
	/**
	 * Certificate identifier
	 */
	std::string identifier;
	/**
	 * ISO8601 Date time of verification
	 */
	std::string timeStamp;
};

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif // #ifndef CERTIFICATE_SIMPLE_HPP
