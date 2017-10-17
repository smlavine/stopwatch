#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#define MIN_WIN_W 27
#define MIN_WIN_H 5
#define SCL 2
#define WIN_W (MIN_WIN_W * SCL)
#define WIN_H (MIN_WIN_H * SCL)
#define VC(x) (sizeof(x) / sizeof(float[2])) // vertex count

static struct {
    GLFWwindow *win;
    GLuint prg;
    GLint pos, scl, mov, scl2, mov2;
    struct {
        double x, y;
    } anchor;
} g;

static void initGLFW(void);
static void exitGLFW(void);
static void initGL(void);
static void exitGL(void);
static void draw(void);
static void getDigits(double t, int *H, int *h, int *M, int *m, int *S, int *s);
static void drawObj(float mov, const float *v, size_t vc);
static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
static void adjustWindowPosition(void);

int main(void) {
    initGLFW();
    initGL();
    while (!glfwWindowShouldClose(g.win)) {
        glfwWaitEvents();
        draw();
        adjustWindowPosition();
        glfwSwapBuffers(g.win);
    }
    exitGL();
    exitGLFW();
}

static void initGLFW(void) {
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_DECORATED, 0);
    g.win = glfwCreateWindow(WIN_W, WIN_H, "Stopwatch", NULL, NULL);
    glfwMakeContextCurrent(g.win);
    glfwSwapInterval(1);
    glfwSetMouseButtonCallback(g.win, mouseButtonCallback);
    g.anchor.x = g.anchor.y = 0;
}

static void exitGLFW(void) {
    glfwDestroyWindow(g.win);
    glfwTerminate();
}

static void initGL(void) {
    const char *VERT =
    "#version 100\n"
    "attribute vec2 pos;\n"
    "uniform vec2 scl, mov, scl2, mov2;\n"
    "void main() {\n"
    "    vec2 p = pos * scl + mov;"
    "    p = p * scl2 + mov2;\n"
    "    gl_Position = vec4(p,0,1);\n"
    "}\n";
    const char *FRAG =
    "#version 100\n"
    "precision mediump float;\n"
    "void main() {\n"
    "    gl_FragColor = vec4(0,1,0,1);\n"
    "}\n";
    g.prg = glCreateProgram();
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vert, 1, &VERT, NULL);
    glShaderSource(frag, 1, &FRAG, NULL);
    glCompileShader(vert);
    glCompileShader(frag);
    glAttachShader(g.prg, vert);
    glAttachShader(g.prg, frag);
    glLinkProgram(g.prg);
    glDetachShader(g.prg, frag);
    glDetachShader(g.prg, vert);
    glDeleteShader(frag);
    glDeleteShader(vert);
    glUseProgram(g.prg);
    g.pos = glGetAttribLocation(g.prg, "pos");
    g.scl = glGetUniformLocation(g.prg, "scl");
    g.mov = glGetUniformLocation(g.prg, "mov");
    g.scl2 = glGetUniformLocation(g.prg, "scl2");
    g.mov2 = glGetUniformLocation(g.prg, "mov2");
    glViewport(0, 0, WIN_W, WIN_H);
}

static void exitGL(void) {
    glUseProgram(0);
    glDeleteProgram(g.prg);
}

