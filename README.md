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

### 2. Registrando Procedimentos (Device Procedures)

```cpp
client.register_procedure("open-door", [](const std::string& payload) {
    // Lógica de hardware aqui
    return "{\"status\": \"opened\"}";
});
```

### 3. Enviando Eventos Garantidos (Inbox)

```cpp
client.inbox_push("ALARM", "{\"level\": \"critical\", \"msg\": \"Tamper detected\"}");
```

## 🏗️ Build (CMake)

```bash
mkdir build && cd build
cmake ..
make
```
