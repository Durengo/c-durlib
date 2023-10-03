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
extern const LogTag cli_tag;
extern DLogger* g_cli_logger;

#define CLI_TRACE(msg) log_message_with_prefix(g_cli_logger, CLI, LOG_TRACE, msg);
#define CLI_DEBUG(msg) log_message_with_prefix(g_cli_logger, CLI, LOG_DEBUG, msg)
#define CLI_INFO(msg) log_message_with_prefix(g_cli_logger, CLI, LOG_INFO, msg)
#define CLI_WARN(msg) log_message_with_prefix(g_cli_logger, CLI, LOG_WARN, msg)
#define CLI_ERROR(msg) log_message_with_prefix(g_cli_logger, CLI, LOG_ERROR, msg)
#define CLI_FATAL(msg) log_message_with_prefix(g_cli_logger, CLI, LOG_FATAL, msg)
#define CLI_CRITICAL(msg) log_message_with_prefix(g_cli_logger, CLI, LOG_CRITICAL, msg)
#define CLI_AUDIT(msg) log_message_with_prefix(g_cli_logger, CLI, LOG_AUDIT, msg)


DLogger* log_init();
void log_init_cli();

void log_message(DLogger *l_logger, LogLevel l_level, const char *l_message);
void log_message_with_prefix(DLogger *l_logger, LogTag l_log_tag, LogLevel l_level, const char *l_message);
// void parse_message_with_prefix(LogLevel l_level, const char *l_prefix, const char *l_message);

void log_close(DLogger *l_logger);
static void log_close_cli();
