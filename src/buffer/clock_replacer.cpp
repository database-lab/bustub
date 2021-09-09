//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// clock_replacer.cpp
//
// Identification: src/buffer/clock_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/clock_replacer.h"
namespace bustub {

ClockReplacer::ClockReplacer(size_t num_pages) :clock_hand_(0), victims_(0) {
  for (size_t i = 0; i < num_pages; i++) {
    frames_.emplace_back(Frame(i));
  }
}

ClockReplacer::~ClockReplacer() = default;

bool ClockReplacer::Victim(frame_id_t *frame_id) {
  std::lock_guard<std::mutex>lock(mutex_);
  if (victims_ == 0) {
    return false;
  }
    while(true) {
        if (frames_[clock_hand_].in_recently_clock_) {
          if (!frames_[clock_hand_].ref_bit_) {
            *frame_id = frames_[clock_hand_].id_;
            frames_[clock_hand_].in_recently_clock_ = false;
            clock_hand_ = (clock_hand_ + 1) % frames_.size();
            victims_--;
            return true;
          }
        }
        frames_[clock_hand_].ref_bit_ = false;
          clock_hand_ = (clock_hand_ + 1) % frames_.size();
    }
    return false; 
}

void ClockReplacer::Pin(frame_id_t frame_id) {
   std::lock_guard<std::mutex>lock(mutex_);
   for (Frame &rframe : frames_) {
        if (rframe.id_ == frame_id) {
          if (rframe.in_recently_clock_) {
            victims_--;
          }
          // disable replacemen in clock by setting in_recently_clock to false.
          rframe.in_recently_clock_ = false;

          return;
        }
    } 
}

void ClockReplacer::Unpin(frame_id_t frame_id) { 
    std::lock_guard<std::mutex>lock(mutex_);
    for (Frame & rframe : frames_) {
        if (rframe.id_ == frame_id) {
          if (!rframe.in_recently_clock_) {
            victims_++;
          }
          rframe.ref_bit_ = true;
          rframe.in_recently_clock_ = true;
          return;
        }
    }
}

size_t ClockReplacer::Size() {
  std::lock_guard<std::mutex>lock(mutex_);
  return victims_;
}

}  // namespace bustub
