#include "class2_vpw_driver.h"

namespace class2 {

bool VpwDriver::begin() {
  rx_head_ = rx_tail_ = rx_size_ = 0;
  tx_head_ = tx_tail_ = tx_size_ = 0;
  tx_count_ = 0;
  rx_count_ = 0;
  return true;
}

void VpwDriver::poll() {
  Frame incoming;
  if (hardwareReceive(incoming)) {
    (void)pushRx(incoming);
    ++rx_count_;
  }

  Frame outgoing;
  if (popTx(outgoing)) {
    (void)hardwareTransmit(outgoing);
  }
}

bool VpwDriver::available() const { return rx_size_ > 0; }

bool VpwDriver::read(Frame& out) { return popRx(out); }

bool VpwDriver::send(const Frame& frame) {
  if (!pushTx(frame)) {
    return false;
  }

  ++tx_count_;
  if (loopback_enabled_) {
    (void)pushRx(frame);
  }
  return true;
}

bool VpwDriver::pushRx(const Frame& frame) {
  if (rx_size_ >= kRxQueueDepth) {
    return false;
  }
  rx_queue_[rx_head_] = frame;
  rx_head_ = static_cast<uint8_t>((rx_head_ + 1) % kRxQueueDepth);
  ++rx_size_;
  return true;
}

bool VpwDriver::popRx(Frame& out) {
  if (rx_size_ == 0) {
    return false;
  }
  out = rx_queue_[rx_tail_];
  rx_tail_ = static_cast<uint8_t>((rx_tail_ + 1) % kRxQueueDepth);
  --rx_size_;
  return true;
}

bool VpwDriver::pushTx(const Frame& frame) {
  if (tx_size_ >= kTxQueueDepth) {
    return false;
  }
  tx_queue_[tx_head_] = frame;
  tx_head_ = static_cast<uint8_t>((tx_head_ + 1) % kTxQueueDepth);
  ++tx_size_;
  return true;
}

bool VpwDriver::popTx(Frame& out) {
  if (tx_size_ == 0) {
    return false;
  }
  out = tx_queue_[tx_tail_];
  tx_tail_ = static_cast<uint8_t>((tx_tail_ + 1) % kTxQueueDepth);
  --tx_size_;
  return true;
}

bool VpwDriver::hardwareReceive(Frame& out) {
  (void)out;
  // TODO: Replace with your VPW front-end read routine.
  // This stub keeps firmware deterministic while hardware mapping is pending.
  return false;
}

bool VpwDriver::hardwareTransmit(const Frame& frame) {
  (void)frame;
  // TODO: Replace with your VPW front-end transmit routine.
  return true;
}

}  // namespace class2
