// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CERTIFICATE_SIMPLE_HPP
#define CERTIFICATE_SIMPLE_HPP

#include <string>
#include <verificaC19-sdk/models/CertificateStatus.hpp>

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
	enum CertificateStatus certificateStatus;
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

#endif // #ifndef CERTIFICATE_SIMPLE_HPP
