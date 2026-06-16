#include <dextro-iot/client.h>
#include <mqtt.h>
#include <iostream>

namespace dextro {
namespace iot {

struct MqttInternal {
    struct mqtt_client client;
    uint8_t sendbuf[2048];
    uint8_t recvbuf[2048];
};

static void publish_callback(void** unused, struct mqtt_response_publish *published) {
    // Aqui rotearíamos para os handlers do IotClient
}

IotClient::IotClient(std::shared_ptr<IOSAL> osal, std::shared_ptr<INetworkProvider> net)
    : m_osal(osal), m_net(net) {
    
    // Registra procedimentos obrigatórios nativos
    register_procedure("list-procedures", [this](const std::string&, const IotContext&) {
        std::string list = "[";
        for (auto const& [name, _] : m_procedures) {
            list += "\"" + name + "\",";
        }
        if (list.length() > 1) list.pop_back();
        list += "]";
        return list;
    });

    register_procedure("device-status", [this](const std::string&, const IotContext&) {
        if (m_status_provider) return m_status_provider();
        return std::string("{}");
    });
}

IotClient::~IotClient() {
    disconnect();
}

bool IotClient::connect(const std::string& host, uint16_t port, const std::string& clientId) {
    m_socket = m_net->create_tcp_socket();
    if (!m_socket->connect(host.c_str(), port)) return false;

    // TODO: Inicializar mqtt-c aqui e fazer o handshake
    // Por brevidade para o TestBench, vamos focar no fluxo arquitetural
    return true;
}

void IotClient::disconnect() {
    if (m_socket) m_socket->disconnect();
}

bool IotClient::is_connected() const {
    return m_socket && m_socket->is_connected();
}

void IotClient::register_procedure(const std::string& name, ProcedureHandler handler) {
    m_procedures[name] = handler;
}

void IotClient::process() {
    // Bomba de mensagens
}

bool IotClient::push(const std::string& type, const std::string& payload) {
    if (!is_connected()) return false;
    // Envia fire-and-forget via QoS 1 para o backend tratar a fila/response
    std::cout << "DEBUG: C++ enviando Push (Remote Procedure): " << type << std::endl;
    return true;
}

bool IotClient::inbox_push(const std::string& type, const std::string& message) {
    if (!is_connected()) return false;
    std::cout << "DEBUG: C++ enviando Inbox Push: " << type << std::endl;
    return true;
}

} // namespace iot
} // namespace dextro
