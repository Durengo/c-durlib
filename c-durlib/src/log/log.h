#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef enum
{
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
    LOG_CRITICAL,
    LOG_AUDIT
} LogLevel;

static const char *LogLevelStrings[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
    "CRITICAL",
    "AUDIT"};

typedef enum
{
    CLI,
    CLI_DEBUG,
    CLI_FILE
} LogTag;

static const char *LogTagStrings[] = {
    "CLI",
    "DEBUG",
    "FILE"};

typedef struct
{
    _Bool m_exists;
} DLogger;

// Defined in log.c
extern const LogTag const_CliTag;
extern DLogger *CliLoggerInstance;

#define CLI_TRACE(msg) LogMessageWithPrefix(CliLoggerInstance, CLI, LOG_TRACE, msg);
#define CLI_DEBUG(msg) LogMessageWithPrefix(CliLoggerInstance, CLI, LOG_DEBUG, msg)
#define CLI_INFO(msg) LogMessageWithPrefix(CliLoggerInstance, CLI, LOG_INFO, msg)
#define CLI_WARN(msg) LogMessageWithPrefix(CliLoggerInstance, CLI, LOG_WARN, msg)
#define CLI_ERROR(msg) LogMessageWithPrefix(CliLoggerInstance, CLI, LOG_ERROR, msg)
#define CLI_FATAL(msg) LogMessageWithPrefix(CliLoggerInstance, CLI, LOG_FATAL, msg)
#define CLI_CRITICAL(msg) LogMessageWithPrefix(CliLoggerInstance, CLI, LOG_CRITICAL, msg)
#define CLI_AUDIT(msg) LogMessageWithPrefix(CliLoggerInstance, CLI, LOG_AUDIT, msg)

DLogger *LogInit();
void LogInitCli();

void LogMessage(DLogger *l_logger, LogLevel l_level, const char *l_message);
void LogMessageWithPrefix(DLogger *l_logger, LogTag l_log_tag, LogLevel l_level, const char *l_message);
// void parse_message_with_prefix(LogLevel l_level, const char *l_prefix, const char *l_message);

void log_close(DLogger *l_logger);
static void log_close_cli();
