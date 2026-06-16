# Dextro IoT - C++ Linux Library

Biblioteca nativa em C++17 para integração de dispositivos Linux ao ecossistema Dextro IoT. Totalmente desacoplada de frameworks de UI (como Qt) e agnóstica a Sistema Operacional.

## 🚀 Arquitetura do Protocolo

Baseada no modelo **Stateless RPC & Mailbox**:
- **Agnosticismo**: O core não gerencia arquivos ou sockets diretamente.
- **RPC Discovery**: Suporte nativo a `list-procedures` e `device-status`.
- **Reliability**: Implementação cliente do padrão Inbox/Outbox para dados persistentes.

## 🛠️ Camadas (OSAL & HAL)

A biblioteca utiliza injeção de dependência para garantir portabilidade:
- **`IOSAL`**: Abstração de Threads e Mutexes.
- **`INetworkProvider`**: Abstração de transporte (TCP/TLS).
- **`IotClient`**: Core lógico do protocolo.

## 💻 Exemplo de Uso

### 1. Inicialização com OSAL Padrão

```cpp
#include <dextro-iot/client.h>
#include <dextro-iot/osal_std.h> // Implementação baseada em std::thread

auto osal = std::make_shared<dextro::iot::StdOSAL>();
auto net = std::make_shared<dextro::iot::PosixNetworkProvider>();

dextro::iot::IotClient client(osal, net);
```

### 2. Registrando Procedimentos (Com Contexto)

A biblioteca passa um `IotContext` contendo o `correlationId` e outros metadados recebidos do broker. O desenvolvedor deve parsear o payload (via `nlohmann::json`, por exemplo).

```cpp
client.register_procedure("open-door", [](const std::string& payload, const dextro::iot::IotContext& ctx) {
    std::cout << "Recebido comando com Correlation ID: " << ctx.correlationId << std::endl;
    std::cout << "Metadata injetado: " << ctx.metadata << std::endl;
    
    // Lógica de hardware aqui
    return "{\"status\": \"opened\"}";
});
```

### 3. Solicitando Ações ao Backend (Remote Procedures)

A API foi simplificada para um modelo semântico `push()`, ocultando os tópicos MQTT subjacentes.

```cpp
// Solicita que o backend processe um evento de acesso
client.push("access-log", "{\"userId\": \"12345\", \"action\": \"entry\"}");
```

### 4. Enviando Eventos Garantidos (Inbox)

```cpp
client.inbox_push("ALARM", "{\"level\": \"critical\", \"msg\": \"Tamper detected\"}");
```

## 🏗️ Build (CMake)

```bash
mkdir build && cd build
cmake ..
make
```
