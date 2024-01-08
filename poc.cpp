#pragma leco tool
import silog;
import sith;
import sitime;

class mutex {};
class lock {
public:
  lock(const mutex &) {}

  lock(const lock &) = delete;
  lock(lock &&) = delete;
  lock &operator=(const lock &) = delete;
  lock &operator=(lock &&) = delete;
};

class thread : public sith::thread {
  mutex *m_mutex{};
  int m_id;

public:
  thread(mutex *m, int i) : m_mutex{m}, m_id{i} {}
  void run() override {
    while (!interrupted()) {
      lock l{*m_mutex};
      silog::log(silog::info, "Running on thread %d", m_id);
      sitime::sleep_ms(m_id * 100);
    }
  }
};

int main() {
  mutex m{};

  thread a{&m, 0};
  a.start();

  thread b{&m, 1};
  b.start();

  sitime::sleep(1);
}
