#include <dextro-iot/client.h>
#include <iostream>
#include <memory>
#include <mqtt.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int open_socket(const char* addr, const char* port) {
    struct addrinfo hints = {0}, *addrs;
    hints.ai_family = AF_INET; // Força IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo(addr, port, &hints, &addrs) != 0) return -1;
    int fd = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
    if (fd < 0) return -1;
    if (connect(fd, addrs->ai_addr, addrs->ai_addrlen) < 0) {
        close(fd);
        return -1;
    }
    freeaddrinfo(addrs);
    return fd;
}

int main(int argc, char** argv) {
    if (argc < 3) return 1;

    const char* host = argv[1];
    const char* port = argv[2];

    int sockfd = open_socket(host, port);
    if (sockfd < 0) {
        std::cerr << "❌ Erro ao abrir socket" << std::endl;
        return 1;
    }

    struct mqtt_client client;
    uint8_t sendbuf[2048], recvbuf[2048];
    mqtt_init(&client, sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), NULL);
    
    mqtt_connect(&client, "DEV-NATIVE-001", NULL, NULL, 0, NULL, NULL, 0, 400);
    
    // Loop de Handshake
    for(int i=0; i<50; ++i) {
        mqtt_sync(&client);
        if (client.error != MQTT_OK) {
            std::cerr << "❌ Erro MQTT durante handshake: " << mqtt_error_str(client.error) << std::endl;
            break;
        }
        usleep(100000); // 100ms
    }

    if (client.error == MQTT_OK) {
        std::cout << "✅ Conectado (Handshake Finalizado)" << std::endl;

        const char* topic = "test/native";
        const char* payload = "{\"status\":\"alive\"}";
        mqtt_publish(&client, topic, payload, strlen(payload), MQTT_PUBLISH_QOS_0);

        std::cout << "💓 Heartbeat enviado para test/native" << std::endl;

        for(int i=0; i<30; ++i) {
            mqtt_sync(&client);
            usleep(100000);
        }
    }

    return 0;
}
