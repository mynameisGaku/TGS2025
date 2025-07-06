#pragma once
#include <framework/gameObject.h>
#include <src/common/network/NetworkInstance.h>
#include <vector>

class NetworkManager
{
public:
    NetworkManager();
    ~NetworkManager();

    void Update() override;
    void Draw() override;

private:
    std::vector<NetworkInstance> m_NetworkInstances;
};