#pragma once

#include "callbacks.h"
#include <common/aglDrawContext.h>

void drawUI2D(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .05f });
    writer.printf("Hi from UI 2D");
}
void drawUI2DOverlay(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .1f });
    writer.printf("Hi from UI 2D Overlay");
}
void drawTool2D(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .15f });
    writer.printf("Hi from Tool 2D");
}
void drawTool2DSuper(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .2f});
    writer.printf("Hi from Tool 2D Super");
}
// void drawMain3D0(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
//     sead::TextWriter writer = initializeTextWriter(info);
//     writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .25f });
//     writer.printf("Hi from Main 3D 0\n");

//     // draw some random shapes but none of them show up :(
//     gDrawMgr.setupRenderer(info);
//     gDrawMgr.begin();
//     gDrawMgr.drawCircle32(sead::Vector3f(0.f, 1.f, 0.f), 50.f, sead::Color4f(1.f, 0.f, 1.f, 1.f));
//     gDrawMgr.drawCircle32(sead::Vector3f(0.f, 0.f, 1.f), 50.f, sead::Color4f(1.f, 0.f, 0.f, 1.f));
//     gDrawMgr.drawCircle32(sead::Vector3f(1.f, 0.f, 0.f), 50.f, sead::Color4f(0.f, 0.f, 1.f, 1.f));
//     gDrawMgr.end();
// }
void drawUI3D0(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .3f });
    writer.printf("Hi from UI 3D 0");
}