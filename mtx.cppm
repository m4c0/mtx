export module mtx;
import hai;
import no;

namespace mtx {
export class mutex : no::copy {
  struct pimpl;
  hai::uptr<pimpl> m_handle;

  friend class cond;
  friend class lock;

public:
  mutex();
  ~mutex();

  mutex(mutex &&);
  mutex &operator=(mutex &&);
};

export class lock : no::no {
  mutex *m_mutex;

  friend class cond;

public:
  lock(mutex *m);
  ~lock();
};

export class cond : no::copy {
  struct pimpl;
  hai::uptr<pimpl> m_handle;

public:
  cond();
  ~cond();

  cond(cond &&);
  cond &operator=(cond &&);

  void wait(lock *);
  void wait(lock *, unsigned seconds);
  void wake_one();
  void wake_all();
};
} // namespace mtx

#if LECO_TARGET_WINDOWS
#pragma leco add_impl "windows.cpp"
#else
#pragma leco add_impl "pthread.cpp"
#endif
