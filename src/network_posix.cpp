#include <dextro-iot/network.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

namespace dextro {
namespace iot {

class PosixSocket : public ISocket {
    int m_fd = -1;
public:
    ~PosixSocket() { disconnect(); }

    bool connect(const char* host, uint16_t port) override {
        struct sockaddr_in serv_addr;
        m_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (m_fd < 0) return false;

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0) {
            struct hostent *server = gethostbyname(host);
            if (!server) return false;
            memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
        }

        if (::connect(m_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            return false;
        }
        return true;
    }

    void disconnect() override {
        if (m_fd >= 0) {
            close(m_fd);
            m_fd = -1;
        }
    }

    int send(const uint8_t* data, size_t len) override {
        return ::send(m_fd, data, len, 0);
    }

    int receive(uint8_t* buffer, size_t max_len, uint32_t timeout_ms) override {
        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
        setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        return ::recv(m_fd, buffer, max_len, 0);
    }

    bool is_connected() const override { return m_fd >= 0; }
};

class PosixNetworkProvider : public INetworkProvider {
public:
    std::unique_ptr<ISocket> create_tcp_socket() override {
        return std::make_unique<PosixSocket>();
    }
    std::unique_ptr<ISocket> create_tls_socket(const char*, const char*, const char*) override {
        // Para o TestBench inicial, usaremos TCP puro ou precisaremos de OpenSSL
        return std::make_unique<PosixSocket>(); 
    }
};

} // namespace iot
} // namespace dextro
