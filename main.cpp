#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

void display();
void initBuffers(std::vector<float>& vector);
void split(std::string s, char del, std::vector<float>& vector);
void fileToVector(std::vector<float>& vector, const char* filename);
void update(int time);

struct ShaderProgramSource {
    std::string VertexShader;
    std::string FragmentShader;
};

static ShaderProgramSource ParseShader(const std::string& filepath);
static unsigned int CompileShader(GLuint type, const std::string& source);
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);


std::vector<float> vector;
GLfloat* mappedBoat;
unsigned int boatVBO, boatVAO;


int main(int argcp, char** argv) {
    glutInit(&argcp, argv);
    glutCreateWindow("Window");

    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing glew\n";
    }

    fileToVector(vector, "vertices.txt");
    
    initBuffers(vector);
    glutTimerFunc(0, update, 0);
    // glutKeyboardFunc(keyDown);
    // glutKeyboardUpFunc(keyUp);

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

// Function implementations
void display() {
    glBindVertexArray(boatVAO);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_QUADS, 0, vector.size() / 6);
    glutSwapBuffers();
}

void initBuffers(std::vector<float>& vector) {
    glGenBuffers(1, &boatVBO);
    glGenVertexArrays(1, &boatVAO);

    glBindVertexArray(boatVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(float), vector.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)(2 * sizeof(GLfloat)));

    ShaderProgramSource source = ParseShader("shader.shader");
    unsigned int shader = createShader(source.VertexShader, source.FragmentShader);
    glUseProgram(shader);
}

void update(int time) {
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    mappedBoat = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

    for (int x = 0; x < vector.size(); x += 6) {
        mappedBoat[x] += 0.01;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void fileToVector(std::vector<float>& vector, const char* filename) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        split(line, ' ', vector);
    }
}

void split(std::string s, char del, std::vector<float>& vector) {
    std::stringstream ss(s);
    std::string word;
    float val;

    while (getline(ss, word, del)) {
        vector.push_back(std::stof(word));
    }
}

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1,
    };

    ShaderType shaderType = ShaderType::NONE;
    std::stringstream ss[2];
    std::string line;

    while (getline(stream, line)) {
        if (line.find("shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                shaderType = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                shaderType = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)shaderType] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(GLuint type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader\n";
        std::cout << message << '\n';

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}