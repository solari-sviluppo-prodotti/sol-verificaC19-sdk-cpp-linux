// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CRL_STORAGE_MEMORY_HPP
#define CRL_STORAGE_MEMORY_HPP

#include <stdbool.h>
#include <time.h>

#include <verificaC19-sdk/interfaces/ICRLStorage.hpp>

#include <json/json.h>

#include <string>

namespace verificaC19Sdk {

/**
 * Implements storage to store CRL to json in memory
 */
class CRLStorageMemory : public ICRLStorage {

public:

	CRLStorageMemory();
	bool hasCertificateHash(const std::string& certificateHash) const;
	void beginUpdatingCRL();
	void storeChunk(const std::string& chunk);
	void clearCRL();
	void commitUpdatedCRL();
	void rollbackUpdatedCRL();
	bool isUpdating() const { return m_updatingMode; }
	int version() const { return m_CRL["version"].asInt(); }
	std::string downloadInfo() const;
	time_t lastUpdate() const { return time(NULL) - m_CRL["_lastUpdate"].asInt(); }

private:

	bool m_updatingMode;
	Json::Value m_CRL;
	Json::Value m_updatingCRL;

};

} // namespace verificaC19Sdk

#endif // #ifndef CRL_STORAGE_MEMORY_HPP
