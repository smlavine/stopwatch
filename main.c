#include <GLFW/glfw3.h>

#include <stdio.h>

static const int WIN_W = 16, WIN_H = 16;
static const double DOUBLE_CLICK_DURATION = 0.2;

static int dragged = 0, paused = 0;
static double grabX, grabY, lastClick = 0;
static double timer = 0; // When running, shows start time, when paused, shows accumulated time

static void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glfwGetCursorPos(win, &grabX, &grabY);
            dragged = 0;
        } else if (action == GLFW_RELEASE) {
            if (!dragged) {
                paused = !paused;
                if (paused) {
                    timer = glfwGetTime() - timer;
                    glfwSetWindowTitle(win, "[Paused]");
                } else {
                    timer = glfwGetTime() - timer;
                    glfwSetWindowTitle(win, "[Unpaused]");
                }
                double currTime = glfwGetTime();
                if (currTime - lastClick < DOUBLE_CLICK_DURATION) {
                    paused = 0;
                    timer = glfwGetTime();
                    glfwSetWindowTitle(win, "[Restarted]");
                }
                lastClick = currTime;
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
        printf("%f\n", paused ? timer : glfwGetTime() - timer);
        glfwSwapBuffers(win);
    }
    glfwDestroyWindow(win);
    glfwTerminate();
}
