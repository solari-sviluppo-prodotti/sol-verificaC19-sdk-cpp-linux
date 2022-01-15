// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <verificaC19-sdk/examples/keys-storage/KeysStorageFile.h>
#include <verificaC19-sdk/examples/keys-provider/KeysProviderItaly.h>
#include <verificaC19-sdk/examples/rules-storage/RulesStorageFile.h>
#include <verificaC19-sdk/examples/rules-provider/RulesProviderItaly.h>
#include <verificaC19-sdk/examples/crl-storage/CRLStorageFile.h>
#include <verificaC19-sdk/examples/crl-provider/CRLProviderItaly.h>
#include <verificaC19-sdk/examples/loggers/LoggerStdout.h>

#include <verificaC19-sdk/DGCVerifier.h>
#include <verificaC19-sdk/DGCUpdater.h>

static void logCertificate(const struct CertificateSimple_c* certificate, const Logger* logger) {
	LoggerStdout_c_info(logger, "---------- Certificate log ----------");
	char* certificateStatus = "";
	switch (certificate->certificateStatus) {
		case NOT_VALID: certificateStatus = "NOT_VALID"; break;
		case NOT_VALID_YET: certificateStatus = "NOT_VALID_YET"; break;
		case VALID: certificateStatus = "VALID"; break;
		case NOT_EU_DCC: certificateStatus = "NOT_EU_DCC"; break;
		case TEST_NEEDED: certificateStatus = "TEST_NEEDED"; break;
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
		LoggerStdout_c_error(logger, "       where mode can be 2G for Super Green Pass, BOOSTER for booster, or empty for Standard Green Pass");
		LoggerStdout_c_error(logger, "Example: verificaC19-c-client ./test.qr");
		LoggerStdout_c_error(logger, "Example: verificaC19-c-client ./test.qr 2G");
		LoggerStdout_c_error(logger, "Example: verificaC19-c-client ./test.qr BOOSTER");
	} else {
		LoggerStdout_c_info(logger, "---------- Test with command line ----------");
		KeysStorage* keysStorage = KeysStorageFile_c_create();
		KeysProvider* keysProvider = KeysProviderItaly_c_create(logger);
		RulesStorage* rulesStorage = RulesStorageFile_c_create();
		RulesProvider* rulesProvider = RulesProviderItaly_c_create(logger);
		CRLStorage* crlStorage = CRLStorageFile_c_create();
		CRLProvider* crlProvider = CRLProviderItaly_c_create(logger);

		Updater* updater = DGCUpdater_c_create(86400, rulesProvider, rulesStorage,
				keysProvider, keysStorage, crlProvider, crlStorage, logger);

		// Force update
		DGCUpdater_c_forceUpdateAll(updater);

		// First, at startup, wait if updated
		if (!DGCUpdater_c_isUpdated(updater)) {
			LoggerStdout_c_info(logger, "Rules, Keys or CRL are updating, waiting");
			while (!DGCUpdater_c_isUpdated(updater)) {
				usleep(10000);
			}
		}

		void* verifier = DGCVerifier_c_create(keysStorage, rulesStorage, crlStorage, logger);
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
						const char* scanMode = SCAN_MODE_3G;
						if (argc > 2 && strcmp(argv[2], "2G") == 0) {
							scanMode = SCAN_MODE_2G;
						}
						if (argc > 2 && strcmp(argv[2], "BOOSTER") == 0) {
							scanMode = SCAN_MODE_BOOSTER;
						}
						struct CertificateSimple_c* certificate = DGCVerifier_c_verify(verifier, qr, scanMode);
						free(qr);
						logCertificate(certificate, logger);
						CertificateSimple_c_release(certificate);
					}
				}
			}
		}
		DGCVerifier_c_release(verifier);
		DGCUpdater_c_release(updater);
		RulesProvider_c_release(rulesProvider);
		RulesStorage_c_release(rulesStorage);
		KeysProvider_c_release(keysProvider);
		KeysStorage_c_release(keysStorage);
	}
}
