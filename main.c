#include <GLFW/glfw3.h>

static const int WIN_W = 16, WIN_H = 16;

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_DECORATED, 0);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    GLFWwindow *win = glfwCreateWindow(WIN_W, WIN_H, "Stopwatch", NULL, NULL);
    while (!glfwWindowShouldClose(win)) {
        glfwWaitEventsTimeout(1);
        glfwSwapBuffers(win);
    }
    glfwDestroyWindow(win);
    glfwTerminate();
}
