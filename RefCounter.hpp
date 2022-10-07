#pragma once

#include <Windows.h>
#include <functional>
#include <mutex>

namespace DxWrappers {

class RefCounter {
  std::mutex m_mutex;
  ULONG m_refCount;

public:
  RefCounter();
  ULONG getRefCount() const;
  ULONG incrementRef();
  ULONG decrementRef();
  ULONG decrementRef(std::function<void(ULONG)> block);
};

} // namespace DxWrappers
