/*
 Copyright (c) 2016, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_THREAD_COUNTER_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_THREAD_COUNTER_H_

#include "utils/threads/thread.h"

namespace threads {
/**
 * @brief The ThreadCounter class is helpful for debugging purpose.
 * ThreadCounter counts number of threads created by
 * Thread::CreateThread method
 * In case when not all threads destroyed in time, DCHECK will be called
 * Helps to find and resolve multi threading issues.
 */
class ThreadCounter {
 public:
  ThreadCounter();
  ~ThreadCounter();
  /**
 * @brief Increment method increments thread counter value
 */
  static void Increment();
  /**
   * @brief Decrement method decrements thread counter value
   */
  static void Decrement();
  /**
   * @brief GetValue method gets thread counter value
   * @return thread counter
   */
  static size_t GetValue();

 private:
  static size_t thread_counter_;
  static sync_primitives::Lock counter_lock_;
};

}  // namespace threads

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_THREADS_THREAD_COUNTER_H_
