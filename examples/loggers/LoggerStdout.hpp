// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef LOGGER_STDOUT_HPP
#define LOGGER_STDOUT_HPP

#include <verificaC19-sdk/interfaces/ILogger.hpp>

#ifdef __cplusplus

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

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

void* LoggerStdout_c_create(LogLevel level);
typedef void* (*pfLoggerStdout_c_create)(LogLevel);

void LoggerStdout_c_release(const void* logger);
typedef void (*pfLoggerStdout_c_release)(const void*);

void LoggerStdout_c_error(const void* logger, const char* format, ...);
typedef void (*pfLoggerStdout_c_error)(const void*, const char*);

void LoggerStdout_c_info(const void* logger, const char* format, ...);
typedef void (*pfLoggerStdout_c_info)(const void*, const char*);

void LoggerStdout_c_debug(const void* logger, const char* format, ...);
typedef void (*pfLoggerStdout_c_debug)(const void*, const char*);

#ifdef __cplusplus
}
#endif


#endif // #ifndef LOGGER_STDOUT_HPP