static void draw(void) {
    static const float ZERO[] = {0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0,
                                 0, 0, 1, 0, 1, 5, 1, 5, 0, 5, 0, 0,
                                 3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5,
                                 3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5};
    static const float ONE[] = {1, 0, 2, 0, 2, 5, 2, 5, 1, 5, 1, 0,
                                0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0,
                                2, 5, 0, 5, 0, 4, 0, 4, 2, 4, 2, 5};
    static const float TWO[] = {0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0,
                                0, 0, 1, 0, 1, 3, 1, 3, 0, 3, 0, 0,
                                3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5,
                                3, 5, 2, 5, 2, 2, 2, 2, 3, 2, 3, 5,
                                1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2};
    static const float THREE[] = {0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0,
                                  3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5,
                                  3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5,
                                  1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2};
    static const float FOUR[] = {0, 2, 1, 2, 1, 5, 1, 5, 0, 5, 0, 2,
                                 0, 2, 3, 2, 3, 3, 3, 3, 0, 3, 0, 2,
                                 3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5};
    static const float FIVE[] = {0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0,
                                 3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5,
                                 0, 2, 1, 2, 1, 5, 1, 5, 0, 5, 0, 2,
                                 2, 0, 3, 0, 3, 3, 3, 3, 2, 3, 2, 0,
                                 1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2};
    static const float SIX[] = {0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0,
                                0, 0, 1, 0, 1, 5, 1, 5, 0, 5, 0, 0,
                                3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5,
                                0, 2, 1, 2, 1, 5, 1, 5, 0, 5, 0, 2,
                                2, 0, 3, 0, 3, 3, 3, 3, 2, 3, 2, 0,
                                1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2};
    static const float SEVEN[] = {3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5,
                                  3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5};
    static const float EIGHT[] = {0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0,
                                  0, 0, 1, 0, 1, 5, 1, 5, 0, 5, 0, 0,
                                  3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5,
                                  3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5,
                                  1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2};
    static const float NINE[] = {0, 0, 3, 0, 3, 1, 3, 1, 0, 1, 0, 0,
                                 0, 2, 1, 2, 1, 5, 1, 5, 0, 5, 0, 2,
                                 3, 5, 0, 5, 0, 4, 0, 4, 3, 4, 3, 5,
                                 3, 5, 2, 5, 2, 0, 2, 0, 3, 0, 3, 5,
                                 1, 2, 2, 2, 2, 3, 2, 3, 1, 3, 1, 2};
    static const float COLON[] = {1, 3, 2, 3, 2, 4, 2, 4, 1, 4, 1, 3,
                                  1, 1, 2, 1, 2, 2, 2, 2, 1, 2, 1, 1};
    static const float *DIGITS[] = {
        ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE
    };
    static const size_t DIGIT_VERTEX_COUNT[] = {
        VC(ZERO), VC(ONE), VC(TWO), VC(THREE), VC(FOUR), VC(FIVE), VC(SIX), VC(SEVEN), VC(EIGHT), VC(NINE)
    };

    int H, h, M, m, S, s;
    getDigits(glfwGetTime(), &H, &h, &M, &m, &S, &s);

    glUniform2f(g.scl2, 2.0 / WIN_W, 2.0 / WIN_H);
    glUniform2f(g.mov2, -1, -1);
    glEnableVertexAttribArray(g.pos);
    glClear(GL_COLOR_BUFFER_BIT);
    drawObj(0, DIGITS[H], DIGIT_VERTEX_COUNT[H]);
    drawObj(4, DIGITS[h], DIGIT_VERTEX_COUNT[h]);
    drawObj(7, COLON, VC(COLON));
    drawObj(10, DIGITS[M], DIGIT_VERTEX_COUNT[M]);
    drawObj(14, DIGITS[m], DIGIT_VERTEX_COUNT[m]);
    drawObj(17, COLON, VC(COLON));
    drawObj(20, DIGITS[S], DIGIT_VERTEX_COUNT[S]);
    drawObj(24, DIGITS[s], DIGIT_VERTEX_COUNT[s]);
    glDisableVertexAttribArray(g.pos);
}

static void getDigits(double t, int *H, int *h, int *M, int *m, int *S, int *s) {
    long ti = t;
    if (ti >= 3600 * 100) ti = 3600 * 100 - 1;
    long hours = ti / 3600;
    ti %= 3600;
    *H = hours / 10;
    *h = hours % 10;
    long minutes = ti / 60;
    ti %= 60;
    *M = minutes / 10;
    *m = minutes % 10;
    long seconds = ti;
    *S = seconds / 10;
    *s = seconds % 10;
}

static void drawObj(float mov, const float *v, size_t vc) {
    glUniform2f(g.scl, SCL, SCL);
    glUniform2f(g.mov, mov * SCL, 0);
    glVertexAttribPointer(g.pos, 2, GL_FLOAT, GL_FALSE, 0, v);
    glDrawArrays(GL_TRIANGLES, 0, vc);
}

static void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(win, 1);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(win, &g.anchor.x, &g.anchor.y);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) adjustWindowPosition;
}

void adjustWindowPosition(void) {
    if (!glfwGetMouseButton(g.win, GLFW_MOUSE_BUTTON_LEFT)) return;
    double x, y;
    int xpos, ypos;
    glfwGetCursorPos(g.win, &x, &y);
    glfwGetWindowPos(g.win, &xpos, &ypos);
    glfwSetWindowPos(g.win, xpos - g.anchor.x + x, ypos - g.anchor.y + y);
}
