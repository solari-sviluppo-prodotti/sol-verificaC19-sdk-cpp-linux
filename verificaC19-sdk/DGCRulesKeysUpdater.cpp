// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <unistd.h>

#include <verificaC19-sdk/DGCRulesKeysUpdater.hpp>

namespace verificaC19Sdk {

class LoggerNull : public ILogger {

public:

	void error(const std::string& message, ...) const { }
	void info(const std::string& message, ...) const { }
	void debug(const std::string& message, ...) const { }

};

DGCRulesKeysUpdater::DGCRulesKeysUpdater(int intervalSeconds,
		IRulesProvider* rulesProvider, IRulesStorage* rulesStorage,
		IKeysProvider* keysProvider, IKeysStorage* keysStorage, ILogger* logger) :
			m_intervalSeconds(intervalSeconds),
			m_rulesKeysProvider(NULL),
			m_rulesProvider(rulesProvider), m_rulesStorage(rulesStorage),
			m_keysProvider(keysProvider), m_keysStorage(keysStorage),
			m_logger(logger), m_threadLoop(true),
			m_forceUpdateRules(false), m_forceUpdateKeys(false) {
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

DGCRulesKeysUpdater::DGCRulesKeysUpdater(int intervalSeconds,
		IRulesKeysProvider* rulesKeysProvider,
		IRulesStorage* rulesStorage, IKeysStorage* keysStorage, ILogger* logger) :
				m_intervalSeconds(intervalSeconds),
				m_rulesProvider(rulesKeysProvider),
				m_rulesKeysProvider(NULL), m_keysProvider(NULL),
				m_rulesStorage(rulesStorage), m_keysStorage(keysStorage),
				m_logger(logger), m_threadLoop(true),
				m_forceUpdateRules(false), m_forceUpdateKeys(false) {
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

DGCRulesKeysUpdater::~DGCRulesKeysUpdater() {
	m_threadLoop = false;
	pthread_join(m_threadUpdater, NULL);
}

void DGCRulesKeysUpdater::forceUpdateRules() {
	// in our case we does not need for semaphores
	m_forceUpdateRules = true;
}

void DGCRulesKeysUpdater::forceUpdateKeys() {
	// in our case we does not need for semaphores
	m_forceUpdateKeys = true;
}

void DGCRulesKeysUpdater::forceUpdateAll() {
	forceUpdateRules();
	forceUpdateKeys();
}

bool DGCRulesKeysUpdater::isUpdated() const {
	return m_rulesStorage->lastUpdate() < m_intervalSeconds &&
			m_keysStorage->lastUpdate() < m_intervalSeconds;
}

void* DGCRulesKeysUpdater::updaterThread(void* p) {
	static_cast<DGCRulesKeysUpdater*>(p)->updaterThread();
	DGCRulesKeysUpdater* this_ = (DGCRulesKeysUpdater*)p;
	return NULL;
}

void DGCRulesKeysUpdater::updaterThread() {
	m_logger->info("Entering updater thread loop");
	while (m_threadLoop) {
		// optimization for provider that provides in a single call rules and keys
		if (m_forceUpdateKeys && m_forceUpdateRules) {
			m_forceUpdateKeys = false;
			m_forceUpdateRules = false;
			m_logger->info("Forced update of Keys and Rules");
			updateAll();
		} else if (m_forceUpdateRules && !m_forceUpdateKeys) {
			m_forceUpdateRules = false;
			m_logger->info("Forced update of Rules");
			updateRules();
		} else if (m_forceUpdateKeys && !m_forceUpdateRules) {
			m_forceUpdateKeys = false;
			m_logger->info("Forced update of Keys");
			updateKeys();
		}

		// optimization for provider that provides in a single call rules and keys
		if ((m_rulesStorage->lastUpdate() >= m_intervalSeconds && m_keysStorage->lastUpdate() >= m_intervalSeconds - 300) ||
			(m_rulesStorage->lastUpdate() >= m_intervalSeconds - 300 && m_keysStorage->lastUpdate() >= m_intervalSeconds)) {
			m_logger->info("Auto update of Keys and Rules");
			updateAll();
		}
		if (m_rulesStorage->lastUpdate() >= m_intervalSeconds) {
			m_logger->info("Auto update of Rules");
			updateRules();
		}
		if (m_keysStorage->lastUpdate() >= m_intervalSeconds) {
			m_logger->info("Auto update of Keys");
			updateKeys();
		}

		while (m_threadLoop &&
				!m_forceUpdateRules &&
				!m_forceUpdateKeys &&
				m_rulesStorage->lastUpdate() < m_intervalSeconds &&
				m_keysStorage->lastUpdate() < m_intervalSeconds) {
			usleep(100000);
		}
	};
	m_logger->info("Exiting updater thread loop");
}

void DGCRulesKeysUpdater::updateAll() const {
	if (m_rulesKeysProvider != NULL) {
		m_rulesKeysProvider->refreshAll(m_rulesStorage, m_keysStorage);
	} else {
		m_keysProvider->refreshKeys(m_keysStorage);
		m_rulesProvider->refreshRules(m_rulesStorage);
	}
}

void DGCRulesKeysUpdater::updateRules() const {
	if (m_rulesKeysProvider != NULL) {
		m_rulesKeysProvider->refreshAll(m_rulesStorage, NULL);
	} else {
		m_rulesProvider->refreshRules(m_rulesStorage);
	}
}

void DGCRulesKeysUpdater::updateKeys() const {
	if (m_rulesKeysProvider != NULL) {
		m_rulesKeysProvider->refreshAll(NULL, m_keysStorage);
	} else {
		m_keysProvider->refreshKeys(m_keysStorage);
	}
}

DGCRulesKeysUpdater* DGCRulesKeysUpdaterRulesAndKeys_create(int intervalSeconds,
		IRulesProvider* rulesProvider, IRulesStorage* rulesStorage,
		IKeysProvider* keysProvider, IKeysStorage* keysStorage, ILogger* logger) {
	return new DGCRulesKeysUpdater(intervalSeconds,
			rulesProvider, rulesStorage,
			keysProvider, keysStorage, logger);
}

DGCRulesKeysUpdater* DGCRulesKeysUpdaterRulesKeys_create(int intervalSeconds,
		IRulesKeysProvider* rulesKeysProvider, IRulesStorage* rulesStorage,
		IKeysStorage* keysStorage, ILogger* logger) {
	return new DGCRulesKeysUpdater(intervalSeconds,
			rulesKeysProvider, rulesStorage,
			keysStorage, logger);
}

void DGCRulesKeysUpdater_release(DGCRulesKeysUpdater* dgcRulesKeysUpdater) {
	delete dgcRulesKeysUpdater;
}

void DGCRulesKeysUpdater_forceUpdateRules(DGCRulesKeysUpdater* dgcRulesKeysUpdater) {
	dgcRulesKeysUpdater->forceUpdateRules();
}

void DGCRulesKeysUpdater_forceUpdateKeys(DGCRulesKeysUpdater* dgcRulesKeysUpdater) {
	dgcRulesKeysUpdater->forceUpdateKeys();
}

void DGCRulesKeysUpdater_forceUpdateAll(DGCRulesKeysUpdater* dgcRulesKeysUpdater) {
	dgcRulesKeysUpdater->forceUpdateAll();
}

bool DGCRulesKeysUpdater_isUpdated(DGCRulesKeysUpdater* dgcRulesKeysUpdater) {
	return dgcRulesKeysUpdater->isUpdated();
}

} // namespace verificaC19Sdk

RulesKeysUpdater* DGCRulesKeysUpdaterRulesAndKeys_c_create(int intervalSeconds,
		RulesProvider* rulesProvider, RulesStorage* rulesStorage,
		KeysProvider* keysProvider, KeysStorage* keysStorage, Logger* logger) {
	return (RulesKeysUpdater*)new verificaC19Sdk::DGCRulesKeysUpdater(intervalSeconds,
			(verificaC19Sdk::IRulesProvider*)rulesProvider, (verificaC19Sdk::IRulesStorage*)rulesStorage,
			(verificaC19Sdk::IKeysProvider*)keysProvider, (verificaC19Sdk::IKeysStorage*)keysStorage, (verificaC19Sdk::ILogger*)logger);
}

RulesKeysUpdater* DGCRulesKeysUpdaterRulesKeys_c_create(int intervalSeconds,
		RulesKeysProvider* rulesKeysProvider, RulesStorage* rulesStorage,
		KeysStorage* keysStorage, Logger* logger) {
	return (RulesKeysUpdater*)new verificaC19Sdk::DGCRulesKeysUpdater(intervalSeconds,
			(verificaC19Sdk::IRulesKeysProvider*)rulesKeysProvider, (verificaC19Sdk::IRulesStorage*)rulesStorage,
			(verificaC19Sdk::IKeysStorage*)keysStorage, (verificaC19Sdk::ILogger*)logger);
}

void DGCRulesKeysUpdater_c_release(RulesKeysUpdater* dgcRulesKeysUpdater) {
	delete (verificaC19Sdk::DGCRulesKeysUpdater*)dgcRulesKeysUpdater;
}

void DGCRulesKeysUpdater_c_forceUpdateRules(RulesKeysUpdater* dgcRulesKeysUpdater) {
	DGCRulesKeysUpdater_forceUpdateRules((verificaC19Sdk::DGCRulesKeysUpdater*)dgcRulesKeysUpdater);
}

void DGCRulesKeysUpdater_c_forceUpdateKeys(RulesKeysUpdater* dgcRulesKeysUpdater) {
	DGCRulesKeysUpdater_forceUpdateKeys((verificaC19Sdk::DGCRulesKeysUpdater*)dgcRulesKeysUpdater);
}

void DGCRulesKeysUpdater_c_forceUpdateAll(RulesKeysUpdater* dgcRulesKeysUpdater) {
	DGCRulesKeysUpdater_forceUpdateAll((verificaC19Sdk::DGCRulesKeysUpdater*)dgcRulesKeysUpdater);
}

bool DGCRulesKeysUpdater_c_isUpdated(RulesKeysUpdater* dgcRulesKeysUpdater) {
	return DGCRulesKeysUpdater_isUpdated((verificaC19Sdk::DGCRulesKeysUpdater*)dgcRulesKeysUpdater);
}
