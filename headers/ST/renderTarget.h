#pragma once

typedef struct stRenderTarget {
    unsigned char* pixels;
    unsigned int width;
    unsigned int height;
} stRenderTarget;

stRenderTarget* stRenderTargetCreate();

void stRenderTargetDestroy(stRenderTarget* renderTarget);

void stRenderTargetUpdate(stRenderTarget* renderTarget, unsigned int width, unsigned int height);

void stRenderTargetSetPixel(stRenderTarget* renderTarget, unsigned int x, unsigned int y, float color[4]);
