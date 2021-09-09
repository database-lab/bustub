//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) 
:num_pages_(num_pages),
 frames_(std::list<frame_id_t>()),
 frame_tables_(std::unordered_map<frame_id_t, std::list<frame_id_t>::iterator>()) {
     
 }

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (frames_.empty()) {
    return false;
  }

    *frame_id = frames_.back();
    frame_tables_.erase(frames_.back());
    frames_.pop_back();
    return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  std::lock_guard<std::mutex> lock(mutex_);
  // find iterator
  auto it = frame_tables_.find(frame_id);
  if (it == frame_tables_.end()) {
    return;
  }

  frames_.erase(it->second);
  frame_tables_.erase(frame_id);
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = frame_tables_.find(frame_id);
  if (it== frame_tables_.end()) {
    // frame is fully.
    if (frames_.size() == num_pages_) {
      return;
    }

    frames_.push_front(frame_id);
    frame_tables_[frame_id] = frames_.begin();
    return;
  }
}

size_t LRUReplacer::Size() { 
  std::lock_guard<std::mutex> lock(mutex_);
  return frames_.size(); 
}

}  // namespace bustub
