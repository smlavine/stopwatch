#include <GLFW/glfw3.h>

static const int WIN_W = 16, WIN_H = 16;

static void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_RELEASE) {
            glfwSetWindowShouldClose(win, 1);
        }
    }
}

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_DECORATED, 0);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    GLFWwindow *win = glfwCreateWindow(WIN_W, WIN_H, "Stopwatch", NULL, NULL);
    glfwSetMouseButtonCallback(win, mouseButtonCallback);
    while (!glfwWindowShouldClose(win)) {
        glfwWaitEventsTimeout(1);
        glfwSwapBuffers(win);
    }
    glfwDestroyWindow(win);
    glfwTerminate();
}
