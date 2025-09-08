#include "config.h"

#include "k2c/logging/log.hpp"


void init() {
    k2c::logging::LogManger::getInstance().initialise();
}


int main() {
    // Initialise logging
    init();

    K2C_IN(K2C_NOARG);

    K2C_INFO("-> Hello, World!");

    K2C_OUT(K2C_NOARG);
}