#include <ST/ST.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "exampleGlobalValues.h"
#include "exampleFunctions.h"

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "Ray-tracer"

int main(int argc, char** argv) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    GladGLContext* context = malloc(sizeof(GladGLContext));
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLContext(context, (GLADloadfunc)glfwGetProcAddress);

    unsigned int framebuffer, renderbuffer;
    context->GenFramebuffers(1, &framebuffer);
    context->GenRenderbuffers(1, &renderbuffer);

    context->BindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    context->RenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, WIDTH, HEIGHT);  // Using GL_RGBA32F for HDR compute shader output
    context->BindRenderbuffer(GL_RENDERBUFFER, 0);

    context->BindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    context->FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);

    if (context->CheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is not complete!\n");
        return -1;
    }
    context->BindFramebuffer(GL_FRAMEBUFFER, 0);

    while(!glfwWindowShouldClose(window)) {
        context->BindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        context->Clear(GL_COLOR_BUFFER_BIT);

        // ...

        context->BindFramebuffer(GL_FRAMEBUFFER, 0);
        context->Clear(GL_COLOR_BUFFER_BIT);

        context->Begin(GL_QUADS);
        context->Vertex2f(-1.0f, -1.0f);
        context->Vertex2f(1.0f, -1.0f);
        context->Vertex2f(1.0f, 1.0f);
        context->Vertex2f(-1.0f, 1.0f);
        context->End();

        glfwSwapBuffers(window);
        glfwPollEvents();
        proccessInput(window);
    }

    context->DeleteRenderbuffers(1, &renderbuffer);
    context->DeleteFramebuffers(1, &framebuffer);
    glfwDestroyWindow(window);
    window = NULL;
    free(context);
    context = NULL;

    glfwTerminate();
}
