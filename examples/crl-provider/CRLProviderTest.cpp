// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <curl/curl.h>

#include <json/json.h>

#include <string>

#include <verificaC19-sdk/DGCVerifier.hpp>
#include <crl-provider/CRLProviderTest.hpp>

namespace verificaC19Sdk {

class LoggerNull : public ILogger {

public:

	void error(const std::string& message, ...) const { }
	void info(const std::string& message, ...) const { }
	void debug(const std::string& message, ...) const { }

};

CRLProviderTest::CRLProviderTest(ILogger* logger) : m_logger(logger) {
	if (m_logger == NULL) {
		m_logger = new LoggerNull();
	}
}

void CRLProviderTest::refreshCRL(ICRLStorage* crlStorage) const {
	// insert keys used by test exampl
	m_logger->debug("Store test CRL");
	crlStorage->beginUpdatingCRL();
	crlStorage->storeChunk("{\"id\":\"1\",\"fromVersion\":0,\"version\":1,\"chunk\":1,\"lastChunk\":1,\"revokedUcvi\":[\"TRt4aMz1Bz1N7c0has7zWGCsJECBdUhWs3CH2GEwL28=\",\"3z03vvu5JsngWhXadAUOM7A2DAq/9dFIjLsDANZRiCU=\"],\"creationDate\":\"2021-12-14T18:54:06.150+00:00\",\"firstElementInChunk\":\"TRt4aMz1Bz1N7c0has7zWGCsJECBdUhWs3CH2GEwL28=\",\"lastElementInChunk\":\"3z03vvu5JsngWhXadAUOM7A2DAq/9dFIjLsDANZRiCU=\",\"sizeSingleChunkInByte\":510000,\"totalNumberUCVI\":2}");
	crlStorage->commitUpdatedCRL();
}

} // namespace verificaC19Sdk

