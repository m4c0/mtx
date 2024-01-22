module;
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

module mtx;
import no;

// Windows definition of "Critical Objects" has two main features:
// - Single-process (mutexes supports multi-process - for MS reasons)
// - Uses processor primitives instead of kernel calls
// Therefore, they are more lightweight and faster. And they allow usage of
// other sync objects, like conditional variables
//
// Ref:
// https://learn.microsoft.com/en-us/windows/win32/sync/critical-section-objects
struct mtx::mutex::pimpl : no::no {
  CRITICAL_SECTION cs{};

  pimpl() : h{InitializeCriticalSection(&cs)} {}
  ~pimpl() { DeleteCriticalSection(&cs); }
};

mtx::mutex::mutex() : m_handle{new mtx::mutex::pimpl{}} {}
mtx::mutex::~mutex() = default;

mtx::mutex::mutex(mutex &&) = default;
mtx::mutex &mtx::mutex::operator=(mutex &&) = default;

mtx::lock::lock(mtx::mutex *m) : m_mutex{m} {
  EnterCriticalSection(m_mutex->m_handle->h);
}
mtx::lock::~lock() { LeaveCriticalSection(m_mutex->m_handle->h); }
