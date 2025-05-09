#pragma leco tool
import hai;
import mtx;
import silog;
import sith;
import sitime;

using namespace mtx;

mutex g_mutex{};
cond g_cond{};
int g_count{};
class producer : public sith::thread {
  void wake_one() {
    lock l{&g_mutex};
    g_count++;
    g_cond.wake_one();
  }
  void wake_all() {
    {
      // This is an example of how "wake" doesn't require the lock. We only use
      // it here as a guard for the count increment
      lock l{&g_mutex};
      g_count++;
    }
    g_cond.wake_all();
  }

public:
  void run() override {
    for (auto i = 0; i < 10; i++) {
      wake_one();
      sitime::sleep_ms(1);
      wake_one();
      sitime::sleep_ms(1);
      wake_all();
      sitime::sleep_ms(1);
    }
  }
};
class consumer : public sith::thread {
  int m_id;

public:
  consumer(int id) : m_id{id} {}

  void run() override {
    int last{};
    while (!interrupted()) {
      lock l{&g_mutex};
      while (g_count <= last && !interrupted()) {
        g_cond.wait(&l);
      }
      if (interrupted()) {
        return;
      }
      last = g_count;
      silog::log(silog::info, "%d consumed by %d", g_count, m_id);
    }
  }
};
void cond_example() {
  silog::log(silog::info, "-=-=-=-=-=-=- Conditional variable example");

  cond cv{};
  producer p{};
  consumer c1{1};
  consumer c2{2};

  sith::run_guard c1g { &c1 };
  sith::run_guard c2g { &c2 };
  sith::run_guard pg { &p };

  sitime::sleep(4);
}

class thread : public sith::thread {
  mutex *m_mutex{};
  int m_id;

public:
  thread(mutex *m, int i) : m_mutex{m}, m_id{i} {}
  void run() override {
    while (!interrupted()) {
      lock l{m_mutex};
      silog::log(silog::info, "Running on thread %d", m_id);
      sitime::sleep_ms(m_id);
    }
  }

  static void example() {
    silog::log(silog::info, "-=-=-=-=-=-=- Mutex example");
    mutex m{};

    thread a{&m, 10};
    sith::run_guard ag { &a };

    thread b{&m, 1};
    sith::run_guard bg { &b };

    sitime::sleep(1);
  }
};

int main() {
#warning this example runs indefinitely sometimes
  thread::example();
  cond_example();
}
