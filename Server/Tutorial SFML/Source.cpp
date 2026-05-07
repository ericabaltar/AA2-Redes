#include <SFML/Network.hpp>
#include <iostream>
#include "NetworkManager.h"

void main()
{
    NT->Init();
    DB->Connect();
    while (!NT->GetCloseServer()) {
        if (NT->CheckIfSocketsAreReadyToReceive()) {
            NT->Update();
        }
    }
    DB->Disconnect();
}