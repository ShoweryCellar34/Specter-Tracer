#include <ST/ST.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "exampleGlobalValues.h"
#include "exampleFunctions.h"

int main(int argc, char** argv) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, TITLE, NULL, NULL);
    globalData.context = malloc(sizeof(GladGLContext));
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLContext(globalData.context, (GLADloadfunc)glfwGetProcAddress);

    globalData.context->GenFramebuffers(1, &globalData.framebuffer);
    globalData.context->GenTextures(1, &globalData.outputTexture);

    // Setup texture for compute shader output
    globalData.context->BindTexture(GL_TEXTURE_2D, globalData.outputTexture);
    globalData.context->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    globalData.context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    globalData.context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    globalData.context->BindTexture(GL_TEXTURE_2D, 0);

    // Attach texture to framebuffer
    globalData.context->BindFramebuffer(GL_FRAMEBUFFER, globalData.framebuffer);
    globalData.context->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, globalData.outputTexture, 0);

    if(globalData.context->CheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is not complete!\n");
        return -1;
    }
    globalData.context->BindFramebuffer(GL_FRAMEBUFFER, 0);

    globalData.computeShader = globalData.context->CreateShader(GL_COMPUTE_SHADER);
    globalData.context->ShaderSource(globalData.computeShader, 1, &computeShaderSource, NULL);
    globalData.context->CompileShader(globalData.computeShader);

    int success;
    char infoLog[512];
    globalData.context->GetShaderiv(globalData.computeShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        globalData.context->GetShaderInfoLog(globalData.computeShader, 512, NULL, infoLog);
        printf("ERROR::COMPUTE_SHADER::COMPILATION_FAILED\n%s\n", infoLog);
        return -1;
    }

    globalData.computeProgram = globalData.context->CreateProgram();
    globalData.context->AttachShader(globalData.computeProgram, globalData.computeShader);
    globalData.context->LinkProgram(globalData.computeProgram);

    globalData.context->GetProgramiv(globalData.computeProgram, GL_LINK_STATUS, &success);
    if(!success) {
        globalData.context->GetProgramInfoLog(globalData.computeProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER_PROGRAM::LINKING_FAILED\n%s\n", infoLog);
        return -1;
    }
    globalData.context->DeleteShader(globalData.computeShader);

    globalData.context->UseProgram(globalData.computeProgram);
    globalData.context->BindImageTexture(0, globalData.outputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    while(!glfwWindowShouldClose(window)) {
        // Run compute shader
        globalData.context->UseProgram(globalData.computeProgram);
        globalData.context->DispatchCompute(windowWidth, windowHeight, 1);
        globalData.context->MemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // Copy to screen
        globalData.context->BindFramebuffer(GL_READ_FRAMEBUFFER, globalData.framebuffer);
        globalData.context->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        globalData.context->BlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glfwSwapBuffers(window);
        glfwPollEvents();
        proccessInput(window);
    }

    globalData.context->DeleteProgram(globalData.computeProgram);
    globalData.context->DeleteTextures(1, &globalData.outputTexture);
    globalData.context->DeleteFramebuffers(1, &globalData.framebuffer);
    glfwDestroyWindow(window);
    window = NULL;
    free(globalData.context);
    globalData.context = NULL;

    glfwTerminate();
}
