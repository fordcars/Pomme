#ifdef ENABLE_USAGE_VISUALIZER_3DS

#include "UsageVisualizer3ds.h"
#include <3ds.h>
#include <GL/picaGL.h>
#include <malloc.h>

namespace
{

constexpr unsigned UPPER_SCREEN_WIDTH = 400;
constexpr unsigned LOWER_SCREEN_WIDTH = 320;
constexpr unsigned SCREEN_HEIGHT = 240;
constexpr float LEFT_MARGIN = 0.1;
constexpr float RIGHT_MARGIN = 0.1;
constexpr float TOP_MARGIN = 0.3;
constexpr size_t USEABLE_RAM_3DS = 128000000; // In bytes

void setupRendering()
{
    pglSelectScreen(GFX_BOTTOM, GFX_LEFT);
    glViewport(0, 0, LOWER_SCREEN_WIDTH, SCREEN_HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION_MATRIX);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glLoadIdentity();
    glPushMatrix();
}

void endRendering()
{
    pglSwapBuffers();

    glMatrixMode(GL_PROJECTION_MATRIX);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glPopMatrix();

    pglSelectScreen(GFX_TOP, GFX_LEFT);
    glViewport(0, 0, UPPER_SCREEN_WIDTH, SCREEN_HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void graphAllocatedSpace(const struct mallinfo& info)
{
    float usagePercentage = (float)info.uordblks/(float)USEABLE_RAM_3DS;
    float totalBarLength = 1.0f - RIGHT_MARGIN - (-1.0f + LEFT_MARGIN);
    float usageBarLength = totalBarLength * usagePercentage;

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.5, -1.0f + LEFT_MARGIN, 0.0f);
        glVertex3f(0.5, 1.0f - RIGHT_MARGIN, 0.0f);
        glVertex3f(0.7, 1.0f - RIGHT_MARGIN, 0.0f);
        glVertex3f(0.7, -1.0f + LEFT_MARGIN, 0.0f);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.5, -1.0f + RIGHT_MARGIN + (totalBarLength - usageBarLength), 0.0f);
        glVertex3f(0.5, 1.0f - LEFT_MARGIN, 0.0f);
        glVertex3f(0.7, 1.0f - LEFT_MARGIN, 0.0f);
        glVertex3f(0.7, -1.0f + RIGHT_MARGIN + (totalBarLength - usageBarLength), 0.0f);
    glEnd();
}

}

namespace UsageVisualizer3ds
{

void update()
{
    struct mallinfo info = mallinfo();

    setupRendering();
    graphAllocatedSpace(info);
    endRendering();
}

} // UsageVisualizer3ds

#endif // ENABLE_USAGE_VISUALIZER_3DS