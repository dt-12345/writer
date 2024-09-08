#pragma once

#include "callbacks.h"

void drawUI2D(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {}
void drawUI2DOverlay(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {}
void drawTool2D(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {}
void drawTool2DSuper(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {
    sead::TextWriter writer = initializeTextWriter(info);
    writer.setCursor(0.f, 0.f);
    TextWriterPrintf(&writer, "Hello World");
}
void drawMain3D0(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {}
void drawUI3D0(agl::lyr::Layer* layer, const agl::lyr::RenderInfo& info) {}