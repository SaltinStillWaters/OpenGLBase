#include "headers.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(200, 50);
    glutInitWindowSize(1200, 600);
    glutCreateWindow("Window");


    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing glew";
    }

    initializeFlagVBO();
    initializeWaveVBO();
    initializeBoatVBO();
    initializePoleVBOs();

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutMouseFunc(mouseClick);


    glutMainLoop();

    return 0;
}

void update(int value) {
    if (g_yShift > 0.05f || g_yShift < -0.02) {
        g_yShiftDirection *= -1.0;
    }

    if (g_amplitude > 0.015f || g_amplitude < 0.0f) {
        g_amplitudeDirection *= -1.0;
    }

    g_xShift += 0.05 * g_direction;
    g_yShift += 0.0005 * g_yShiftDirection;
    g_amplitude += 0.0001 * g_amplitudeDirection;
    g_boatY += 0.001 * g_yShiftDirection;

    updateFlagVBO();
    updateWaveVBO(0.1, 2.0f, 1.0f, -.1f);
    updateBoatVertices();
    updatePoleVertices();

    glutPostRedisplay();
    glutTimerFunc(16 + g_extraSpeed, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (g_keys['a']) {
        g_direction = 1.0f;
        g_extraSpeed = -12;
        
        if (g_boatX > -.48f) {
            g_boatX -= 0.005;
        }
    } else if (g_keys['d']) {
        g_direction = -1.0f;
        g_extraSpeed = -12;
        
        if (g_boatX < .5f) {
            g_boatX += 0.005;
        }
    } else {
        g_extraSpeed = 0;
    }

    drawWave();
    drawFlag();
    drawPole();
    drawBoatBody();

    glutSwapBuffers();
}

//EVENT HANDLERS
void keyDown(unsigned char key, int x, int y) {
    g_keys[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
    g_keys[key] = false;
}

void mouseClick(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        g_flagColor = (rand() % 101) / 100.0f * 255.0f;
        glutPostRedisplay();
    }
}

//WAVE
void initializeWaveVBO() {
    glGenBuffers(1, &waveVBO);
    glBindBuffer(GL_ARRAY_BUFFER, waveVBO);
    glBufferData(GL_ARRAY_BUFFER, 9999 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void updateWaveVBO(float amplitude, float frequency, float xShift, float yShift) {
    amplitude += g_amplitude;
    xShift += g_xShift;
    yShift += g_yShift;

    float minX = -1;
    float maxX = 1;

    GLfloat vertices[9999];
    int ctr = 0;
    for (float x = minX; x <= maxX; x += 0.005f) {
        float y = amplitude * sin(x * 2.0f * PI * frequency + xShift) + yShift;
        vertices[ctr++] = x;
        vertices[ctr++] = y;
        vertices[ctr++] = x;
        vertices[ctr++] = -1.0f;
    }
    waveVertexCount = ctr / 2;

    glBindBuffer(GL_ARRAY_BUFFER, waveVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Update buffer data
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawWave() {

    glBindBuffer(GL_ARRAY_BUFFER, waveVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3ub(26, 93, 237);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, waveVertexCount);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//FLAG
void initializeFlagVBO() {
    glGenBuffers(1, &flagVBO);
    glBindBuffer(GL_ARRAY_BUFFER, flagVBO);
    glBufferData(GL_ARRAY_BUFFER, 9999 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void updateFlagVBO() {
    float xLeft = -0.3;
    float xRight = 0.3;
    
    float height = 0.5f;
    float frequency = 5.0f;
    float amplitude = 0.02f;
    float xShift = g_xShift * 2;
    float yShift = 0.4f;

    GLfloat vertices[9999];
    int ctr = 0;

    for (float x = xLeft; x < xRight; x += 0.001f) {
        float y = amplitude * sin(x * 2.0f * PI * frequency + xShift) + yShift;

        vertices[ctr++] = x + g_boatX;
        vertices[ctr++] = y + g_boatY;
        vertices[ctr++] = x + g_boatX;
        vertices[ctr++] = y + height + g_boatY;
    }
    flagVertexCount = ctr / 2;

    glBindBuffer(GL_ARRAY_BUFFER, flagVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ctr * sizeof(GLfloat), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawFlag() {
    glBindBuffer(GL_ARRAY_BUFFER, flagVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3ub(g_flagColor, g_flagColor, g_flagColor);
    glDrawArrays(GL_LINES, 0, flagVertexCount);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//BOAT
void initializeBoatVBO() {
    glGenBuffers(1, &boatVBO);
}

void updateBoatVertices() {
    GLfloat boatVertices[76];
    int boatCtr = 0;

    for (int i = 0; i <= 180; i += 10) {
        float theta = i * PI / 180.0f;
        float x = 0.6f * cos(theta);
        float y = -0.3f * sin(theta);

        float slopeAdjustment = 0.002f * i;
        boatVertices[boatCtr++] = x + g_boatX;
        boatVertices[boatCtr++] = y - 0.1f + slopeAdjustment + g_boatY;
    }

    for (int i = 180; i >= 0; i -= 10) {  
        float theta = i * PI / 180.0f;
        float x = 0.6f * cos(theta);
        float y = -0.25f * sin(theta);     

        float slopeAdjustment = 0.002f * i;
        boatVertices[boatCtr++] = x + g_boatX;
        boatVertices[boatCtr++] = y + 0.05f + slopeAdjustment + g_boatY;
    }

    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boatVertices), boatVertices, GL_DYNAMIC_DRAW);
}

void drawBoatBody() {
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glColor3ub(102, 59, 17);
    glDrawArrays(GL_POLYGON, 0, 38);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//POLE
void initializePoleVBOs() {
    glGenBuffers(1, &poleBaseVBO);
    glGenBuffers(1, &poleCircleVBO);
}

void updatePoleVertices() {
    // Global settings
    float xRelativeToFlag = 0.315f;
    float yRelativeToFlag = -0.05f;

    GLfloat poleBaseVertices[] = {
        // Pole
        -0.015f + xRelativeToFlag + g_boatX, 0.0f + yRelativeToFlag + g_boatY,
         0.015f + xRelativeToFlag + g_boatX, 0.0f + yRelativeToFlag + g_boatY,
         0.015f + xRelativeToFlag + g_boatX, 1.0f + yRelativeToFlag + g_boatY,
        -0.015f + xRelativeToFlag + g_boatX, 1.0f + yRelativeToFlag + g_boatY,

        // Base
        -0.04f + xRelativeToFlag + g_boatX, -0.05f + yRelativeToFlag + g_boatY,
         0.04f + xRelativeToFlag + g_boatX, -0.05f + yRelativeToFlag + g_boatY,
         0.04f + xRelativeToFlag + g_boatX, 0.0f + yRelativeToFlag + g_boatY,
        -0.04f + xRelativeToFlag + g_boatX, 0.0f + yRelativeToFlag + g_boatY,
    };

    glBindBuffer(GL_ARRAY_BUFFER, poleBaseVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(poleBaseVertices), poleBaseVertices, GL_DYNAMIC_DRAW);

    // Circle
    float radius = 0.04f;
    int numSegments = 20;
    GLfloat circleVertices[44]; 
    int ctr = 0;

    circleVertices[ctr++] = 0.0f + xRelativeToFlag + g_boatX;
    circleVertices[ctr++] = 1.05f + yRelativeToFlag + g_boatY;

    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * PI * i / numSegments;
        float x = radius * cos(angle);
        float y = radius * sin(angle) + 1.0f;

        circleVertices[ctr++] = x + xRelativeToFlag + g_boatX;
        circleVertices[ctr++] = y + yRelativeToFlag + g_boatY;
    }

    glBindBuffer(GL_ARRAY_BUFFER, poleCircleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawPole() {
    glColor3ub(192, 192, 192);
    glBindBuffer(GL_ARRAY_BUFFER, poleBaseVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glDrawArrays(GL_QUADS, 0, 8);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glBindBuffer(GL_ARRAY_BUFFER, poleCircleVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}