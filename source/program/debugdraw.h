#pragma once

#include <cstring>

#include <lyr/aglLayer.h>

#include "lib.hpp"
#include "nn.hpp"
#include "utils.hpp"

#include "debugdrawmgr.h"
#include "seadTextWriter.h"

void initDebugDrawer();
sead::TextWriter initializeTextWriter(const agl::lyr::RenderInfo&);

static DebugDrawMgr gDrawMgr;