// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef LOGGER_STDOUT_H
#define LOGGER_STDOUT_H

#include <verificaC19-sdk/interfaces/LogLevel.h>

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


#endif // #ifndef LOGGER_STDOUT_H
