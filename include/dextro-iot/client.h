#ifndef DEXTRO_IOT_CLIENT_H
#define DEXTRO_IOT_CLIENT_H

#include <dextro-iot/osal.h>
#include <dextro-iot/network.h>
#include <map>
#include <string>
#include <functional>
#include <vector>

namespace dextro {
namespace iot {

struct IotContext {
    std::string correlationId;
    std::string timestamp;
    // Permite expansão de metadados enviados pelo backend
};

using ProcedureHandler = std::function<std::string(const std::string& payload, const IotContext& context)>;

/**
 * @brief Cliente IoT Agnóstico (RPC + Mailbox).
 */
class IotClient {
public:
    IotClient(std::shared_ptr<IOSAL> osal, std::shared_ptr<INetworkProvider> net);
    ~IotClient();

    // Conectividade
    bool connect(const std::string& host, uint16_t port, const std::string& clientId);
    void disconnect();
    bool is_connected() const;

    // Discovery & RPC (Device Procedures)
    void register_procedure(const std::string& name, ProcedureHandler handler);

    // Remote Procedures
    void send_remote_procedure(const std::string& type, const std::string& payload, 
                             std::function<void(const std::string&)> on_response);

    // Mailbox
    void inbox_push(const std::string& type, const std::string& message);
    void outbox_pull();
    
    // Heartbeat & Status
    void set_status_provider(std::function<std::string()> status_func);

    // Loop de processamento (para ambientes sem threads nativas ou pump central)
    void process();

private:
    std::shared_ptr<IOSAL> m_osal;
    std::shared_ptr<INetworkProvider> m_net;
    std::unique_ptr<ISocket> m_socket;
    
    std::map<std::string, ProcedureHandler> m_procedures;
    std::function<std::string()> m_status_provider;

    // Métodos internos de protocolo
    void handle_list_procedures();
    void handle_device_status();
};

} // namespace iot
} // namespace dextro

#endif // DEXTRO_IOT_CLIENT_H
