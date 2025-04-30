#pragma once

const char* computeShaderSource = 
    "#version 460 core\n"
    "layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;\n"
    "layout (rgba32f, binding = 0) uniform image2D outputImage;\n"
    "void main() {\n"
    "    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);\n"
    "    vec2 uv = vec2(pixelCoord) / vec2(imageSize(outputImage));\n"
    "    vec3 color = vec3(uv, 0.5);\n"
    "    imageStore(outputImage, pixelCoord, vec4(color, 1.0));\n"
    "}\n";

typedef struct glData {
    GladGLContext* context;
    unsigned int framebuffer, outputTexture;
    unsigned int computeShader, computeProgram;
} glData;

glData globalData = {NULL, 0, 0, 0, 0};

#define DEF_WIDTH 1280
#define DEF_HEIGHT 720
int windowWidth = DEF_WIDTH;
int windowHeight = DEF_HEIGHT;
#define TITLE "Ray-tracer"
