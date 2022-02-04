// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CERTIFICATE_MODEL_HPP
#define CERTIFICATE_MODEL_HPP

#include <verificaC19-sdk/models/CertificateStatus.hpp>

#include <string>

namespace verificaC19Sdk {

/**
 * Represent Exemption data
 */
class ExemptionModel {
public:
	/**
	 * ISO8601 Date of certificate validity start
	 */
	std::string certificateValidFrom;
	/**
	 * ISO8601 Date of certificate validity end
	 */
	std::string certificateValidUntil;
};

/**
 * Represent Recovery data
 */
class RecoveryModel {
public:
	/**
	 * ISO8601 Date of first positive test
	 */
	std::string dateOfFirstPositiveTest;
	/**
	 * ISO8601 Date of certificate validity start
	 */
	std::string certificateValidFrom;
	/**
	 * ISO8601 Date of certificate validity end
	 */
	std::string certificateValidUntil;
};

typedef std::string TestType;
static const TestType RAPID("LP217198-3");
static const TestType MOLECULAR("LP6464-4");

typedef std::string TestResult;
static const TestResult NOT_DETECTED("260415000");

/**
 * Represent Test data
 */
class TestModel {
public:
	/**
	 * Test type (LP217198-3 - RAPID or LP6464-4 - MOLECULAR)
	 */
	TestType typeOfTest;
	/**
	 * Test name
	 */
	std::string testName;
	/**
	 * Test Manufacturer
	 */
	std::string testManufacturer;
	/**
	 * ISO8601 Date and time of collection
	 */
	std::string dateTimeOfCollection;
	/**
	 * ISO8601 Date and time of result
	 */
	std::string dateTimeOfTestResult;
	/**
	 * Test result (260415000 - NOT DETECTED)
	 */
	TestResult testResult;
	/**
	 * Testing centre
	 */
	std::string testingCentre;
};

/**
 * Represent Vaccination data
 */
class VaccinationModel {
public:
	VaccinationModel() : doseNumber(0), totalSeriesOfDoses(0) {}
	/**
	 * Vaccine name
	 */
	std::string vaccine;
	/**
	 * Vaccine product
	 */
	std::string medicinalProduct;
	/**
	 * Vaccine manufacturer
	 */
	std::string manufacturer;
	/**
	 * Dose number
	 */
	int doseNumber;
	/**
	 * Total series of doses initially expected
	 */
	int totalSeriesOfDoses;
	/**
	 * ISO8601 Date of vaccination
	 */
	std::string dateOfVaccination;
};

/**
 * Represent a Person
 */
class PersonModel {
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
 * Represent a European Digital Green Certificate
 */
class CertificateModel {
public:
	CertificateModel() {}
	/**
	 * Check if is a Digital Certificate of Vaccination
	 *
	 * @return true if is a Digital Certificate of Vaccination, else false
	 */
	bool isVaccination() const { return !vaccination.dateOfVaccination.empty(); }
	/**
	 * Check if is a Digital Certificate of Test
	 *
	 * @return true if is a Digital Certificate of Test, else false
	 */
	bool isTest() const { return !test.dateTimeOfCollection.empty(); }
	/**
	 * Check if is a Digital Certificate of Recovery
	 *
	 * @return true if is a Digital Certificate of Recovery, else false
	 */
	bool isRecovery() const { return !recoveryStatement.certificateValidFrom.empty(); }
	/**
	 * Check if is a Digital Certificate of Exemption
	 *
	 * @return true if is a Digital Certificate of Exemption, else false
	 */
	bool isExemption() const { return !exemption.certificateValidFrom.empty(); }
	/**
	 * Person informations
	 */
	PersonModel person;
	/**
	 * ISO8601 Date of birth
	 */
	std::string dateOfBirth;
	/**
	 * Vaccination informations
	 */
	VaccinationModel vaccination;
	/**
	 * Test informations
	 */
	TestModel test;
	/**
	 * Recovery informations
	 */
	RecoveryModel recoveryStatement;
	/**
	 * Exemption informations
	 */
	ExemptionModel exemption;
	/**
	 * Disease type (840539006 - COVID-19)
	 */
	std::string disease;
	/**
	 * Country of vaccination
	 */
	std::string country;
	/**
	 * Digital Certificate issuer
	 */
	std::string issuer;
	/**
	 * Digital Certificate identifier
	 */
	std::string identifier;
	/**
	 * Digital Certificate version
	 */
	std::string version;
	/**
	 * Digital Certificate country issuer
	 */
	std::string countryIssuer;
	/**
	 * Digital Certificate generation date
	 */
	std::string dateTimeOfGeneration;
	/**
	 * Digital Certificate expiration date
	 */
	std::string dateTimeOfExpiration;
};

} // namespace verificaC19Sdk

#endif // #ifndef CERTIFICATE_MODEL_HPP
