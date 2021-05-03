#pragma once

#include "iserver.h"

class CTcpServer : public IServer
{
public:
    CTcpServer(const char* ip, uint16_t port);
    ~CTcpServer() = default;

    void initServer() noexcept(false) override;
    void acceptClient(std::unique_ptr<SocketInfo>& client_info) noexcept(false);
    void acceptClient(SocketInfo* client_info) noexcept(false);
    void addClient(std::unique_ptr<CSocket> client_socket) noexcept;

    const CSocket* getClient(uint8_t idx) const noexcept(false);
    const CSocket* getLastClient() const noexcept(false);

    int32_t operator<< (const IPackage* pkg) noexcept(false) = delete;
    int32_t operator>> (IPackage* pkg) noexcept(false) = delete;
    int32_t operator<< (const char* message) const noexcept(false) = delete;
    int32_t operator<< (const std::string& message) const noexcept(false) = delete;
    template<class T>
    int32_t operator<< (const std::vector<T>& data) const noexcept(false) = delete;
protected:
    std::vector<std::unique_ptr<CSocket>> mClientSockets;
};
