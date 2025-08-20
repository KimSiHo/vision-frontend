#include "vision_service.hpp"

#include <chrono>
#include <iostream>
#include <utility>

VisionService::VisionService(zmq::context_t& ctx)
    : sub_(ctx,
           std::string(VisionCommon::AI_RESULTS_ENDPOINT),
           std::string(VisionCommon::TOPIC_DETECTIONS)) {
  // 필요 시 수신 옵션 예시:
  // int one = 1;
  // sub_socket 내부 소켓 핸들을 노출하지 않았다면
  // 옵션은 SubSocket에 헬퍼를 추가해서 설정하세요 (예: SetConflate(true)).
}

VisionService::~VisionService() { Stop(); }

void VisionService::Start() {
  if (running_.exchange(true)) return;
  worker_ = std::thread([this] { Run(); });
}

void VisionService::Stop() {
  if (!running_.exchange(false)) return;
  if (worker_.joinable()) worker_.join();
}

void VisionService::Run() {
  std::cout << "[Front] VisionService: listening on "
            << VisionCommon::AI_RESULTS_ENDPOINT << " topic="
            << VisionCommon::TOPIC_DETECTIONS << std::endl;

  while (running_) {
    auto msg = sub_.receive();  // [topic][payload] 멀티파트 수신
    if (!msg) {
      // non-block 설정이 없다면 여기서는 블로킹됨.
      // 필요하면 SubSocket에 타임아웃/논블록 옵션을 추가하세요.
      continue;
    }
    const auto& [topic, payload] = *msg;
    std::cout << "[Front] topic=" << topic << " payload=" << payload << std::endl;
  }

  std::cout << "[Front] VisionService: stopped" << std::endl;
}
