#pragma once

#include "callbacks.h"
#include <common/aglDrawContext.h>

void drawUI2D(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .05f });
    TextWriterPrintf(&writer, "Hi from UI 2D");
}
void drawUI2DOverlay(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .1f });
    TextWriterPrintf(&writer, "Hi from UI 2D Overlay");
}
void drawTool2D(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .15f });
    TextWriterPrintf(&writer, "Hi from Tool 2D");
}
void drawTool2DSuper(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .2f});
    TextWriterPrintf(&writer, "Hi from Tool 2D Super");

    // draw some random shapes but none of them show up :(
    gDrawMgr.setupRenderer(info);
    gDrawMgr.begin();
    gDrawMgr.drawLine(sead::Vector3f(0.f, 0.f, 0.f), sead::Vector3f(200.f, 0.f, 0.f), sead::Color4f(1.f, 1.f, 1.f, 1.f), sead::Color4f(1.f, 1.f, 1.f, 1.f));
    gDrawMgr.drawSphere4x8(sead::Vector3f(10.f, 10.f, 10.f), 100.f, sead::Color4f(1.f, 1.f, 1.f, 1.f), sead::Color4f(1.f, 1.f, 1.f, 1.f));
    gDrawMgr.drawAxis(sead::Vector3f(0.f, 0.f, 0.f), 100.f);
    gDrawMgr.end();
}
void drawMain3D0(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .25f });
    TextWriterPrintf(&writer, "Hi from Main 3D 0\n");

    // draw some random shapes but none of them show up :(
    gDrawMgr.setupRenderer(info);
    gDrawMgr.begin();
    gDrawMgr.drawLine(sead::Vector3f(0.f, 0.f, 0.f), sead::Vector3f(200.f, 0.f, 0.f), sead::Color4f(1.f, 1.f, 1.f, 1.f), sead::Color4f(1.f, 1.f, 1.f, 1.f));
    gDrawMgr.drawSphere4x8(sead::Vector3f(10.f, 10.f, 10.f), 100.f, sead::Color4f(1.f, 1.f, 1.f, 1.f), sead::Color4f(1.f, 1.f, 1.f, 1.f));
    gDrawMgr.drawAxis(sead::Vector3f(0.f, 0.f, 0.f), 100.f);
    gDrawMgr.end();
}
void drawUI3D0(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursorFromTopLeft({ writer.getWidth() * .05f, writer.getHeight() * .3f });
    TextWriterPrintf(&writer, "Hi from UI 3D 0");
}