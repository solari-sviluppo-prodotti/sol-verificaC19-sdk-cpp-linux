// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <examples/keys-storage/KeysStorageFile.h>
#include <examples/keys-provider/KeysProviderItaly.h>
#include <examples/rules-storage/RulesStorageFile.h>
#include <examples/rules-provider/RulesProviderItaly.h>
#include <examples/loggers/LoggerStdout.h>

#include <verificaC19-sdk/DGCVerifier.h>
#include <verificaC19-sdk/DGCRulesKeysUpdater.h>

static void logCertificate(const struct CertificateSimple_c* certificate, const Logger* logger) {
	LoggerStdout_c_info(logger, "---------- Certificate log ----------");
	char* certificateStatus = "";
	switch (certificate->certificateStatus) {
		case NOT_VALID: certificateStatus = "NOT_VALID"; break;
		case NOT_VALID_YET: certificateStatus = "NOT_VALID_YET"; break;
		case VALID: certificateStatus = "VALID"; break;
		case NOT_EU_DCC: certificateStatus = "NOT_EU_DCC"; break;
	}
	LoggerStdout_c_info(logger, "Certificate status: %s", certificateStatus);
	LoggerStdout_c_info(logger, "Person standardisedFamilyName: %s", certificate->person.standardisedFamilyName);
	LoggerStdout_c_info(logger, "Person familyName: %s", certificate->person.familyName);
	LoggerStdout_c_info(logger, "Person standardisedGivenName: %s", certificate->person.standardisedGivenName);
	LoggerStdout_c_info(logger, "Person givenName: %s", certificate->person.givenName);
	LoggerStdout_c_info(logger, "Date of birth: %s", certificate->dateOfBirth);
	LoggerStdout_c_info(logger, "Verification timeStamp: %s", certificate->timeStamp);
}

int main (int argc, char** argv) {
	Logger* logger = LoggerStdout_c_create(DEBUG);

	if (argc < 2) {
		LoggerStdout_c_error(logger, "Usage: verificaC19-c-client <qrfile> [mode]");
		LoggerStdout_c_error(logger, "       where mode can be 2G for Super Green Pass, or empty for Standard Green Pass");
		LoggerStdout_c_error(logger, "Example: verificaC19-c-client ./test.qr");
		LoggerStdout_c_error(logger, "Example: verificaC19-c-client ./test.qr 2G");
	} else {
		LoggerStdout_c_info(logger, "---------- Test with command line ----------");
		KeysStorage* keysStorage = KeysStorageFile_c_create();
		KeysProvider* keysProvider = KeysProviderItaly_c_create(logger);
		RulesStorage* rulesStorage = RulesStorageFile_c_create();
		RulesProvider* rulesProvider = RulesProviderItaly_c_create(logger);

		RulesKeysUpdater* rulesKeysUpdater = DGCRulesKeysUpdaterRulesAndKeys_c_create(86400, rulesProvider, rulesStorage,
				keysProvider, keysStorage, logger);

		// First, at startup, wait if updated
		if (!DGCRulesKeysUpdater_c_isUpdated(rulesKeysUpdater)) {
			LoggerStdout_c_info(logger, "Rules or Keys are updating, waiting");
			while (!DGCRulesKeysUpdater_c_isUpdated(rulesKeysUpdater)) {
				usleep(10000);
			}
		}

		void* verifier = DGCVerifier_c_create(keysStorage, rulesStorage, logger);
		if (!DGCVerifier_c_verifyMinSdkVersion(verifier)) {
			LoggerStdout_c_error(logger, "Minimum SDK version does not match");
		} else {
			int fcertificate = open(argv[1], O_RDONLY);
			if (fcertificate == -1) {
				LoggerStdout_c_error(logger, "No certificate file opened");
			} else {
				struct stat sb;
				if (fstat(fcertificate, &sb) == -1) {
					LoggerStdout_c_error(logger, "Error get certificate file size");
					close(fcertificate);
				} else {
					char* qr = calloc(1, sb.st_size + 1);
					if (read(fcertificate, qr, sb.st_size) != sb.st_size) {
						LoggerStdout_c_error(logger, "Error reading certificate file");
						close(fcertificate);
						free(qr);
					} else {
						close(fcertificate);
						if (argc > 2 && strcmp(argv[2], "2G") == 0) {
							DGCVerifier_c_setScanMode(verifier, SCAN_MODE_2G);
						}
						struct CertificateSimple_c* certificate = DGCVerifier_c_verify(verifier, qr);
						free(qr);
						logCertificate(certificate, logger);
						CertificateSimple_c_release(certificate);
					}
				}
			}
		}
		DGCVerifier_c_release(verifier);
		DGCRulesKeysUpdater_c_release(rulesKeysUpdater);
		RulesProviderItaly_c_release(rulesProvider);
		RulesStorageFile_c_release(rulesStorage);
		KeysProviderItaly_c_release(keysProvider);
		KeysStorageFile_c_release(keysStorage);
	}
}
