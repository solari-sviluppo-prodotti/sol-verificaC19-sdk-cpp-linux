// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <unistd.h>

#include <verificaC19-sdk/DGCUpdater.hpp>

namespace verificaC19Sdk {

class LoggerNull : public ILogger {

public:

	void error(const std::string& message, ...) const { }
	void info(const std::string& message, ...) const { }
	void debug(const std::string& message, ...) const { }

};

DGCUpdater::DGCUpdater(int intervalSeconds,
		IRulesProvider* rulesProvider, IRulesStorage* rulesStorage,
		IKeysProvider* keysProvider, IKeysStorage* keysStorage,
		ICRLProvider* crlProvider, ICRLStorage* crlStorage,
		ILogger* logger) :
			m_intervalSeconds(intervalSeconds),
			m_allProvider(NULL),
			m_rulesProvider(rulesProvider), m_rulesStorage(rulesStorage),
			m_keysProvider(keysProvider), m_keysStorage(keysStorage),
			m_crlProvider(crlProvider), m_crlStorage(crlStorage),
			m_logger(logger), m_threadLoop(true),
			m_forceUpdateRules(false), m_forceUpdateKeys(false),
			m_forceUpdateCRL(false) {
	if (m_logger == NULL) {
		m_logger = new LoggerNull();
	}

	// set minimum update interval to 4 hours
	if (m_intervalSeconds < 14400) {
		m_intervalSeconds = 14400;
	}
	// set maximum update interval to 24 hours
	if (m_intervalSeconds > 86400) {
		m_intervalSeconds = 86400;
	}

	pthread_attr_t attrUpdater;
	pthread_attr_init(&attrUpdater);
	pthread_create(&m_threadUpdater, &attrUpdater, updaterThread, this);
}

DGCUpdater::DGCUpdater(int intervalSeconds,
		IAllProvider* allProvider,
		IRulesStorage* rulesStorage, IKeysStorage* keysStorage,
		ICRLStorage* crlStorage, ILogger* logger) :
				m_intervalSeconds(intervalSeconds),
				m_allProvider(allProvider),
				m_rulesProvider(NULL), m_keysProvider(NULL),
				m_rulesStorage(rulesStorage), m_keysStorage(keysStorage),
				m_crlStorage(crlStorage), m_logger(logger), m_threadLoop(true),
				m_forceUpdateRules(false), m_forceUpdateKeys(false),
				m_forceUpdateCRL(false) {
	if (m_logger == NULL) {
		m_logger = new LoggerNull();
	}

	// set minimum update interval to 4 hours
	if (m_intervalSeconds < 14400) {
		m_intervalSeconds = 14400;
	}
	// set maximum update interval to 24 hours
	if (m_intervalSeconds > 86400) {
		m_intervalSeconds = 86400;
	}

	pthread_attr_t attrUpdater;
	pthread_attr_init(&attrUpdater);
	pthread_create(&m_threadUpdater, &attrUpdater, updaterThread, this);
}

DGCUpdater::~DGCUpdater() {
	m_threadLoop = false;
	pthread_join(m_threadUpdater, NULL);
}

void DGCUpdater::forceUpdateRules() {
	// in our case we does not need for semaphores
	m_forceUpdateRules = true;
}

void DGCUpdater::forceUpdateKeys() {
	// in our case we does not need for semaphores
	m_forceUpdateKeys = true;
}

void DGCUpdater::forceUpdateCRL() {
	// in our case we does not need for semaphores
	m_forceUpdateCRL = true;
}

void DGCUpdater::forceUpdateAll() {
	forceUpdateRules();
	forceUpdateKeys();
	forceUpdateCRL();
}

bool DGCUpdater::isUpdated() const {
	return !m_forceUpdateRules &&
			!m_forceUpdateKeys &&
			(m_crlStorage == NULL || !m_forceUpdateCRL) &&
			m_rulesStorage->lastUpdate() < m_intervalSeconds &&
			m_keysStorage->lastUpdate() < m_intervalSeconds &&
			(m_crlStorage == NULL || m_crlStorage->lastUpdate() < m_intervalSeconds);
}

void* DGCUpdater::updaterThread(void* p) {
	static_cast<DGCUpdater*>(p)->updaterThread();
	DGCUpdater* this_ = (DGCUpdater*)p;
	return NULL;
}

void DGCUpdater::updaterThread() {
	m_logger->info("Entering updater thread loop");
	while (m_threadLoop) {
		// optimization for provider that provides in a single call rules, keys and CRL
		if (m_forceUpdateRules && m_forceUpdateKeys && m_forceUpdateCRL) {
			m_logger->info("Forced update of Rules, Keys and CRL");
			updateAll();
			m_forceUpdateRules = false;
			m_forceUpdateKeys = false;
			m_forceUpdateCRL = false;
		}

		// optimization for provider that provides in a single call rules and keys
		if ((m_rulesStorage->lastUpdate() >= m_intervalSeconds &&
				m_keysStorage->lastUpdate() >= m_intervalSeconds - 300 &&
				(m_crlStorage == NULL || m_crlStorage->lastUpdate() >= m_intervalSeconds - 300)) ||
			(m_keysStorage->lastUpdate() >= m_intervalSeconds &&
				m_rulesStorage->lastUpdate() >= m_intervalSeconds - 300 &&
				(m_crlStorage == NULL || m_crlStorage->lastUpdate() >= m_intervalSeconds - 300)) ||
			(m_crlStorage != NULL && m_crlStorage->lastUpdate() >= m_intervalSeconds &&
				m_rulesStorage->lastUpdate() >= m_intervalSeconds - 300 &&
				m_keysStorage->lastUpdate() >= m_intervalSeconds - 300)) {
			m_logger->info("Auto update of Keys and Rules");
			updateAll();
			m_forceUpdateRules = false;
			m_forceUpdateKeys = false;
			m_forceUpdateCRL = false;
		}
		if (m_forceUpdateRules || m_rulesStorage->lastUpdate() >= m_intervalSeconds) {
			m_logger->info("%s update of Rules", m_forceUpdateRules ? "Forced" : "Auto");
			updateRules();
			m_forceUpdateRules = false;
		}
		if (m_forceUpdateKeys || m_keysStorage->lastUpdate() >= m_intervalSeconds) {
			m_logger->info("%s update of Keys", m_forceUpdateKeys ? "Forced" : "Auto");
			updateKeys();
			m_forceUpdateKeys = false;
		}
		if (m_crlStorage != NULL && (m_forceUpdateCRL || m_crlStorage->lastUpdate() >= m_intervalSeconds)) {
			m_logger->info("%s update of CRL", m_forceUpdateCRL ? "Forced" : "Auto");
			updateCRL();
			m_forceUpdateCRL = false;
		}

		while (m_threadLoop &&
				!m_forceUpdateRules &&
				!m_forceUpdateKeys &&
				(m_crlStorage == NULL || !m_forceUpdateCRL) &&
				m_rulesStorage->lastUpdate() < m_intervalSeconds &&
				m_keysStorage->lastUpdate() < m_intervalSeconds &&
				(m_crlStorage == NULL || m_crlStorage->lastUpdate() < m_intervalSeconds)) {
			usleep(100000);
		}
	};
	m_logger->info("Exiting updater thread loop");
}

void DGCUpdater::updateAll() const {
	if (m_allProvider != NULL) {
		m_allProvider->refreshAll(m_rulesStorage, m_keysStorage, m_crlStorage);
	} else {
		m_keysProvider->refreshKeys(m_keysStorage);
		m_rulesProvider->refreshRules(m_rulesStorage);
		if (m_crlProvider != NULL) {
			m_crlProvider->refreshCRL(m_crlStorage);
		}
	}
}

void DGCUpdater::updateRules() const {
	if (m_allProvider != NULL) {
		m_allProvider->refreshAll(m_rulesStorage, NULL, NULL);
	} else {
		m_rulesProvider->refreshRules(m_rulesStorage);
	}
}

void DGCUpdater::updateKeys() const {
	if (m_allProvider != NULL) {
		m_allProvider->refreshAll(NULL, m_keysStorage, NULL);
	} else {
		m_keysProvider->refreshKeys(m_keysStorage);
	}
}

void DGCUpdater::updateCRL() const {
	if (m_allProvider != NULL) {
		m_allProvider->refreshAll(NULL, NULL, m_crlStorage);
	} else {
		m_crlProvider->refreshCRL(m_crlStorage);
	}
}

DGCUpdater* DGCUpdater_create(int intervalSeconds,
		IRulesProvider* rulesProvider, IRulesStorage* rulesStorage,
		IKeysProvider* keysProvider, IKeysStorage* keysStorage,
		ICRLProvider* crlProvider, ICRLStorage* crlStorage,
		ILogger* logger) {
	return new DGCUpdater(intervalSeconds,
			rulesProvider, rulesStorage,
			keysProvider, keysStorage,
			crlProvider, crlStorage,
			logger);
}

DGCUpdater* DGCUpdaterAll_create(int intervalSeconds,
		IAllProvider* allProvider, IRulesStorage* rulesStorage,
		IKeysStorage* keysStorage, ICRLStorage* crlStorage, ILogger* logger) {
	return new DGCUpdater(intervalSeconds,
			allProvider, rulesStorage,
			keysStorage, crlStorage, logger);
}

void DGCUpdater_release(DGCUpdater* dgcUpdater) {
	delete dgcUpdater;
}

void DGCUpdater_forceUpdateRules(DGCUpdater* dgcUpdater) {
	dgcUpdater->forceUpdateRules();
}

void DGCUpdater_forceUpdateKeys(DGCUpdater* dgcUpdater) {
	dgcUpdater->forceUpdateKeys();
}

void DGCUpdater_forceUpdateCRL(DGCUpdater* dgcUpdater) {
	dgcUpdater->forceUpdateCRL();
}

void DGCUpdater_forceUpdateAll(DGCUpdater* dgcUpdater) {
	dgcUpdater->forceUpdateAll();
}

bool DGCUpdater_isUpdated(DGCUpdater* dgcUpdater) {
	return dgcUpdater->isUpdated();
}

} // namespace verificaC19Sdk

