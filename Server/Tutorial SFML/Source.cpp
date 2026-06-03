#include <SFML/Network.hpp>
#include <iostream>
#include "NetworkManager.h"
#include "MovementManager.h"
#include "ThreadManager.h"

void main()
{
    ThrdM->Init();
    NT->Init();
    DB->Connect();
    while (!NT->GetCloseServer()) {
        if (NT->CheckIfSocketsAreReadyToReceive()) {
            NT->Update();
            MovM->Update();
        }
    }
    DB->Disconnect();
}