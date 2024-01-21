module;
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

module mtx;
import no;

struct mtx::pimpl : no::no {
  HANDLE h{INVALID_HANDLE_VALUE};

  pimpl() : h{CreateMutex(nullptr, false, nullptr)} {}
  ~pimpl() {
    if (h != INVALID_HANDLE_VALUE)
      CloseHandle(h);
  }
};

mtx::mutex::mutex() : m_handle{new mtx::pimpl{}} {}
mtx::mutex::~mutex() = default;

mtx::mutex::mutex(mutex &&) = default;
mtx::mutex &mtx::mutex::operator=(mutex &&) = default;

mtx::lock::lock(mtx::mutex *m) : m_mutex{m} {
  WaitForSingleObject(m_mutex->m_handle->h, INFINITE);
}
mtx::lock::~lock() { ReleaseMutex(m_mutex->m_handle->h); }
