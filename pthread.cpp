module;
#include <pthread.h>

module mtx;

struct mtx::pimpl : pthread_mutex_t {};

mtx::mutex::mutex() : m_handle{new mtx::pimpl{}} {
  pthread_mutex_init(&*m_handle, nullptr);
}
mtx::mutex::~mutex() { pthread_mutex_destroy(&*m_handle); }

mtx::lock::lock(mtx::mutex *m) : m_mutex{m} {
  pthread_mutex_lock(&*(m_mutex->m_handle));
}
mtx::lock::~lock() { pthread_mutex_unlock(&*(m_mutex->m_handle)); }
