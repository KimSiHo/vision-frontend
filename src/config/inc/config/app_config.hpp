#pragma once

#include <string_view>

namespace AppConfig {
    inline constexpr std::string_view LOG_FILE = "/var/log/vision/front.log";

    inline constexpr const char* PING_SERVER = "https://www.google.com";
    inline constexpr const int PING_TIMEOUT = 20000;
}
