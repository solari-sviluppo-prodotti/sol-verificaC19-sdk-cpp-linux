// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <dlfcn.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

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
		case EXPIRED: certificateStatus = "EXPIRED"; break;
	}
	logger.info("Certificate status: %s", certificateStatus.c_str());
	logger.info("Person standardisedFamilyName: %s", certificate.person.standardisedFamilyName.c_str());
	logger.info("Person familyName: %s", certificate.person.familyName.c_str());
	logger.info("Person standardisedGivenName: %s", certificate.person.standardisedGivenName.c_str());
	logger.info("Person givenName: %s", certificate.person.givenName.c_str());
	logger.info("Date of birth: %s", certificate.dateOfBirth.c_str());
	logger.info("Verification timeStamp: %s", certificate.timeStamp.c_str());
}

class LoggerFd : public ILogger {

public:
	LoggerFd(int fd, LogLevel level) { m_fd = fd; m_level = level; }
	void error(const std::string& format, ...) const;
	void info(const std::string& format, ...) const;
	void debug(const std::string& format, ...) const;
	void log(LogLevel level, const std::string& format, va_list ap) const;
private:
	int m_fd;
	LogLevel m_level;
};

void LoggerFd::error(const std::string& format, ...) const {
	va_list args1;
	va_start(args1, format);
	log(ERROR, format, args1);
	va_end(args1);
}

void LoggerFd::info(const std::string& format, ...) const {
	if (m_level < INFO) return;
	va_list args1;
	va_start(args1, format);
	log(INFO, format, args1);
	va_end(args1);
}

void LoggerFd::debug(const std::string& format, ...) const {
	va_list args1;
	va_start(args1, format);
	log(DEBUG, format, args1);
	va_end(args1);
}

void LoggerFd::log(LogLevel level, const std::string& format, va_list ap) const {
	if (m_level < level) return;
	//Time
	struct timeval rawtime;
	gettimeofday(&rawtime, NULL);
	struct tm* info=localtime(&rawtime.tv_sec);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%d/%m %H:%M:%S", info);
	write(m_fd, buffer, strlen(buffer));
	sprintf(buffer, ".%03ld: ", rawtime.tv_usec / 1000);
	write(m_fd, buffer, strlen(buffer));
	vsprintf(buffer, format.c_str(), ap);
	write(m_fd, buffer, strlen(buffer));
	strcpy(buffer, "\n");
	write(m_fd, buffer, strlen(buffer));
}

int main (int argc, char** argv) {
	LoggerStdout logger(DEBUG);

	if (argc < 2) {
		logger.error("Usage: verificaC19-client <qrfile> [mode]");
		logger.error("       where mode can be 2G for Super Green Pass, BOOSTER for booster, WORK for work, ENTRY_ITALY for entry italy, or empty for Standard Green Pass");
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

		DGCVerifier verifier(&keysStorage, &rulesStorage, &crlStorage, strcmp(argv[1], "socket") == 0 ? NULL : &logger);
		if (!verifier.verifyMinSdkVersion()) {
			logger.error("Minimum SDK version does not match");
		} else {
			if (strcmp(argv[1], "socket") == 0) {
				int server_fd = socket(AF_INET, SOCK_STREAM, 0);
				int opt = 1;
				setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
				struct sockaddr_in address;
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = INADDR_ANY;
				address.sin_port = htons(atoi(argv[2]));
				bind(server_fd, (struct sockaddr *)&address, sizeof(address));
				while (true) {
					listen(server_fd, 3);
					int addrlen = sizeof(address);
					int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
					LoggerFd sock_logger(new_socket, DEBUG);
					char buffer[2048 + 1];
					memset(buffer, 0, sizeof(buffer));
					int valread = read(new_socket, buffer, 2048);
					std::string readed = std::string(buffer, valread - 1);
					if (readed == "close") {
						close(new_socket);
						break;
					}
					std::string qr = readed.substr(0, readed.find(";"));
					std::string scanMode = readed.substr(readed.find(";") + 1);
					if (scanMode == "3G") {
						scanMode = SCAN_MODE_3G;
					} else if (scanMode == "2G") {
						scanMode = SCAN_MODE_2G;
					} else if (scanMode == "BOOSTER") {
						scanMode = SCAN_MODE_BOOSTER;
					} else if (scanMode == "WORK") {
						scanMode = SCAN_MODE_WORK;
					} else if (scanMode == "ENTRY_ITALY") {
						scanMode = SCAN_MODE_ENTRY_ITALY;
					}
					CertificateSimple certificate = verifier.verify(qr, scanMode);
					logCertificate(certificate, sock_logger);
					close(new_socket);
				}
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
					if (argc > 2 && strcmp(argv[2], "ENTRY_ITALY") == 0) {
						scanMode = SCAN_MODE_ENTRY_ITALY;
					}
					CertificateSimple certificate = verifier.verify(qr, scanMode);
					logCertificate(certificate, logger);
				}
			}
		}
	}
}
