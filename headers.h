#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <cstdlib>

#define PI 3.14159265358979323846f

void wave(float amplitude, float frequency, float xShift, float yShift);
void display();
void update(int value);
void flag();
void pole();
void boatBody();
void mouseClick(int button, int state, int x, int y);
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void initializeFlagVBO();
void updateFlagVBO();
void drawFlag();
void initializeFlagVBO();
void initializeWaveVBO();
void updateWaveVBO(float amplitude, float frequency, float xShift, float yShift);
void drawWave();
void updateBoatVertices();
void initializeBoatVBO();
void drawBoatBody();
void initializePoleVBOs();
void updatePoleVertices();
void drawPole();

float g_xShift = 0;
float g_yShift = 0;
float g_amplitude = 0;
float g_direction = -1.0f;
float g_yShiftDirection = 1;
float g_amplitudeDirection = 1;
float g_boatX = 0;
float g_boatY = -.3;
int g_extraSpeed = 0;
bool g_keys[256];
int g_flagColor = 222;

GLuint flagVBO;
int flagVertexCount = 0;

GLuint waveVBO;
int waveVertexCount = 0;

GLuint boatVBO;
int boatVertexCount = 0;

GLuint lineVBO;
int lineVertexCount = 0;

GLuint poleBaseVBO;
GLuint poleCircleVBO ;