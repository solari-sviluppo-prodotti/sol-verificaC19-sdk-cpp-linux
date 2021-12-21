// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef LOGGER_STDOUT_H
#define LOGGER_STDOUT_H

#include <verificaC19-sdk/interfaces/Logger.h>
#include <verificaC19-sdk/interfaces/LogLevel.h>

Logger* LoggerStdout_c_create(LogLevel level);
typedef Logger* (*pfLoggerStdout_c_create)(LogLevel);

void LoggerStdout_c_release(const Logger* logger);
typedef void (*pfLoggerStdout_c_release)(const Logger*);

void LoggerStdout_c_error(const Logger* logger, const char* format, ...);
typedef void (*pfLoggerStdout_c_error)(const Logger*, const char*);

void LoggerStdout_c_info(const Logger* logger, const char* format, ...);
typedef void (*pfLoggerStdout_c_info)(const Logger*, const char*);

void LoggerStdout_c_debug(const Logger* logger, const char* format, ...);
typedef void (*pfLoggerStdout_c_debug)(const Logger*, const char*);

#endif // #ifndef LOGGER_STDOUT_H
