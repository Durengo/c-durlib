#include "log.h"

#ifdef C_DURLIB_PLATFORM_WINDOWS
#include <windows.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// Declared in log.h
const LogTag const_CliTag = CLI;

DLogger *CliLoggerInstance = NULL;

#ifdef C_DURLIB_PLATFORM_WINDOWS
_Bool b_IsCliLoggerInitialized = false;
HANDLE p_ConsoleInstance = NULL;
WORD word_OriginalConsoleAttributes = 0;
#endif

#ifdef C_DURLIB_PLATFORM_WINDOWS
void InitializeConsole()
{
    if (!b_IsCliLoggerInitialized)
    {
        p_ConsoleInstance = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        GetConsoleScreenBufferInfo(p_ConsoleInstance, &consoleInfo);
        word_OriginalConsoleAttributes = consoleInfo.wAttributes;
        b_IsCliLoggerInitialized = true;
    }
}

void ResetConsoleColor()
{
    SetConsoleTextAttribute(p_ConsoleInstance, word_OriginalConsoleAttributes);
}

void SetConsoleColor(LogLevel l_level)
{
    WORD attributes = 0;

    switch (l_level)
    {
    case LOG_TRACE:
        attributes = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case LOG_DEBUG:
        attributes = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
        break;
    case LOG_INFO:
        attributes = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case LOG_WARN:
        attributes = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    case LOG_ERROR:
        attributes = FOREGROUND_INTENSITY | FOREGROUND_RED;
        break;
    case LOG_FATAL:
        attributes = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;
        break;
    case LOG_CRITICAL:
        attributes = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case LOG_AUDIT:
        attributes = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    }
    SetConsoleTextAttribute(p_ConsoleInstance, attributes);
}
#endif

DLogger *LogInit()
{
    DLogger *logger = (DLogger *)malloc(sizeof(DLogger));
    logger->m_exists = true;
#ifdef C_DURLIB_PLATFORM_WINDOWS
    InitializeConsole();
#endif
    return logger;
}

void LogInitCli()
{
    CliLoggerInstance = (DLogger *)malloc(sizeof(DLogger));
    CliLoggerInstance->m_exists = true;
#ifdef C_DURLIB_PLATFORM_WINDOWS
    InitializeConsole();
#endif
}

void LogMessage(DLogger *l_logger, LogLevel l_level, const char *l_message)
{
    if (!l_logger->m_exists)
        return;

    // LogLevel + l_message + 2 brackets + 3 spaces + 1 newline
    size_t required_size = strlen(LogLevelStrings[l_level]) + strlen(l_message) + 2 + 1 + 1;

    // + 1 null terminator
    char *msg = (char *)malloc(required_size + 1);

    if (msg)
    {
        snprintf(msg, required_size, "[%s] %s\n", LogLevelStrings[l_level], l_message);

#ifdef C_DURLIB_PLATFORM_WINDOWS
        SetConsoleColor(l_level);
        WriteConsole(p_ConsoleInstance, msg, strlen(msg), NULL, NULL);
        ResetConsoleColor();
#else
        printf("%s", msg);
#endif

        free(msg);
    }
}

void LogMessageWithPrefix(DLogger *l_logger, LogTag l_log_tag, LogLevel l_level, const char *l_message)
{
    if (!l_logger->m_exists)
        return;

    // LogTag + LogLevel + l_message + 4 brackets + 2 whitespaces + 1 newline + 1 null terminator
    size_t required_size = strlen(LogTagStrings[l_log_tag]) + strlen(LogLevelStrings[l_level]) + strlen(l_message) + 4 + 2 + 1 + 1;

    char *msg = (char *)malloc(required_size);

    if (msg)
    {
        snprintf(msg, required_size, "[%s] [%s] %s\n", LogTagStrings[l_log_tag], LogLevelStrings[l_level], l_message);

#ifdef C_DURLIB_PLATFORM_WINDOWS
        SetConsoleColor(l_level);
        WriteConsole(p_ConsoleInstance, msg, strlen(msg), NULL, NULL);
        ResetConsoleColor();
#else
        printf("%s", msg);
#endif

        free(msg);
    }
}

void log_close_cli()
{
    if (CliLoggerInstance == NULL)
        return;

    log_close(CliLoggerInstance);
    CliLoggerInstance = NULL;
}

void log_close(DLogger *l_logger)
{
    if (l_logger->m_exists)
        free(l_logger);
}