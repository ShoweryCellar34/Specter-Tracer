#pragma once

void proccessInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
}
