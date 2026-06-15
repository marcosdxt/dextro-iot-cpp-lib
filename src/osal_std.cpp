#include <dextro-iot/osal.h>
#include <thread>
#include <mutex>
#include <chrono>

namespace dextro {
namespace iot {

class StdMutex : public IMutex {
    std::mutex m_mutex;
public:
    void lock() override { m_mutex.lock(); }
    void unlock() override { m_mutex.unlock(); }
    bool try_lock() override { return m_mutex.try_lock(); }
};

class StdThread : public IThread {
    std::thread m_thread;
public:
    StdThread(std::function<void()> func) : m_thread(func) {}
    void join() override { if (m_thread.joinable()) m_thread.join(); }
    void detach() override { if (m_thread.joinable()) m_thread.detach(); }
};

class StdOSAL : public IOSAL {
public:
    std::unique_ptr<IMutex> create_mutex() override {
        return std::make_unique<StdMutex>();
    }

    std::unique_ptr<IThread> create_thread(std::function<void()> func) override {
        return std::make_unique<StdThread>(func);
    }

    void sleep_ms(uint32_t ms) override {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    uint64_t get_time_ms() override {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
        ).count();
    }
};

} // namespace iot
} // namespace dextro
