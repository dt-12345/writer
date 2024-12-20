// nvm we're not using this but I'll leave it here as a reference

#pragma once

#include <nvn_Cpp.h>

#include <gfx/seadPrimitiveRenderer.h>
#include <heap/seadDisposer.h>

namespace sead {

class PrimitiveDrawMgrNvn : public PrimitiveRendererBase {
public:
    void prepareFromBinaryImpl(Heap* heap, const void* bin_data, u32 bin_size) override;
    void prepareImpl(Heap* heap, const SafeString& path) override;
    void m3() override;
    void setCameraImpl(const Camera& camera) override;
    void setProjectionImpl(const Projection& projection) override;
    void beginImpl(DrawContext*, const Matrix34f&, const Matrix44f&) override;
    void endImpl(DrawContext*) override;
    void drawQuadImpl(const Matrix34f& model_mtx, const Color4f& colorL,
                              const Color4f& colorR) override;
    void drawQuadImpl(const Matrix34f& model_mtx, const Texture& texture,
                              const Color4f& colorL, const Color4f& colorR, const Vector2f& uv_src,
                              const Vector2f& uv_size) override;
    void drawBoxImpl(const Matrix34f& model_mtx, const Color4f& colorL,
                             const Color4f& colorR) override;
    void drawCubeImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1) override;
    void drawWireCubeImpl(const Matrix34f& model_mtx, const Color4f& c0,
                                  const Color4f& c1) override;
    void drawLineImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1) override;
    void drawSphere4x8Impl(const Matrix34f& model_mtx, const Color4f& north,
                                   const Color4f& south) override;
    void drawSphere8x16Impl(const Matrix34f& model_mtx, const Color4f& north,
                                    const Color4f& south) override;
    void drawDisk16Impl(const Matrix34f& model_mtx, const Color4f& center,
                                const Color4f& edge) override;
    void drawDisk32Impl(const Matrix34f& model_mtx, const Color4f& center,
                                const Color4f& edge) override;
    void drawCircle16Impl(const Matrix34f& model_mtx, const Color4f& edge) override;
    void drawCircle32Impl(const Matrix34f& model_mtx, const Color4f& edge) override;
    void drawCylinder16Impl(const Matrix34f& model_mtx, const Color4f& top,
                                    const Color4f& btm) override;
    void drawCylinder32Impl(const Matrix34f& model_mtx, const Color4f& top,
                                    const Color4f& btm) override;

    virtual ~PrimitiveDrawMgrNvn();

private:
    nvn::Program mProgram;
    nvn::Buffer mBuffer;
    nvn::MemoryPool mMemoryPool;
    nvn::VertexAttribState mVertexAttribState0;
    nvn::VertexAttribState mVertexAttribState1;
    nvn::VertexStreamState mVertexStreamState;
    nvn::MemoryPool mQuadMemoryPool;
    nvn::Buffer mQuadVertexBuffer;
    nvn::Buffer mQuadIndexBuffer;
    nvn::Buffer mPrimitiveMemoryPool;
    nvn::Buffer mLineVertexBuffer;
    nvn::Buffer mLineIndexBuffer;
    nvn::Buffer mCubeVertexBuffer;
    nvn::Buffer mCubeIndexBuffer;
    nvn::Buffer mWireCubeVertexBuffer;
    nvn::Buffer mWireCubeIndexBuffer;
    nvn::Buffer mSphere4x8VertexBuffer;
    nvn::Buffer mSphere4x8IndexBuffer;
    nvn::Buffer mSphere8x16VertexBuffer;
    nvn::Buffer mSphere8x16IndexBuffer;
    nvn::Buffer mDisk16VertexBuffer;
    nvn::Buffer mDisk16IndexBuffer;
    nvn::Buffer mDisk32VertexBuffer;
    nvn::Buffer mDisk32IndexBuffer;
    nvn::Buffer _600;
    nvn::Buffer _630;
    nvn::Buffer mCylinder16VertexBuffer;
    nvn::Buffer mCylinder16IndexBuffer;
    nvn::Buffer mCylinder32VertexBuffer;
    nvn::Buffer mCylinder32IndexBuffer;
    nvn::Buffer mUniformBuffer;
    void* mpUBOMemory;
    u8 _758;
    int _75C;
    int mMemoryPoolSize;
    int _764;
    int _768;
    bool _76C;

    SEAD_SINGLETON_DISPOSER(PrimitiveDrawMgrNvn)
};
static_assert(sizeof(PrimitiveDrawMgrNvn) == 0x790);

} // namespace sead