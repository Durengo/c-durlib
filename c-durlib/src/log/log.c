#include "log.h"
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// Declared in log.h
const LogTag cli_tag = CLI;

DLogger *g_cli_logger = NULL;

#ifdef C_DURLIB_PLATFORM_WINDOWS
_Bool g_is_console_initialized = false;
HANDLE m_hConsole = NULL;
WORD m_originalConsoleAttributes = 0;
#endif

#ifdef C_DURLIB_PLATFORM_WINDOWS
void InitializeConsole()
{
    if (!g_is_console_initialized)
    {
        m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        GetConsoleScreenBufferInfo(m_hConsole, &consoleInfo);
        m_originalConsoleAttributes = consoleInfo.wAttributes;
        g_is_console_initialized = true;
    }
}

void ResetConsoleColor()
{
    SetConsoleTextAttribute(m_hConsole, m_originalConsoleAttributes);
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
    SetConsoleTextAttribute(m_hConsole, attributes);
}
#endif

DLogger *log_init()
{
    DLogger *logger = (DLogger *)malloc(sizeof(DLogger));
    logger->m_exists = true;

    InitializeConsole();

    return logger;
}

void log_init_cli()
{
    g_cli_logger = (DLogger *)malloc(sizeof(DLogger));
    g_cli_logger->m_exists = true;

    InitializeConsole();
}

void log_message(DLogger *l_logger, LogLevel l_level, const char *l_message)
{
    if (!l_logger->m_exists)
        return;

    char *msg = (char *)malloc(sizeof(char) * (strlen(LogLevelStrings[l_level]) + 1));
    strcpy(msg, LogLevelStrings[l_level]);
    strcat(msg, " ");
    strcat(msg, l_message);
    strcat(msg, "\n");
#ifdef C_DURLIB_PLATFORM_WINDOWS
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), msg, strlen(msg), NULL, NULL);
    return;
#else
    printf("%s", msg);
    return;
#endif
}

void log_message_with_prefix(DLogger *l_logger, LogTag l_log_tag, LogLevel l_level, const char *l_message)
{
    if (!l_logger->m_exists)
        return;

    // LogTag + LogLevel + l_message + 4 brackets + 3 spaces + 1 newline
    size_t required_size = strlen(LogTagStrings[l_log_tag]) + strlen(LogLevelStrings[l_level]) + strlen(l_message) + 4 + 3 + 1;
    
    // + 1 null terminator
    char *msg = (char *)malloc(required_size + 1);

    if (msg)
    {
        snprintf(msg, required_size, "[%s] [%s] %s\n", LogTagStrings[l_log_tag], LogLevelStrings[l_level], l_message);

#ifdef C_DURLIB_PLATFORM_WINDOWS
        SetConsoleColor(l_level);
        WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), msg, strlen(msg), NULL, NULL);
        ResetConsoleColor();
#else
        printf("%s", msg);
#endif

        free(msg);
    }
}

void log_close_cli()
{
    if (g_cli_logger == NULL)
        return;

    log_close(g_cli_logger);
    g_cli_logger = NULL;
}

void log_close(DLogger *l_logger)
{
    if (l_logger->m_exists)
        free(l_logger);
}