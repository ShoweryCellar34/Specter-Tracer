#pragma once

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    globalData.context->BindTexture(GL_TEXTURE_2D, globalData.outputTexture);
    globalData.context->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    globalData.context->BindTexture(GL_TEXTURE_2D, 0);
}

void proccessInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
}
