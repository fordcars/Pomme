#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// From https://libctru.devkitpro.org/hid_8h_source.html
enum
{
    POMME_3DS_KEY_A            = 1 << 0, ///< A
    POMME_3DS_KEY_B            = 1 << 1, ///< B
    POMME_3DS_KEY_SELECT       = 1 << 2, ///< Select
    POMME_3DS_KEY_START        = 1 << 3, ///< Start
    POMME_3DS_KEY_DRIGHT       = 1 << 4, ///< D-Pad Right
    POMME_3DS_KEY_DLEFT        = 1 << 5, ///< D-Pad Left
    POMME_3DS_KEY_DUP          = 1 << 6, ///< D-Pad Up
    POMME_3DS_KEY_DDOWN        = 1 << 7, ///< D-Pad Down
    POMME_3DS_KEY_R            = 1 << 8, ///< R
    POMME_3DS_KEY_L            = 1 << 9, ///< L
    POMME_3DS_KEY_X            = 1 << 10, ///< X
    POMME_3DS_KEY_Y            = 1 << 11, ///< Y
    POMME_3DS_KEY_ZL           = 1 << 14, ///< ZL (New 3DS only)
    POMME_3DS_KEY_ZR           = 1 << 15, ///< ZR (New 3DS only)
    POMME_3DS_KEY_TOUCH        = 1 << 20, ///< Touch (Not actually provided by HID)
    POMME_3DS_KEY_CSTICK_RIGHT = 1 << 24, ///< C-Stick Right (New 3DS only)
    POMME_3DS_KEY_CSTICK_LEFT  = 1 << 25, ///< C-Stick Left (New 3DS only)
    POMME_3DS_KEY_CSTICK_UP    = 1 << 26, ///< C-Stick Up (New 3DS only)
    POMME_3DS_KEY_CSTICK_DOWN  = 1 << 27, ///< C-Stick Down (New 3DS only)
    POMME_3DS_KEY_CPAD_RIGHT   = 1 << 28, ///< Circle Pad Right
    POMME_3DS_KEY_CPAD_LEFT    = 1 << 29, ///< Circle Pad Left
    POMME_3DS_KEY_CPAD_UP      = 1 << 30, ///< Circle Pad Up
    POMME_3DS_KEY_CPAD_DOWN    = 1 << 31, ///< Circle Pad Down
};

void Init3ds();
void Shutdown3ds();
bool ShouldDoMainLoop3ds();
void WaitForVBlank3ds();
void SwapBuffers3ds();

void ScanInput3ds();
float Get3dsCPadX(); // [-1, 1]
float Get3dsCPadY(); // [-1, 1]
unsigned GetNewlyDownButtons3ds();
unsigned GetNewlyUpButtons3ds();
unsigned GetHeldButtons3ds();

#ifdef __cplusplus
}
#endif