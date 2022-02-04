// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <dlfcn.h>
#include <unistd.h>

#include <cstring>
#include <sstream>
#include <fstream>

#include <verificaC19-sdk/examples/keys-storage/KeysStorageFile.hpp>
#include <verificaC19-sdk/examples/keys-provider/KeysProviderItaly.hpp>
#include <verificaC19-sdk/examples/rules-storage/RulesStorageFile.hpp>
#include <verificaC19-sdk/examples/rules-provider/RulesProviderItaly.hpp>
#include <verificaC19-sdk/examples/crl-storage/CRLStorageFile.hpp>
#include <verificaC19-sdk/examples/crl-provider/CRLProviderItaly.hpp>
#include <verificaC19-sdk/examples/loggers/LoggerStdout.hpp>

#include <verificaC19-sdk/DGCVerifier.hpp>
#include <verificaC19-sdk/DGCUpdater.hpp>

using namespace verificaC19Sdk;

static void logCertificate(const CertificateSimple& certificate, const ILogger& logger) {
	logger.info("---------- Certificate log ----------");
	std::string certificateStatus = "";
	switch (certificate.certificateStatus) {
		case NOT_VALID: certificateStatus = "NOT_VALID"; break;
		case NOT_VALID_YET: certificateStatus = "NOT_VALID_YET"; break;
		case VALID: certificateStatus = "VALID"; break;
		case NOT_EU_DCC: certificateStatus = "NOT_EU_DCC"; break;
		case TEST_NEEDED: certificateStatus = "TEST_NEEDED"; break;
	}
	logger.info("Certificate status: %s", certificateStatus.c_str());
	logger.info("Person standardisedFamilyName: %s", certificate.person.standardisedFamilyName.c_str());
	logger.info("Person familyName: %s", certificate.person.familyName.c_str());
	logger.info("Person standardisedGivenName: %s", certificate.person.standardisedGivenName.c_str());
	logger.info("Person givenName: %s", certificate.person.givenName.c_str());
	logger.info("Date of birth: %s", certificate.dateOfBirth.c_str());
	logger.info("Verification timeStamp: %s", certificate.timeStamp.c_str());
}

int main (int argc, char** argv) {
	LoggerStdout logger(DEBUG);

	if (argc < 2) {
		logger.error("Usage: verificaC19-client <qrfile> [mode]");
		logger.error("       where mode can be 2G for Super Green Pass, BOOSTER for booster, WORK for work, SCHOOL for school, or empty for Standard Green Pass");
		logger.error("Example: verificaC19-client ./test.qr");
		logger.error("Example: verificaC19-client ./test.qr 2G");
		logger.error("Example: verificaC19-client ./test.qr BOOSTER");
		logger.error("Example: verificaC19-client ./test.qr WORK");
		logger.error("Example: verificaC19-client ./test.qr SCHOOL");
	} else {
		logger.info("---------- Test with command line ----------");
		KeysStorageFile keysStorage;
		KeysProviderItaly keysProvider(&logger);
		RulesStorageFile rulesStorage;
		RulesProviderItaly rulesProvider(&logger);
		CRLStorageFile crlStorage;
		CRLProviderItaly crlProvider(&logger);

		DGCUpdater updater(86400, &rulesProvider, &rulesStorage,
				&keysProvider, &keysStorage, &crlProvider, &crlStorage, &logger);

		// Force update
		// updater.forceUpdateAll();

		// First, at startup, wait if updated
		if (!updater.isUpdated()) {
			logger.info("Rules, Keys or CRL are updating, waiting");
			while (!updater.isUpdated()) {
				usleep(10000);
			}
		}

		DGCVerifier verifier(&keysStorage, &rulesStorage, &crlStorage, &logger);
		if (!verifier.verifyMinSdkVersion()) {
			logger.error("Minimum SDK version does not match");
		} else {
			std::ifstream fcertificate;
			fcertificate.open(argv[1]);
			if (!fcertificate.is_open()) {
				logger.error("No certificate file opened");
			} else {
				std::stringstream qrs;
				qrs << fcertificate.rdbuf();
				fcertificate.close();
				std::string qr = qrs.str();
				const char* scanMode = SCAN_MODE_3G;
				if (argc > 2 && strcmp(argv[2], "2G") == 0) {
					scanMode = SCAN_MODE_2G;
				}
				if (argc > 2 && strcmp(argv[2], "BOOSTER") == 0) {
					scanMode = SCAN_MODE_BOOSTER;
				}
				if (argc > 2 && strcmp(argv[2], "WORK") == 0) {
					scanMode = SCAN_MODE_WORK;
				}
				if (argc > 2 && strcmp(argv[2], "SCHOOL") == 0) {
					scanMode = SCAN_MODE_SCHOOL;
				}
				CertificateSimple certificate = verifier.verify(qr, scanMode);
				logCertificate(certificate, logger);
			}
		}
	}
}
