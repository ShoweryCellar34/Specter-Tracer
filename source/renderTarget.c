#include <ST/renderTarget.h>

#include <stdlib.h>
#include <string.h>

stRenderTarget* stRenderTargetCreate() {
    stRenderTarget* renderTarget;

    renderTarget = calloc(1, sizeof(stRenderTarget));

    return renderTarget;
}

void stRenderTargetDestroy(stRenderTarget* renderTarget) {
    if(renderTarget->pixels) {
        free(renderTarget->pixels);
    }

    free(renderTarget);
}

void stRenderTargetUpdate(stRenderTarget* renderTarget, unsigned int width, unsigned int height) {
    unsigned int size = width * height * 4 * sizeof(float);
    renderTarget->width = width;
    renderTarget->height = height;

    if(renderTarget->pixels) {
        renderTarget->pixels = realloc(renderTarget->pixels, size);
    } else {
        renderTarget->pixels = malloc(size);
    }
}

void stRenderTargetSetPixel(stRenderTarget* renderTarget, unsigned int x, unsigned int y, float color[4]) {
    memcpy(renderTarget->pixels + y * renderTarget->height + x * 4 * sizeof(float), color, 4 * sizeof(float));
}
