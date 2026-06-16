#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include "MovementPacket.h"

class MovementReconciliation
{
private:
    std::vector<MovementPacket> pendingPackets;

public:
    void AddPendingPacket(const MovementPacket& packet)
    {
        pendingPackets.push_back(packet);
    }

    void Reconcile(
        Vector2 clientPosition,
        const Vector2& serverPosition,
        int lastValidatedPacketID
    )
    {
        clientPosition = serverPosition;

        pendingPackets.erase(
            std::remove_if(
                pendingPackets.begin(),
                pendingPackets.end(),
                [lastValidatedPacketID](const MovementPacket& packet)
                {
                    return packet.ID <= lastValidatedPacketID;
                }
            ),
            pendingPackets.end()
        );

        std::cout << "Reconciliacion aplicada. Ultimo paquete validado: "
            << lastValidatedPacketID << std::endl;
    }

    const std::vector<MovementPacket>& GetPendingPackets() const
    {
        return pendingPackets;
    }

    void Clear()
    {
        pendingPackets.clear();
    }
};