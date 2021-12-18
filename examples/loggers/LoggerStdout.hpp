// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef LOGGER_STDOUT_HPP
#define LOGGER_STDOUT_HPP

#include <verificaC19-sdk/interfaces/ILogger.hpp>

namespace verificaC19Sdk {

class LoggerStdout : public ILogger {

public:

	LoggerStdout(LogLevel level) { m_level = level; }

	void error(const std::string& format, ...) const;
	void info(const std::string& format, ...) const;
	void debug(const std::string& format, ...) const;
	void log(LogLevel level, const std::string& format, va_list ap) const;

private:

	LogLevel m_level;

};

} // namespace verificaC19Sdk

extern "C" {

#include <examples/loggers/LoggerStdout.h>

} // extern "C"

#endif // #ifndef LOGGER_STDOUT_HPP
