#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#include <string.h>
#include <stdio.h>

typedef struct {
    float w; // width in pixels
    size_t n; // number of points
    float *p; // points ({x,y} tuples)
} Glyph;

static const int WIN_W = 29, WIN_H = 7, SCL = 2;
static const double DOUBLE_CLICK_DURATION = 0.2;
static const Glyph ZERO = {3, 24, (float[]){0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 5, 1, 5, 0, 5, 0, 0, 3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5, 3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5}};
static const Glyph ONE = {3, 18, (float[]){1, 0, 2, 0, 2, 5, 2, 5, 1, 5, 1, 0, 0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0, 2, 5, 0, 5, 0, 4, 0, 4, 2, 4, 2, 5}};
static const Glyph TWO = {3, 30, (float[]){0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 3, 1, 3, 0, 3, 0, 0, 3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5, 3, 5, 2, 5, 2, 2, 2, 2, 3, 2, 3, 5, 1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2}};
static const Glyph THREE = {3, 24, (float[]){0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0, 3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5, 3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5, 1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2}};
static const Glyph FOUR = {3, 18, (float[]){0, 2, 1, 2, 1, 5, 1, 5, 0, 5, 0, 2, 0, 2, 3, 2, 3, 3, 3, 3, 0, 3, 0, 2, 3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5}};
static const Glyph FIVE = {3, 30, (float[]){0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0, 3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5, 0, 2, 1, 2, 1, 5, 1, 5, 0, 5, 0, 2, 2, 0, 3, 0, 3, 3, 3, 3, 2, 3, 2, 0, 1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2}};
static const Glyph SIX = {3, 36, (float[]){0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 5, 1, 5, 0, 5, 0, 0, 3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5, 0, 2, 1, 2, 1, 5, 1, 5, 0, 5, 0, 2, 2, 0, 3, 0, 3, 3, 3, 3, 2, 3, 2, 0, 1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2}};
static const Glyph SEVEN = {3, 12, (float[]){3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5, 3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5}};
static const Glyph EIGHT = {3, 30, (float[]){0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 5, 1, 5, 0, 5, 0, 0, 3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5, 3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5, 1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2}};
static const Glyph NINE = {3, 30, (float[]){0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0, 0, 2, 1, 2, 1, 5, 1, 5, 0, 5, 0, 2, 3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5, 3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5, 1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2}};
static const Glyph COLON = {1, 12, (float[]){0, 1, 1, 1, 1, 2, 1, 2, 0, 2, 0, 1, 0, 3, 1, 3, 1, 4, 1, 4, 0, 4, 0, 3}};
static const Glyph *DIGITS[] = {&ZERO, &ONE, &TWO, &THREE, &FOUR, &FIVE, &SIX, &SEVEN, &EIGHT, &NINE};
static int dragged = 0, paused = 0;
static double grabX, grabY, lastClick = 0, timer = 0;
static GLuint prg;
static GLint posLoc, movLoc;

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
                timer = glfwGetTime() - timer;
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
    dragged = 1;
}

static void moveWindow(GLFWwindow *win) {
    if (!glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT)) return;
    int wx, wy;
    double cx, cy;
    glfwGetCursorPos(win, &cx, &cy);
    glfwGetWindowPos(win, &wx, &wy);
    glfwSetWindowPos(win, wx + cx - grabX, wy + cy - grabY);
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
    posLoc = glGetAttribLocation(prg, "pos");
    movLoc = glGetUniformLocation(prg, "mov");
    glUniform2f(glGetUniformLocation(prg, "size"), WIN_W, WIN_H);
    glViewport(0, 0, WIN_W * SCL, WIN_H * SCL);
    glClearColor(0, 0, 0, 1);
}

static void exitGL(void) {
    glDeleteProgram(prg);
}

static void getDigits(int *H, int *h, int *M, int *m, int *S, int *s) {
    long t = paused ? timer : glfwGetTime() - timer;
    long hours = t / 3600;
    t %= 3600;
    *H = hours / 10;
    *h = hours % 10;
    long minutes = t / 60;
    t %= 60;
    *M = minutes / 10;
    *m = minutes % 10;
    long seconds = t;
    t %= 1;
    *S = seconds / 10;
    *s = seconds % 10;
}

static void drawGlyph(float movX, float movY, Glyph glyph) {
    glUniform2f(movLoc, movX, movY);
    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 0, glyph.p);
    glDrawArrays(GL_TRIANGLES, 0, glyph.n);
}

static void draw(void) {
    int H, h, M, m, S, s;
    getDigits(&H, &h, &M, &m, &S, &s);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableVertexAttribArray(posLoc);
    drawGlyph(1, 1, *DIGITS[H]);
    drawGlyph(5, 1, *DIGITS[h]);
    drawGlyph(9, 1, COLON);
    drawGlyph(11, 1, *DIGITS[M]);
    drawGlyph(15, 1, *DIGITS[m]);
    drawGlyph(19, 1, COLON);
    drawGlyph(21, 1, *DIGITS[S]);
    drawGlyph(25, 1, *DIGITS[s]);
    glDisableVertexAttribArray(posLoc);
}

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_DECORATED, 0);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    GLFWwindow *win = glfwCreateWindow(WIN_W * SCL, WIN_H * SCL, "Stopwatch", NULL, NULL);
    glfwMakeContextCurrent(win);
    setIcon(win);
    glfwGetCursorPos(win, &grabX, &grabY);
    glfwSetMouseButtonCallback(win, mouseButtonCallback);
    glfwSetCursorPosCallback(win, cursorPosCallback);
    initGL();
    while (!glfwWindowShouldClose(win)) {
        glfwWaitEventsTimeout(1);
        moveWindow(win);
        setTitle(win);
        draw();
        glfwSwapBuffers(win);
    }
    exitGL();
    glfwDestroyWindow(win);
    glfwTerminate();
}
