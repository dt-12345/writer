#pragma once

#include <sead/devenv/seadFontMgr.h>
#include <sead/math/seadBoundBox.h>
#include <sead/math/seadVector.h>
#include <sead/gfx/seadColor.h>

namespace sead {
class TextWriter;
} // namespace sead

using PrintfFunc = void (sead::TextWriter*, const char*, ...);

extern PrintfFunc* TextWriterPrintf;

namespace sead {

class Camera;
class DrawContext;
class Viewport;

class TextWriter {
public:
    ~TextWriter() { __vftable->dtor2(this); }

    // note these probably don't all exist, just for utility

    void printf(const char* fmt, auto&&... args) {
        TextWriterPrintf(this, fmt, std::forward<decltype(args)>(args)...);
    }

    void printDropShadow(const char* fmt, auto&&... args) {
        sead::Vector2f offset(1.f, -1.f);
        setColor(0.f, 0.f, 0.f, 1.f);
        auto cursor = getCursor();
        setCursor(cursor + offset);
        printf(fmt, std::forward<decltype(args)>(args)...);
        setColor(1.f, 1.f, 1.f, 1.f);
        setCursor(cursor);
        printf(fmt, std::forward<decltype(args)>(args)...);
    }
    
    void setProjectionAndCamera(Projection* projection, Camera* camera) { mProjection = projection; mCamera = camera; }
    void setViewport(Viewport* viewport) { mViewport = viewport; }

    void setScale(const Vector2f& scale) { mScale = scale; }
    void setScale(float x, float y) { mScale = { x, y }; }
    void setCursor(const Vector2f& cursor) { mCursor = cursor; }
    void setCursor(float x, float y) { mCursor = { x, y }; }

    void setColor(const Color4f& color) { mColor = color; }
    void setColor(float r, float g, float b, float a) { mColor = { r, g, b, a }; }
    void setColor(float r, float g, float b) { mColor = { r, g, b, mColor.a }; }

    void setDrawContext(DrawContext* ctx) { mDrawCtx = ctx; }
    void setFormatBuffer(wchar_t* buffer, int size) { mFormatBuffer = buffer; mFormatBufferLength = size; }

    void setScaleFromFontHeight(float scale) {
        float height = mFont->getHeight();
        mScale.set(scale / height, scale / height);
    }
    void setScaleFromFontSize(const Vector2f& scale) {
        mScale.set(scale.x / mFont->getWidth(), scale.y / mFont->getHeight());
    }

    void setCursorFromTopLeft(const Vector2f& pos) {
        mCursor.set(pos.x - mViewport->getHalfSizeX(), mViewport->getHalfSizeY() - pos.y);
    }

    void getCursorFromTopLeft(Vector2f* pos) const {
        *pos = Vector2f(mCursor.x - mViewport->getHalfSizeX(), mViewport->getHalfSizeY() - mCursor.y);
    }

    const Color4f& getColor() const { return mColor; }
    const Vector2f& getCursor() const { return mCursor; }
    const Vector2f& getScale() const { return mScale; }

    Color4f& getColor() { return mColor; }
    Vector2f& getCursor() { return mCursor; }
    Vector2f& getScale() { return mScale; }

    float getHeight() const { return mViewport->getSizeY(); }
    float getWidth() const { return mViewport->getSizeX(); }

    void scaleBy(float scale_factor) { mScale *= scale_factor; }

private:
    struct vtable {
        void (*dtor0)(TextWriter* tw);
        void (*dtor2)(TextWriter* tw);
    };

    static vtable* sVtable;

    vtable* __vftable;
    Viewport* mViewport;
    Projection* mProjection;
    Camera* mCamera;
    FontBase* mFont;
    Vector2f mCursor;
    Vector2f mScale{ 1.f, 1.f };
    Color4f mColor{ 1.f, 1.f, 1.f, 1.f };
    BoundBox2f mBounds;
    float _58;
    float _5C;
    wchar_t* mFormatBuffer;
    int mFormatBufferLength;
    bool mDoFontBeginEnd;
    DrawContext* mDrawCtx;
};
static_assert(sizeof(TextWriter) == 0x78);

} // namespace sead