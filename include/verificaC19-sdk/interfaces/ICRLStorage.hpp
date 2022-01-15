// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef ICRL_STORAGE_HPP
#define ICRL_STORAGE_HPP

#include <string>

namespace verificaC19Sdk {

/**
 * Interface to implement Certificates Revocation List (list of sha256 of UVCI) Storage service
 * to store and retrieve CRL from storage archive
 */
class ICRLStorage {

public:

	/**
	 * Check if has certificate hash in storage archive
	 *
	 * @param certificateHash Certificate sha256 to search in list
	 *
	 * @return true if certificate hash found in list, else false.
	 */
	virtual bool hasCertificateHash(const std::string& certificateHash) const = 0;

	/**
	 * Start transaction with CRL storage archive
	 */
	virtual void beginUpdatingCRL() = 0;

	/**
	 * Store CRL chunk in storage archive, if started transaction it must be stored in transaction
	 * mode, else it must be stored immediately to archive
	 *
	 * @param chunk CRL chunk
	 */
	virtual void storeChunk(const std::string& chunk) = 0;

	/**
	 * Clear CRL in storage archive, if started transaction it must be stored in transaction
	 * mode, else it must be stored immediately to archive
	 */
	virtual void clearCRL() = 0;

	/**
	 * End transaction with CRL storage service
	 */
	virtual void commitUpdatedCRL() = 0;

	/**
	 * End transaction with CRL storage service
	 */
	virtual void rollbackUpdatedCRL() = 0;

	/**
	 * Request to CRL storage service if update transaction is running
	 *
	 * @return true if transaction running, else false.
	 */
	virtual bool isUpdating() const = 0;

	/**
	 * Get current version stored in CRL storage archive
	 *
	 * @return Current version.
	 */
	virtual int version() const = 0;

	/**
	 * Get current CRL download informations stored in CRL storage archive
	 *
	 * @return Current download informations (should contain chunk number, chunks id, chunks destination version).
	 */
	virtual std::string downloadInfo() const = 0;

	/**
	 * Get time in seconds elapsed until last update
	 *
	 * @return Seconds elapsed until last update
	 */
	virtual time_t lastUpdate() const = 0;
};

} // namespace verificaC19Sdk

#endif // #ifndef ICRL_STORAGE_HPP
