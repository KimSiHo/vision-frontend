#pragma once

#include <string>

#include "common/utils/json.hpp"

class IService {
public:
  virtual bool handle(const std::string& topic, const app_common::Json& payload) = 0;
  virtual ~IService() = default;
};
