module;
#include <pthread.h>

module mtx;

struct mtx::mutex::pimpl : pthread_mutex_t {};

mtx::mutex::mutex() : m_handle{new mtx::mutex::pimpl{}} {
  pthread_mutex_init(&*m_handle, nullptr);
}
mtx::mutex::~mutex() {
  if (m_handle)
    pthread_mutex_destroy(&*m_handle);
}

mtx::mutex::mutex(mutex &&) = default;
mtx::mutex &mtx::mutex::operator=(mutex &&) = default;

mtx::lock::lock(mtx::mutex *m) : m_mutex{m} {
  pthread_mutex_lock(&*(m_mutex->m_handle));
}
mtx::lock::~lock() { pthread_mutex_unlock(&*(m_mutex->m_handle)); }
