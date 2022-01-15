// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <stdarg.h>
#include <loggers/LoggerStdout.hpp>

extern "C" {

#include <loggers/LoggerStdout.h>

} // extern "C"

Logger* LoggerStdout_c_create(LogLevel level) {
	return (Logger*)new verificaC19Sdk::LoggerStdout(level);
}

void LoggerStdout_c_release(const Logger* logger) {
	verificaC19Sdk::LoggerStdout* this_ = (verificaC19Sdk::LoggerStdout*)logger;
	delete this_;
}

void LoggerStdout_c_error(const Logger* logger, const char* format, ...) {
	verificaC19Sdk::LoggerStdout* this_ = (verificaC19Sdk::LoggerStdout*)logger;
	va_list args1;
	va_start(args1, format);
	this_->log(ERROR, std::string(format), args1);
	va_end(args1);
}

void LoggerStdout_c_info(const Logger* logger, const char* format, ...) {
	verificaC19Sdk::LoggerStdout* this_ = (verificaC19Sdk::LoggerStdout*)logger;
	va_list args1;
	va_start(args1, format);
	this_->log(INFO, std::string(format), args1);
	va_end(args1);
}

void LoggerStdout_c_debug(const Logger* logger, const char* format, ...) {
	verificaC19Sdk::LoggerStdout* this_ = (verificaC19Sdk::LoggerStdout*)logger;
	va_list args1;
	va_start(args1, format);
	this_->log(DEBUG, std::string(format), args1);
	va_end(args1);
}
