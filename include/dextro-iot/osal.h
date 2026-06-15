#ifndef DEXTRO_IOT_OSAL_H
#define DEXTRO_IOT_OSAL_H

#include <string>
#include <functional>
#include <memory>

namespace dextro {
namespace iot {

/**
 * @brief Interface para abstração de Mutex.
 */
class IMutex {
public:
    virtual ~IMutex() = default;
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual bool try_lock() = 0;
};

/**
 * @brief Interface para abstração de Threads/Tasks.
 */
class IThread {
public:
    virtual ~IThread() = default;
    virtual void join() = 0;
    virtual void detach() = 0;
};

/**
 * @brief Factory para criação de primitivas de OS.
 * A aplicação injeta uma implementação concreta desta factory.
 */
class IOSAL {
public:
    virtual ~IOSAL() = default;
    virtual std::unique_ptr<IMutex> create_mutex() = 0;
    virtual std::unique_ptr<IThread> create_thread(std::function<void()> func) = 0;
    virtual void sleep_ms(uint32_t ms) = 0;
    virtual uint64_t get_time_ms() = 0;
};

} // namespace iot
} // namespace dextro

#endif // DEXTRO_IOT_OSAL_H
