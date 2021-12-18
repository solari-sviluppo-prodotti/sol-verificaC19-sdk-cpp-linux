// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef IKEYS_STORAGE_HPP
#define IKEYS_STORAGE_HPP

#include <string>

namespace verificaC19Sdk {

/**
 * Interface to implement Key Storage service to store and retreive keys from storage archive
 */
class IKeysStorage {

public:

	/**
	 * Retreive requested key from storage archive
	 *
	 * @param kid Base64 encoded Key identifier
	 *
	 * @return Base64 encoded X509 certificate
	 */
	virtual std::string getKey(const std::string& kid) const = 0;

	/**
	 * Start transaction with Keys storage archive
	 */
	virtual void beginUpdatingKeys() = 0;

	/**
	 * Store key in storage archive, if started transaction it must be stored in transaction
	 * mode, else it must be stored immediately to archive
	 *
	 * @param kid Base64 encoded Key identifier
	 *
	 * @param ecx Base64 encoded X509 Certificate
	 */
	virtual void storeKey(const std::string& kid, const std::string& ecx) = 0;

	/**
	 * End transaction with Keys storage service
	 */
	virtual void commitUpdatedKeys() = 0;

	/**
	 * Rollback transaction with storage service
	 */
	virtual void rollbackUpdatedKeys() = 0;

	/**
	 * Request to Keys storage service if update transaction is running
	 *
	 * @return true if transaction running, else false.
	 */
	virtual bool isUpdating() = 0;

	/**
	 * Store to Keys storage service the token matched to last stored key
	 *
	 * @param token Token to store
	 */
	virtual void setLastStoredKeyToken(std::string& token) = 0;

	/**
	 * Get token matched to last stored key
	 *
	 * @return Token matched to last stored key
	 */
	virtual std::string getLastStoredKeyToken() const = 0;

	/**
	 * Get time in seconds elapsed until last update
	 *
	 * @return Seconds elapsed until last update
	 */
	virtual time_t lastUpdate() = 0;

};

} // namespace verificaC19Sdk

#endif // #ifndef IKEYS_STORAGE_HPP
