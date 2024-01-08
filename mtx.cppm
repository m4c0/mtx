export module mtx;
import hai;

namespace mtx {
struct pimpl;

export class mutex {
  hai::uptr<pimpl> m_handle;

  friend class lock;

public:
  mutex();
  ~mutex();

  mutex(const mutex &) = delete;
  mutex(mutex &&) = delete;
  mutex &operator=(const mutex &) = delete;
  mutex &operator=(mutex &&) = delete;
};

export class lock {
  mutex *m_mutex;

public:
  lock(mutex *m);
  ~lock();

  lock(const lock &) = delete;
  lock(lock &&) = delete;
  lock &operator=(const lock &) = delete;
  lock &operator=(lock &&) = delete;
};
} // namespace mtx

#if LECO_TARGET_WINDOWS
#pragma leco add_impl "windows.cpp"
#else
#pragma leco add_impl "pthread.cpp"
#endif
