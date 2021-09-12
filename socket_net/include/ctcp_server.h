#pragma once

#include <functional>

#include "iserver.h"

class CTcpServer : public IServer
{
public:
    explicit CTcpServer(const char* running_ip, uint16_t running_port) noexcept(false);
    inline ~CTcpServer() = default;

    void initServer() noexcept(false) override;
    size_t acceptClient() noexcept(false);
    inline void setAllocSocketFunc(std::function<std::unique_ptr<CSocket>(SOCKET, std::unique_ptr<sockaddr_in>)> fnc) { mAllocSocketFunction = fnc; }

    const CSocket* getClient(size_t idx) const noexcept(false);
    const CSocket* getLastClient() const noexcept(false);

    int32_t operator<< (const IPackage* pkg) noexcept(false) = delete;
    int32_t operator>> (IPackage* pkg) noexcept(false) = delete;
    int32_t operator<< (const char* message) const noexcept(false) = delete;
    int32_t operator<< (const std::string& message) const noexcept(false) = delete;
    template<class T>
    int32_t operator<< (const std::vector<T>& data) const noexcept(false) = delete;
protected:
    std::vector<std::unique_ptr<CSocket>> mClientSockets;
    std::function<std::unique_ptr<CSocket>(SOCKET, std::unique_ptr<sockaddr_in>)> mAllocSocketFunction = nullptr;
};
