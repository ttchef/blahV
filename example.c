
#include "blahV/blahV_context.h"
#include "blahV/blahV_window.h"
#include <stdio.h>

#include <blahV/blahV.h>

int main() {
    
    blvContext blv_context = {0};

    if (blvWindowInit(&blv_context, 800, 600, "Test Window") != BLV_OK) {
        fprintf(stderr, "Failed to create a blv window!\n");
        return -1;
    }

    while (!blvWindowShouldQuit(&blv_context)) {

    }

    blvDeinit(&blv_context);

    return 0;
}

