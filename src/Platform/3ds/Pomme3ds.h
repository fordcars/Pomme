#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void Init3ds();
void Shutdown3ds();
void WaitForVBlank3ds();
void SwapBuffers3ds();

#ifdef __cplusplus
}
#endif