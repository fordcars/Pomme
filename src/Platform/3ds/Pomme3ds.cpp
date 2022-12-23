#include <3ds.h>
#include "Platform/3ds/Pomme3ds.h"

void Pomme::Platform::N3DS::Init()
{
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, consoleGetDefault());
    consoleDebugInit(debugDevice_CONSOLE);
}