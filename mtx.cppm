export module mtx;

namespace mtx {
export class mutex {};
export class lock {
public:
  lock(const mutex &) {}

  lock(const lock &) = delete;
  lock(lock &&) = delete;
  lock &operator=(const lock &) = delete;
  lock &operator=(lock &&) = delete;
};
} // namespace mtx
