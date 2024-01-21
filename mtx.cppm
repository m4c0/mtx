export module mtx;
import hai;
import no;

namespace mtx {
struct pimpl;

export class mutex : no::copy {
  hai::uptr<pimpl> m_handle;

  friend class lock;

public:
  mutex();
  ~mutex();

  mutex(mutex &&);
  mutex &operator=(mutex &&);
};

export class lock : no::no {
  mutex *m_mutex;

public:
  lock(mutex *m);
  ~lock();
};
} // namespace mtx

#if LECO_TARGET_WINDOWS
#pragma leco add_impl "windows.cpp"
#else
#pragma leco add_impl "pthread.cpp"
#endif
