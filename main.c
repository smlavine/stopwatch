#include <GLFW/glfw3.h>

static const int WIN_W = 16, WIN_H = 16;

static int dragged = 0, paused = 0;
static double grabX, grabY;

static void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glfwGetCursorPos(win, &grabX, &grabY);
            dragged = 0;
        } else if (action == GLFW_RELEASE) {
            if (!dragged) {
                paused = !paused;
                if (paused) {
                    glfwSetWindowTitle(win, "[Paused]");
                } else {
                    glfwSetWindowTitle(win, "Stopwatch");
                }
            }
        }
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_RELEASE) {
            glfwSetWindowShouldClose(win, 1);
        }
    }
}

static void cursorPosCallback(GLFWwindow *win, double x, double y) {
    if (!glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT)) return;
    dragged = 1;
    int wx, wy;
    glfwGetWindowPos(win, &wx, &wy);
    glfwSetWindowPos(win, wx + x - grabX, wy + y - grabY);
}

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_DECORATED, 0);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    GLFWwindow *win = glfwCreateWindow(WIN_W, WIN_H, "Stopwatch", NULL, NULL);
    glfwGetCursorPos(win, &grabX, &grabY);
    glfwSetMouseButtonCallback(win, mouseButtonCallback);
    glfwSetCursorPosCallback(win, cursorPosCallback);
    while (!glfwWindowShouldClose(win)) {
        glfwWaitEventsTimeout(1);
        glfwSwapBuffers(win);
    }
    glfwDestroyWindow(win);
    glfwTerminate();
}
