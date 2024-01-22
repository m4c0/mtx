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

struct mtx::cond::pimpl : pthread_cond_t {};

mtx::cond::cond() : m_handle{new mtx::cond::pimpl{}} {
  pthread_cond_init(&*m_handle, nullptr);
}
mtx::cond::~cond() {
  if (m_handle)
    pthread_cond_destroy(&*m_handle);
}

mtx::cond::cond(cond &&) = default;
mtx::cond &mtx::cond::operator=(cond &&) = default;

void mtx::cond::wait(lock *l) {
  timespec ts{};
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_nsec += 100 * 1000 * 1000;
  pthread_cond_timedwait(&*m_handle, &*l->m_mutex->m_handle, &ts);
}
void mtx::cond::wake_one() { pthread_cond_signal(&*m_handle); }
void mtx::cond::wake_all() { pthread_cond_broadcast(&*m_handle); }
