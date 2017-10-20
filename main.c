#include <GLFW/glfw3.h>

#include <string.h>
#include <stdio.h>

static const int WIN_W = 16, WIN_H = 16;
static const double DOUBLE_CLICK_DURATION = 0.2;

static int dragged = 0, paused = 0;
static double grabX, grabY, lastClick = 0, timer = 0;

static void setTitle(GLFWwindow *win) {
    static int x = 0;
    static char prev[18] = {'\0'}, next[18] = {'\0'};
    char *buffer = x ? prev : next;
    x = !x;

    long s = paused ? timer : glfwGetTime() - timer;
    long h = s / 3600;
    s %= 3600;
    long m = s / 60;
    s %= 60;
    snprintf(buffer, 18, paused ? "[Paused] %02ld:%02ld:%02ld" : "%02ld:%02ld:%02ld", h, m, s);
    if (strcmp(prev, next) != 0) glfwSetWindowTitle(win, buffer);
}

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
                } else {
                    timer = glfwGetTime() - timer;
                }
                double currTime = glfwGetTime();
                if (currTime - lastClick < DOUBLE_CLICK_DURATION) {
                    paused = 0;
                    timer = glfwGetTime();
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
        setTitle(win);
        glfwSwapBuffers(win);
    }
    glfwDestroyWindow(win);
    glfwTerminate();
}
