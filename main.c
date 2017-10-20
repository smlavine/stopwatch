#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#include <string.h>
#include <stdio.h>

static const int WIN_W = 16, WIN_H = 16, SCL = 2;
static const double DOUBLE_CLICK_DURATION = 0.2;

static int dragged = 0, paused = 0;
static double grabX, grabY, lastClick = 0, timer = 0;
static GLuint prg;
static GLint movLoc;

static void setIcon(GLFWwindow *win) {
    GLFWimage image = {16, 16, (unsigned char[]) {
  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,
  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,  0,  0,  0,255,
    }};
    glfwSetWindowIcon(win, 1, &image);
}

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

static void initGL(void) {
    const char *VERT =
    "#version 100\n"
    "attribute vec2 pos;\n"
    "uniform vec2 mov, size;\n"
    "void main() {\n"
    "    vec2 p = (pos + mov) / size * vec2(2, 2) - vec2(1, 1);\n"
    "    gl_Position = vec4(p, 0, 1);\n"
    "}\n";
    const char *FRAG =
    "#version 100\n"
    "precision mediump float;\n"
    "void main() {\n"
    "    gl_FragColor = vec4(0, 1, 0, 1);\n"
    "}\n";
    prg = glCreateProgram();
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vert, 1, &VERT, NULL);
    glShaderSource(frag, 1, &FRAG, NULL);
    glCompileShader(vert);
    glCompileShader(frag);
    glAttachShader(prg, vert);
    glAttachShader(prg, frag);
    glLinkProgram(prg);
    glDetachShader(prg, frag);
    glDetachShader(prg, vert);
    glDeleteShader(frag);
    glDeleteShader(vert);
    glUseProgram(prg);
    movLoc = glGetAttribLocation(prg, "mov");
    glUniform2f(glGetAttribLocation(prg, "size"), WIN_W, WIN_H);
    glViewport(0, 0, WIN_W * SCL, WIN_H * SCL);
    glClearColor(0, 0, 1, 1);
}

static void exitGL(void) {
    glDeleteProgram(prg);
}

static void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_DECORATED, 0);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    GLFWwindow *win = glfwCreateWindow(WIN_W, WIN_H, "Stopwatch", NULL, NULL);
    glfwMakeContextCurrent(win);
    setIcon(win);
    glfwGetCursorPos(win, &grabX, &grabY);
    glfwSetMouseButtonCallback(win, mouseButtonCallback);
    glfwSetCursorPosCallback(win, cursorPosCallback);
    initGL();
    while (!glfwWindowShouldClose(win)) {
        glfwWaitEventsTimeout(1);
        setTitle(win);
        draw();
        glfwSwapBuffers(win);
    }
    exitGL();
    glfwDestroyWindow(win);
    glfwTerminate();
}
