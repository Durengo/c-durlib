#include "log/log.h"

int main()
{
    log_init_cli();

    CLI_TRACE("Hello, World!");
    CLI_DEBUG("TEST");
    CLI_INFO("TEST");
    CLI_WARN("TEST");
    CLI_ERROR("TEST");
    CLI_FATAL("TEST");
    CLI_CRITICAL("TEST");
    CLI_AUDIT("TEST");

    return 0;
}