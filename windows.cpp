module;
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

module mtx;

struct mtx::pimpl {
  HANDLE h;
};

mtx::mutex::mutex() : m_handle{new mtx::pimpl{}} {
  m_handle->h = CreateMutex(nullptr, false, nullptr);
}
mtx::mutex::~mutex() {
  if (m_handle)
    CloseHandle(m_handle->h);
}

mtx::mutex::mutex(mutex &&) = default;
mtx::mutex &mtx::mutex::operator=(mutex &&) = default;

mtx::lock::lock(mtx::mutex *m) : m_mutex{m} {
  WaitForSingleObject(m_handle->h, INFINITE);
}
mtx::lock::~lock() { ReleaseMutex(m_handle->h); }
