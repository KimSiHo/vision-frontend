#pragma once

#include <string_view>

namespace app_config {

inline constexpr std::string_view kLogFile = "/var/log/vision/front.log";

inline constexpr const char* kPingServer = "https://www.google.com";
inline constexpr int kPingTimeout = 20000;

}  // namespace app_config
