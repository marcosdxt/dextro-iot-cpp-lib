#ifndef DEXTRO_IOT_NETWORK_H
#define DEXTRO_IOT_NETWORK_H

#include <cstdint>
#include <cstddef>
#include <memory>

namespace dextro {
namespace iot {

/**
 * @brief Interface para abstração de Sockets (Transporte).
 */
class ISocket {
public:
    virtual ~ISocket() = default;
    
    virtual bool connect(const char* host, uint16_t port) = 0;
    virtual void disconnect() = 0;
    
    virtual int send(const uint8_t* data, size_t len) = 0;
    virtual int receive(uint8_t* buffer, size_t max_len, uint32_t timeout_ms) = 0;
    
    virtual bool is_connected() const = 0;
};

/**
 * @brief Factory para criação de conexões de rede.
 */
class INetworkProvider {
public:
    virtual ~INetworkProvider() = default;
    virtual std::unique_ptr<ISocket> create_tcp_socket() = 0;
    virtual std::unique_ptr<ISocket> create_tls_socket(const char* ca_cert, const char* client_cert, const char* client_key) = 0;
};

} // namespace iot
} // namespace dextro

#endif // DEXTRO_IOT_NETWORK_H
