#pragma once

#include <sead/math/seadBoundBox.h>
#include <sead/math/seadVector.h>
#include <sead/gfx/seadColor.h>

namespace sead {

class Camera;
class DrawContext;
class Viewport;

class TextWriter {
public:
    ~TextWriter() { __vftable->dtor2(this); }
    
    void setProjection(Projection* projection) { mProjection = projection; }
    void setCamera(Camera* camera) { mCamera = camera; }
    void setViewport(Viewport* viewport) { mViewport = viewport; }

    void setScale(const Vector2f& scale) { mScale = scale; }
    void setScale(float x, float y) { mScale = { x, y }; }
    void setCursor(const Vector2f& cursor) { mCursor = cursor; }
    void setCursor(float x, float y) { mCursor = { x, y }; }

    void setColor(const Color4f& color) { mColor = color; }
    void setColor(float r, float g, float b, float a) { mColor = { r, g, b, a}; }
    void setColor(float r, float g, float b) { mColor = { r, g, b, mColor.a }; }

private:
    struct vtable {
        void (*dtor0)(TextWriter* tw);
        void (*dtor2)(TextWriter* tw);
    };

    static vtable* sVtable;

    vtable* __vftable;
    Viewport* mViewport;
    void* mProjection;
    void* mCamera;
    void* mFont;
    Vector2f mCursor;
    Vector2f mScale{ 1.f, 1.f };
    Color4f mColor{ 1.f, 1.f, 1.f, 1.f };
    BoundBox2f _48;
    float mLineSpace;
    float _5C;
    wchar_t* mFormatBuffer;
    int mFormatBufferLength;
    bool mDoFontBeginEnd;
    DrawContext* mDrawCtx;
};
static_assert(sizeof(TextWriter) == 0x78);

} // namespace sead