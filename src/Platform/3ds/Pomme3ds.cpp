#include <3ds.h>
#include "GL/picaGL.h"
#include <string>
#include <limits>
#include "Platform/3ds/Pomme3ds.h"

static unsigned g3dsNewlyDownButtons = 0;
static unsigned g3dsNewlyUpButtons = 0;
static unsigned g3dsHeldButtons = 0;
static float g3dsCPadX = 0.0f;
static float g3dsCPadY = 0.0f;

static std::string GetResultSummary(int summaryCode)
{
   switch(summaryCode)
   {
   case RS_SUCCESS:
      return "RS_SUCCESS";

   case RS_NOP:
      return "RS_NOP";

   case RS_WOULDBLOCK:
      return "RS_WOULDBLOCK";

   case RS_OUTOFRESOURCE:
      return "RS_OUTOFRESOURCE";

   case RS_NOTFOUND:
      return "RS_NOTFOUND";

   case RS_INVALIDSTATE:
      return "RS_INVALIDSTATE";

   case RS_NOTSUPPORTED:
      return "RS_NOTSUPPORTED";

   case RS_INVALIDARG:
      return "RS_INVALIDARG";

   case RS_WRONGARG:
      return "RS_WRONGARG";

   case RS_CANCELED:
      return "RS_CANCELED";

   case RS_STATUSCHANGED:
      return "RS_STATUSCHANGED";

   case RS_INTERNAL:
      return "RS_INTERNAL";

   case RS_INVALIDRESVAL:
      return "RS_INVALIDRESVAL";

   default:
      return "Unknown summary code: " + std::to_string(summaryCode);
   }
}

static void InitGFX()
{
   gfxInit(GSP_RGBA8_OES,GSP_RGB565_OES,false); 
   consoleInit(GFX_BOTTOM, consoleGetDefault());
   consoleDebugInit(debugDevice_CONSOLE);
   pglInit();
}

static void InitFS()
{
   int rc = romfsInit();
   if(rc)
   {
      std::string msg = "romfsInit error: " + GetResultSummary(R_SUMMARY(rc)) + "\n";
      printf(msg.c_str());
   }
}

void Init3ds()
{
    InitGFX();
    InitFS();
}

void Shutdown3ds()
{
   romfsExit();
   gfxExit();
   pglExit();
}

bool ShouldDoMainLoop3ds()
{
   return aptMainLoop();
}

void WaitForVBlank3ds()
{
   gspWaitForVBlank();
}

void SwapBuffers3ds()
{
   pglSwapBuffers();
}

void ScanInput3ds()
{
   hidScanInput();
   g3dsNewlyDownButtons = hidKeysDown();
   g3dsNewlyUpButtons = hidKeysUp();
   g3dsHeldButtons = hidKeysHeld();

   circlePosition pos;
   hidCircleRead(&pos);

   float range = std::numeric_limits<int16_t>::max() + 1.0f;
   g3dsCPadX = static_cast<float>(pos.dx) / range;
   g3dsCPadY = static_cast<float>(pos.dy) / range;
}

float Get3dsCPadX()
{
   return g3dsCPadX;
}

float Get3dsCPadY()
{
   return g3dsCPadY;
}

unsigned GetNewlyDownButtons3ds()
{
   return g3dsNewlyDownButtons;
}

unsigned GetNewlyUpButtons3ds()
{
   return g3dsNewlyUpButtons;
}

unsigned GetHeldButtons3ds()
{
   return g3dsHeldButtons;
}