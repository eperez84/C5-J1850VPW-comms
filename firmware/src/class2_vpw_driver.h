#pragma once

#include <Arduino.h>

namespace class2 {

constexpr uint8_t kMaxDataBytes = 8;
constexpr uint8_t kRxQueueDepth = 8;
constexpr uint8_t kTxQueueDepth = 8;

struct Frame {
  uint8_t id = 0;
  uint8_t len = 0;
  uint8_t data[kMaxDataBytes] = {0};
};

class VpwDriver {
 public:
  bool begin();
  void poll();

  bool available() const;
  bool read(Frame& out);
  bool send(const Frame& frame);

  uint32_t txCount() const { return tx_count_; }
  uint32_t rxCount() const { return rx_count_; }

  void setLoopbackForBench(bool enabled) { loopback_enabled_ = enabled; }

 private:
  bool pushRx(const Frame& frame);
  bool popRx(Frame& out);

  bool pushTx(const Frame& frame);
  bool popTx(Frame& out);

  bool hardwareReceive(Frame& out);
  bool hardwareTransmit(const Frame& frame);

  Frame rx_queue_[kRxQueueDepth];
  uint8_t rx_head_ = 0;
  uint8_t rx_tail_ = 0;
  uint8_t rx_size_ = 0;

  Frame tx_queue_[kTxQueueDepth];
  uint8_t tx_head_ = 0;
  uint8_t tx_tail_ = 0;
  uint8_t tx_size_ = 0;

  bool loopback_enabled_ = false;
  uint32_t tx_count_ = 0;
  uint32_t rx_count_ = 0;
};

}  // namespace class2
