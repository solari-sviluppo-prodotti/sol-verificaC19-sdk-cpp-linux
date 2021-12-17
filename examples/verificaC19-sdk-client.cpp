// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <dlfcn.h>
#include <unistd.h>

#include <cstring>
#include <sstream>
#include <fstream>

#include <examples/keys-storage/KeysStorageFile.hpp>
#include <examples/keys-provider/KeysProviderItaly.hpp>
#include <examples/rules-storage/RulesStorageFile.hpp>
#include <examples/rules-provider/RulesProviderItaly.hpp>
#include <examples/loggers/LoggerStdout.hpp>

#include <verificaC19-sdk/DGCVerifier.hpp>
#include <verificaC19-sdk/DGCRulesKeysUpdater.hpp>

using namespace verificaC19Sdk;

static void logCertificate(const CertificateSimple& certificate, const ILogger& logger) {
	logger.info("---------- Certificate log ----------");
	std::string certificateStatus = "";
	switch (certificate.certificateStatus) {
		case NOT_VALID: certificateStatus = "NOT_VALID"; break;
		case NOT_VALID_YET: certificateStatus = "NOT_VALID_YET"; break;
		case VALID: certificateStatus = "VALID"; break;
		case NOT_EU_DCC: certificateStatus = "NOT_EU_DCC"; break;
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
		logger.error("       where mode can be 2G for Super Green Pass, or empty for Standard Green Pass");
		logger.error("Example: verificaC19-client ./test.qr");
		logger.error("Example: verificaC19-client ./test.qr 2G");
	} else {
		logger.info("---------- Test with command line ----------");
		KeysStorageFile keysStorage;
		KeysProviderItaly keysProvider(&logger);
		RulesStorageFile rulesStorage;
		RulesProviderItaly rulesProvider(&logger);

		DGCRulesKeysUpdater rulesKeyUpdater(86400, &rulesProvider, &rulesStorage,
				&keysProvider, &keysStorage, &logger);

		// First, at startup, wait if updated
		if (!rulesKeyUpdater.isUpdated()) {
			logger.info("Rules or Keys are updating, waiting");
			while (!rulesKeyUpdater.isUpdated()) {
				usleep(10000);
			}
		}

		DGCVerifier verifier(&keysStorage, &rulesStorage, &logger);
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
				if (argc > 2 && strcmp(argv[2], "2G") == 0) {
					verifier.setScanMode(SCAN_MODE_2G);
				}
				CertificateSimple certificate = verifier.verify(qr);
				logCertificate(certificate, logger);
			}
		}
	}
}
