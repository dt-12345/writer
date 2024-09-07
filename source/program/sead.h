#pragma once

namespace sead {

struct ListNode {
    ListNode* mPrev;
    ListNode* mNext;
};

class Heap {
public:
    virtual ~Heap();
    virtual void checkDerivedRuntimeTypeInfo();
    virtual void getRuntimeTypeInfo();
    virtual void destroy();
    virtual size_t adjust();
    virtual void *tryAlloc(size_t size, s32 alignment);
    virtual void free(void *address);
    virtual size_t freeAndGetAllocatableSize(void *address, u32 alignment);
    virtual size_t resizeFront(void *address, size_t size);
    virtual size_t resizeBack(void *address, size_t size);
    virtual size_t tryRealloc(void *address, size_t new_size, s32 alignment);
    virtual size_t freeAll();
    virtual void *getStartAddress();
    virtual void *getEndAddress();
    virtual size_t getSize();
    virtual size_t getFreeSize();
    virtual size_t getMaxAllocatableSize(int alignment);
    virtual bool isInclude(void *address);
    virtual bool isEmpty();
    virtual bool isFreeable();
    virtual bool isResizable();
    virtual bool isAdjustable();
    virtual void dump();
    virtual void dumpYaml();
    virtual void genInformation_();
    virtual s32 makeMetaString_();
    virtual void pushBackChild_(Heap *child);

    Heap* mDisposerHeap;
    ListNode mListNode;
    char* mName;
    // actually too lazy to fill this all out
};

struct Vector2f {
    float x;
    float y;
};

struct BoundBox2f {
    Vector2f min;
    Vector2f max;
};

struct Color4f {
    float r;
    float g;
    float b;
    float a;
};

class Viewport {
public:
    virtual ~Viewport();

    BoundBox2f mBounds;
    u32 mPosture;
    Vector2f _1C;
};

class DrawContext {
public:
    virtual ~DrawContext();

private:
    char _08[0xe0]; // lazy lol
};

class TextWriter {
public:
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
    Vector2f mScale;
    Color4f mColor;
    BoundBox2f _48;
    float _58;
    float mLineSpace;
    wchar_t* mFormatBuffer;
    int mFormatBufferLength;
    bool mDoFontBeginEnd;
    DrawContext* mDrawCtx;
};
static_assert(sizeof(TextWriter) == 0x78);

} // namespace sead