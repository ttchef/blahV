
#ifndef BLAHV_CONTEXT_H
#define BLAHV_CONTEXT_H

#include "blahV_log.h"
#include "blahV_window.h"

#include <GLFW/glfw3.h>

struct blvContext {
    blvWindow window;
};

void blvDeinit(blvContext* context);

#endif
