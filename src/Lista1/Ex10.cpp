// Exercicio 10 - Desenho com primitivas em OpenGL Moderna
// Desenho baseado em imagem com casa, sol e gramado

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
})";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 uColor;
void main() {
    FragColor = vec4(uColor, 1.0);
})";

void framebuffer_size_callback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

std::vector<float> generateCircle(float cx, float cy, float r, int segments = 100) {
    std::vector<float> verts = { cx, cy };
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * M_PI * i / segments;
        verts.push_back(cx + r * cos(theta));
        verts.push_back(cy + r * sin(theta));
    }
    return verts;
}

unsigned int createObject(const std::vector<float>& verts) {
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return VAO;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* win = glfwCreateWindow(800, 600, "Desenho Casa", nullptr, nullptr);
    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Compilar shaders
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, nullptr);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fs);

    unsigned int shader = glCreateProgram();
    glAttachShader(shader, vs);
    glAttachShader(shader, fs);
    glLinkProgram(shader);

    glDeleteShader(vs);
    glDeleteShader(fs);

    // ----- FORMAS E CORES ----- //

    // Céu (azul)
    std::vector<float> sky = {
        -1.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.0f
    };

    // Gramado (verde)
    std::vector<float> grass = {
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 0.0f,
         1.0f, 0.0f, -1.0f, 0.0f, -1.0f, -1.0f
    };

    // Casa (roxa)
    std::vector<float> house = {
        -0.3f, -0.3f, 0.3f, -0.3f, 0.3f, 0.3f,
         0.3f, 0.3f, -0.3f, 0.3f, -0.3f, -0.3f
    };

    // Telhado (vermelho)
    std::vector<float> roof = {
        -0.35f, 0.3f, 0.35f, 0.3f, 0.0f, 0.6f
    };

    // Porta (marrom)
    std::vector<float> door = {
        -0.08f, -0.3f, 0.08f, -0.3f, 0.08f, 0.05f,
         0.08f, 0.05f, -0.08f, 0.05f, -0.08f, -0.3f
    };

    // Sol (amarelo)
    std::vector<float> sun = generateCircle(-0.7f, 0.7f, 0.15f);

    // Maçaneta (amarelo claro)
    std::vector<float> knob = generateCircle(0.05f, -0.1f, 0.015f);

    // Criar VAOs
    unsigned int vaoSky = createObject(sky);
    unsigned int vaoGrass = createObject(grass);
    unsigned int vaoHouse = createObject(house);
    unsigned int vaoRoof = createObject(roof);
    unsigned int vaoDoor = createObject(door);
    unsigned int vaoSun = createObject(sun);
    unsigned int vaoKnob = createObject(knob);

    // Main loop
    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);

        // Céu azul
        glUniform3f(glGetUniformLocation(shader, "uColor"), 0.0f, 0.7f, 1.0f);
        glBindVertexArray(vaoSky);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Gramado
        glUniform3f(glGetUniformLocation(shader, "uColor"), 0.7f, 1.0f, 0.1f);
        glBindVertexArray(vaoGrass);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Casa
        glUniform3f(glGetUniformLocation(shader, "uColor"), 0.6f, 0.2f, 0.6f);
        glBindVertexArray(vaoHouse);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Telhado
        glUniform3f(glGetUniformLocation(shader, "uColor"), 1.0f, 0.0f, 0.0f);
        glBindVertexArray(vaoRoof);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Porta
        glUniform3f(glGetUniformLocation(shader, "uColor"), 0.6f, 0.4f, 0.2f);
        glBindVertexArray(vaoDoor);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Sol
        glUniform3f(glGetUniformLocation(shader, "uColor"), 1.0f, 1.0f, 0.0f);
        glBindVertexArray(vaoSun);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sun.size() / 2);

        // Maçaneta
        glUniform3f(glGetUniformLocation(shader, "uColor"), 1.0f, 1.0f, 0.3f);
        glBindVertexArray(vaoKnob);
        glDrawArrays(GL_TRIANGLE_FAN, 0, knob.size() / 2);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
