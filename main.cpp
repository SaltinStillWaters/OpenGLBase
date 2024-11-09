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

std::vector<float> oceanVector;
unsigned int oceanVBO, oceanVAO;

std::vector<float> cloud1Vector;
std::vector<float> cloud2Vector;
std::vector<float> cloud3Vector;

GLfloat* mappedCloud1;
GLfloat* mappedCloud2;
GLfloat* mappedCloud3;

unsigned int cloud1VBO, cloud1VAO;
unsigned int cloud2VBO, cloud2VAO;
unsigned int cloud3VBO, cloud3VAO;

float xSpd = .01f;
bool isKeyPressed[256];

void keyDown(unsigned char key, int x, int y) {
    isKeyPressed[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
    isKeyPressed[key] = false;
}

int main(int argcp, char** argv) {
    glutInit(&argcp, argv);
    glutCreateWindow("Window");

    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing glew\n";
    }

    fileToVector(vector, "vertices.txt");
    fileToVector(cloud1Vector, "cloud1.txt");
    fileToVector(cloud2Vector, "cloud2.txt");
    fileToVector(cloud3Vector, "cloud3.txt");
    fileToVector(oceanVector, "ocean.txt");

    initBuffers(vector);
    
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

// Function implementations
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindVertexArray(oceanVAO);
    glBindBuffer(GL_ARRAY_BUFFER, oceanVBO);
    glDrawArrays(GL_QUADS, 0, oceanVector.size() / 6);

    glBindVertexArray(boatVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    glDrawArrays(GL_QUADS, 0, vector.size() / 6);

    glBindVertexArray(cloud1VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cloud1VBO);
    glDrawArrays(GL_QUADS, 0, cloud1Vector.size() / 6);

    glBindVertexArray(cloud2VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cloud2VBO);
    glDrawArrays(GL_QUADS, 0, cloud2Vector.size() / 6);

    glBindVertexArray(cloud3VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cloud3VBO);
    glDrawArrays(GL_QUADS, 0, cloud3Vector.size() / 6);
   
    glutSwapBuffers();
}

void initBuffers(std::vector<float>& vector) {
    //Boat
    glGenBuffers(1, &boatVBO);
    glGenVertexArrays(1, &boatVAO);

    glBindVertexArray(boatVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    glBufferData(GL_ARRAY_BUFFER, vector.size() * sizeof(float), vector.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)(2 * sizeof(GLfloat)));

    // Setup for cloud1Vector
    glGenBuffers(1, &cloud1VBO);
    glGenVertexArrays(1, &cloud1VAO);

    glBindVertexArray(cloud1VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cloud1VBO);
    glBufferData(GL_ARRAY_BUFFER, cloud1Vector.size() * sizeof(float), cloud1Vector.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)(2 * sizeof(GLfloat)));
    
    // Setup for cloud2Vector
    glGenBuffers(1, &cloud2VBO);
    glGenVertexArrays(1, &cloud2VAO);

    glBindVertexArray(cloud2VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cloud2VBO);
    glBufferData(GL_ARRAY_BUFFER, cloud2Vector.size() * sizeof(float), cloud2Vector.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)(2 * sizeof(GLfloat)));

    // Setup for cloud3Vector
    glGenBuffers(1, &cloud3VBO);
    glGenVertexArrays(1, &cloud3VAO);

    glBindVertexArray(cloud3VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cloud3VBO);
    glBufferData(GL_ARRAY_BUFFER, cloud3Vector.size() * sizeof(float), cloud3Vector.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)(2 * sizeof(GLfloat)));


    glGenBuffers(1, &oceanVBO);
    glGenVertexArrays(1, &oceanVAO);

    glBindVertexArray(oceanVAO);
    glBindBuffer(GL_ARRAY_BUFFER, oceanVBO);
    glBufferData(GL_ARRAY_BUFFER, oceanVector.size() * sizeof(float), oceanVector.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)(2 * sizeof(GLfloat)));
    

    //Shader
    ShaderProgramSource source = ParseShader("shader.shader");
    unsigned int shader = createShader(source.VertexShader, source.FragmentShader);
    glUseProgram(shader);
}

    void update(int time) {
        float xOffset = (isKeyPressed['d'] - isKeyPressed['a']) * xSpd;

        //Boat Movement
        glBindVertexArray(boatVAO);
        glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
        mappedBoat = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

        for (int x = 0; x < vector.size(); x += 6) {
            mappedBoat[x] += xOffset;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);

        glBindVertexArray(cloud1VAO);
        glBindBuffer(GL_ARRAY_BUFFER, cloud1VBO);
        mappedCloud1 = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

        for (int x = 0; x < cloud1Vector.size(); x += 6) {
            mappedCloud1[x] += -.002;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);

        glBindVertexArray(cloud2VAO);
        glBindBuffer(GL_ARRAY_BUFFER, cloud2VBO);
        mappedCloud2 = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

        for (int x = 0; x < cloud2Vector.size(); x += 6) {
            mappedCloud2[x] += -.001;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);

        glBindVertexArray(cloud3VAO);
        glBindBuffer(GL_ARRAY_BUFFER, cloud3VBO);
        mappedCloud3 = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

        for (int x = 0; x < cloud3Vector.size(); x += 6) {
            mappedCloud3[x] += -.001;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);


        glBindBuffer(GL_ARRAY_BUFFER, 0);

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