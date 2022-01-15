// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CRL_PROVIDER_TEST_HPP
#define CRL_PROVIDER_TEST_HPP

#include <verificaC19-sdk/interfaces/ICRLProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

#include <string>
#include <map>

namespace verificaC19Sdk {

/**
 * Implements provider to store test CRL
 */
class CRLProviderTest : public ICRLProvider {

public:

	/**
	 * @param logger Optional implementation of Logger, if NULL no log will be done
	 */
	CRLProviderTest(ILogger* logger);
	void refreshCRL(ICRLStorage* crlStorage) const;

private:

	ILogger* m_logger;

};

} // namespace verificaC19Sdk

#endif // #ifndef CRL_PROVIDER_TEST_HPP
