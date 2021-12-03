// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef ILOGGER_HPP
#define ILOGGER_HPP

enum LogLevel {
	NONE,
	ERROR,
	INFO,
	DEBUG
};

typedef enum LogLevel LogLevel;

#ifdef __cplusplus

#include <string>

namespace verificaC19Sdk {

typedef enum ::LogLevel LogLevel;

/**
 * Interface to implement Logger service
 */
class ILogger {

public:

	/**
	 * Log messages to error level
	 *
	 * @param format Formatter in printf style
	 *
	 * @param ... Params in va_arg style
	 */
	virtual void error(const std::string& format, ...) const = 0;

	/**
	 * Log messages to info level
	 *
	 * @param format Formatter in printf style
	 *
	 * @param ... Params in va_arg style
	 */
	virtual void info(const std::string& format, ...) const = 0;

	/**
	 * Log messages to debug level
	 *
	 * @param format Formatter in printf style
	 *
	 * @param ... Params in va_arg style
	 */
	virtual void debug(const std::string& format, ...) const = 0;

};

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#endif // #ifndef ILOGGER_HPP
