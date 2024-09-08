#pragma once

#include <mc/seadJob.h>

namespace agl::lyr {

class RenderDisplay;

class LayerJob : public sead::Job {
public:
    ~LayerJob() override;

private:
    u32 _10;
    RenderDisplay* mDisplay;
    char _28[8];
};

} // namespace agl::lyr