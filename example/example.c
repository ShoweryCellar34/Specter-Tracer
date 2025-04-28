#include <ST/ST.h>

int main(int argc, char** argv) {
    stRenderTarget* renderTarget = stRenderTargetCreate();

    stRenderTargetUpdate(renderTarget, 4, 4);
    stRenderTargetSetPixel(renderTarget, 1, 0, (float[4]){0.5f, 0.1f, 1.0f, 0.95f});

    for(unsigned int i = 0; i < renderTarget->width * 4; i++) {
        for()
    }

    stRenderTargetDestroy(renderTarget);
}
