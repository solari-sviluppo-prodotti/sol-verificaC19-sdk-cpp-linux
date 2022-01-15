// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CRL_PROVIDER_ITALY_HPP
#define CRL_PROVIDER_ITALY_HPP

#include <verificaC19-sdk/interfaces/ICRLProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

#include <string>
#include <map>

namespace verificaC19Sdk {

/**
 * Implements provider to download CRL from italian server
 */
class CRLProviderItaly : public ICRLProvider {

public:

	/**
	 * @param logger Optional implementation of Logger, if NULL no log will be done
	 */
	CRLProviderItaly(ILogger* logger);
	void refreshCRL(ICRLStorage* crlStorage) const;

private:

	static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);
	long curlRequest(std::string url, std::string& response) const;

	ILogger* m_logger;

};

} // namespace verificaC19Sdk

#endif // #ifndef CRL_PROVIDER_ITALY_HPP
