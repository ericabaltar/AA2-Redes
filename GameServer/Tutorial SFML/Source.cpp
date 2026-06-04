#include <SFML/Network.hpp>
#include <iostream>
#include "NetworkManager.h"
#include "ThreadManager.h"

void main()
{
    NT->Init();
    ThrdM->Init();
    while (!NT->GetCloseServer()) {
        NT->Update();
    }
}