#include <windows.h> // For Windows-specific functions
#include <GL/glut.h> // GLUT, includes glut.h and GL.h
#include <math.h>
#define PI 3.14159265358979323846

bool showIntro = true;
float starX[60], starY[60];
bool starsInitialized = false;
float cloudOffsets[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float cloudSpeeds[6] = {0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f};
bool isRaining = false;
bool isNight = false;
float rainY[500], rainX[500];
float birdAutoX = 0.0f;
float birdControlY = 0.0f;
float birdControlX = 0.0f;
float wingAngle = 0.0f;
bool wingUp = true;
float boatX = 0.0f;
bool boatMoving = false;
int boatDirection = 2;
float oarAngle = 0.0f;
bool oarForward = true;
float waveOffset = 0.0f;
float hyacinthOffset = 0.0f;
float leafAngle = 0.0f;
float clothWave = 0.0f;
bool musicOn = true;
void playCurrentSound()
{
    if (isRaining)
    {
        PlaySound("rain.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    }
    else if (isNight)
    {

         PlaySound("night.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

    }
    else
    {
         PlaySound("nature.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    }

}

void init() {
    glMatrixMode(GL_PROJECTION);      // Switch to projection matrix
    glLoadIdentity();                 // Reset matrix
    gluOrtho2D(0.0, 100.0, 0.0, 80.0); // 2D coordinate system
    for(int i = 0; i < 500; i++) {
    rainX[i] = rand() % 100;
    rainY[i] = rand() % 80;
  }
}
void drawBackground() {
    glBegin(GL_QUADS);
    if (isNight) glColor3f(0.05f, 0.05f, 0.1f);
    else if (isRaining) glColor3f(0.3f, 0.4f, 0.5f);
    else glColor3f(0.3f, 0.6f, 0.9f);
    glVertex2f(0.0f, 80.0f); glVertex2f(100.0f, 80.0f);
    if (isNight) glColor3f(0.1f, 0.1f, 0.2f);
    else if (isRaining) glColor3f(0.5f, 0.6f, 0.7f);
    else glColor3f(0.7f, 0.9f, 1.0f);
    glVertex2f(100.0f, 38.0f); glVertex2f(0.0f, 38.0f);
    glEnd();
    glBegin(GL_QUADS);
    if (isNight) glColor3f(0.1f, 0.2f, 0.1f);
    else glColor3f(0.35f, 0.65f, 0.35f);
    glVertex2f(0.0f, 38.0f);glVertex2f(55.0f, 38.0f);
    glVertex2f(55.0f, 0.0f);glVertex2f(0.0f, 0.0f);
    glEnd();
    glBegin(GL_POLYGON);
    if (isNight) glColor3f(0.15f, 0.25f, 0.15f);
    else glColor3f(0.35f, 0.65f, 0.35f);
    glVertex2f(55.0f, 38.0f);glVertex2f(100.0f, 50.0f);
    glVertex2f(100.0f, 0.0f);glVertex2f(55.0f, 0.0f);
    glEnd();
}
void drawRain() {
    glColor3f(0.8f, 0.8f, 1.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 500; i++) {
        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i] - 0.7f, rainY[i] - 1.5f);
    }
    glEnd();
}

void initStars() {
    for (int i = 0; i < 60; i++) {
        starX[i] = (float)(rand() % 100);
        starY[i] = (float)(55 + (rand() % 30));
    }
    starsInitialized = true;
}

void drawSun(float x, float y, float radius) {
    glPushMatrix();
    glColor3f(1.0f, 0.4f, 0.0f);
    int i;
    int triangleAmount = 100;
    float twicePi = 2.0f * 3.14159265f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for(i = 0; i <= triangleAmount; i++) {
            glVertex2f(
                x + (radius * cos(i * twicePi / triangleAmount)),
                y + (radius * sin(i * twicePi / triangleAmount) * 1.32f)
            );
        }
    glEnd();
    glPopMatrix();
}
void drawMoon(float x, float y, float radius) {
    glColor3f(1.0f, 1.0f, 0.9f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void drawBambooStick(float x, float yBottom, float yTop) {
    glBegin(GL_POLYGON);
    glVertex2f(x, yBottom);
    glVertex2f(x + 0.4f, yBottom);
    glVertex2f(x + 0.4f, yTop);
    glVertex2f(x, yTop);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x, yTop);
    glVertex2f(x + 0.4f, yTop);
    glVertex2f(x + 0.2f, yTop + 0.7f);
    glEnd();
}

void drawCustomFence(float startX, float endX, float stickYBottom, float stickYTop, int railsCount, float railTopY) {
    glColor3f(0.5f, 0.3f, 0.1f);
    // (Rails)
    for(int i = 0; i < railsCount; i++) {
        float yPos = railTopY - (i * 2.0f);
        glBegin(GL_POLYGON);
        glVertex2f(startX, yPos);
        glVertex2f(endX, yPos);
        glVertex2f(endX, yPos - 0.3f);
        glVertex2f(startX, yPos - 0.3f);
        glEnd();
    }

    for (float x = startX + 0.3f; x < endX; x += 1.2f) {
        drawBambooStick(x, stickYBottom, stickYTop);
    }
}

void drawAllBambooFences(float tx, float ty) {
    glPushMatrix();
    glTranslatef(tx, ty, 0);
    drawCustomFence(0.0f, 11.0f, 28.0f, 32.5f, 2, 31.5f);
    drawCustomFence(62.0f, 105.5f, 29.0f, 33.5f, 2, 32.5f);
    drawCustomFence(40.0f, 47.0f, 31.5f, 35.5f, 2, 34.5f);
    glPopMatrix();
}


void drawCoconutTree(float tx, float ty, float scale) {
    glPushMatrix();
    glTranslatef(tx, ty, 0.0f);
    glScalef(scale, scale, 1.0f);
    float t, xt, yt;
    // (Curved Trunk)
    glColor3f(0.4f, 0.25f, 0.1f);
    glLineWidth(4.0f);
    glBegin(GL_LINE_STRIP);
    float tx0=0, ty0=0, tx1=2, ty1=8, tx2=-1, ty2=15, tx3=1, ty3=22;
    for(t=0; t<=1; t+=0.05){
        xt = (1-t)*(1-t)*(1-t)*tx0 + 3*(1-t)*(1-t)*t*tx1 + 3*(1-t)*t*t*tx2 + t*t*t*tx3;
        yt = (1-t)*(1-t)*(1-t)*ty0 + 3*(1-t)*(1-t)*t*ty1 + 3*(1-t)*t*t*ty2 + t*t*t*ty3;
        glVertex2f(xt, yt);
    }
    glEnd();
    // (Leaves & Leaflets)
    float leaf_pts[6][2] = {{6,18}, {8,25}, {-6,26}, {-8,18}, {0,28}, {4,27}};

    for(int i=0; i<6; i++) {
        glPushMatrix();
        glTranslatef(1.0f, 22.0f, 0.0f);
        glRotatef(sin(leafAngle + i) * 3.5f, 0.0f, 0.0f, 1.0f);
        glTranslatef(-1.0f, -22.0f, 0.0f);

        float lx0=1, ly0=22,
              lx1=leaf_pts[i][0]*0.5f, ly1=26,
              lx2=leaf_pts[i][0]*0.8f, ly2=leaf_pts[i][1]+2,
              lx3=leaf_pts[i][0], ly3=leaf_pts[i][1];
        // (Main Vein of the leaf)
        glColor3f(0.0f, 0.35f, 0.0f);
        glLineWidth(2.5f);
        glBegin(GL_LINE_STRIP);
        for(t=0; t<=1.0; t+=0.02){
            xt = (1-t)*(1-t)*(1-t)*lx0 + 3*(1-t)*(1-t)*t*lx1 + 3*(1-t)*t*t*lx2 + t*t*t*lx3;
            yt = (1-t)*(1-t)*(1-t)*ly0 + 3*(1-t)*(1-t)*t*ly1 + 3*(1-t)*t*t*ly2 + t*t*t*ly3;
            glVertex2f(xt, yt);
        }
        glEnd();
        //(Leaflets)
        glColor3f(0.0f, 0.45f, 0.0f);
        glLineWidth(1.0f);
        for(t=0; t<=1.01; t+=0.10){
            xt = (1-t)*(1-t)*(1-t)*lx0 + 3*(1-t)*(1-t)*t*lx1 + 3*(1-t)*t*t*lx2 + t*t*t*lx3;
            yt = (1-t)*(1-t)*(1-t)*ly0 + 3*(1-t)*(1-t)*t*ly1 + 3*(1-t)*t*t*ly2 + t*t*t*ly3;
            glBegin(GL_LINES);
                glVertex2f(xt, yt); glVertex2f(xt + 0.3f, yt - 1.8f);
                glVertex2f(xt, yt); glVertex2f(xt - 0.3f, yt - 1.8f);
            glEnd();
        }
        glPopMatrix();
    }
    // (Coconuts)
    glColor3f(0.1f, 0.3f, 0.0f);
    for(float c_ang=0; c_ang<6.28; c_ang+=2.0) {
        glBegin(GL_POLYGON);
        for(int ci=0; ci<20; ci++) {
            float a = 2.0f * 3.14159f * ci / 20;
            glVertex2f(1.0f + cos(c_ang)*0.8f + cos(a)*0.6f, 21.5f + sin(c_ang)*0.5f + sin(a)*0.6f);
        }
        glEnd();
    }
    glPopMatrix();
}

void drawSmallHouse(float tx, float ty) {
    glPushMatrix();
    glTranslatef(tx, ty, 0);
    // (Main Body)
    glColor3f(0.8f, 0.4f, 0.2f);
    glBegin(GL_POLYGON); // left wall
    glVertex2f(29.0f, 30.0f); glVertex2f(36.2f, 30.0f);
    glVertex2f(36.2f, 40.0f);glVertex2f(33.0f, 41.8f);
    glVertex2f(29.8f, 40.0f);
    glEnd();
    glColor3f(0.7f, 0.4f, 0.2f);
    glBegin(GL_QUADS); // right wall
    glVertex2f(36.2, 30.0f);  glVertex2f(36.2f, 40.0f);
    glVertex2f(44.0f, 38.0f);glVertex2f(44.0f, 31.5f);
    glEnd();
    glColor3f(0.3f, 0.15f, 0.05f);
    glBegin(GL_QUADS); // door
    glVertex2f(39.5f, 30.2f); glVertex2f(41.0f, 30.2f);
    glVertex2f(41.0f, 39.5f);glVertex2f(39.5f, 39.5f);
    glEnd();
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS); // Side window
    glVertex2f(37.2f, 32.8f); glVertex2f(37.2f, 35.1f);
    glVertex2f(38.6f, 35.1f); glVertex2f(38.6f, 32.8f);
    glEnd();
    glBegin(GL_QUADS); // Right window
    glVertex2f(41.9f, 33.0f);   glVertex2f(41.9f, 35.2f);
    glVertex2f(43.1f, 35.2f); glVertex2f(43.1f, 33.0f);
    glEnd();
    glBegin(GL_QUADS); // Front window
    glVertex2f(31.5f, 37.4f); glVertex2f(33.5f, 37.4f);
    glVertex2f(33.5f, 32.8f); glVertex2f(31.5f, 32.8f);
    glEnd();
    // (Base)
    glColor3f(0.2f, 0.55f, 0.65f);
    glBegin(GL_QUADS);
    glVertex2f(28.0f, 29.2f); glVertex2f(36.2f, 29.2f);
    glVertex2f(36.2f, 30.0f); glVertex2f(28.0f, 30.0f);
    glVertex2f(36.2f, 29.2f); glVertex2f(36.2, 30.0f);
    glVertex2f(44.0, 31.6f);  glVertex2f(45.0f, 31.0f);
    glEnd();
    //  (Roof)
    glColor3f(0.2f, 0.55f, 0.65f);
    glBegin(GL_QUAD_STRIP); // left roof
    glVertex2f(32.0, 42.0);   glVertex2f(33.2, 42.0);
    glVertex2f(28.0, 39.0);   glVertex2f(29.5, 39.3);
    glEnd();
    glBegin(GL_QUADS); // right roof
    glVertex2f(32.0f, 42.0f); glVertex2f(37.0f, 36.3f);
    glVertex2f(45.0f, 37.3f); glVertex2f(40.0f, 42.0f);
    glEnd();
    glPopMatrix();
}

void drawBigHouse(float tx, float ty) {
    glPushMatrix();
    glTranslatef(tx, ty, 0);
    //left roof
    glColor3f(0.4f, 0.5f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(5.5f, 36.0f);  glVertex2f(8.0f, 36.0f);
    glVertex2f(22.0f, 33.0f); glVertex2f(13.0f, 43.1f);
    glEnd();
    glColor3f(0.8f, 0.0f, 0.1f);
    glBegin(GL_POLYGON);//left wall
    glVertex2f(7.0f, 24.0f);  glVertex2f(20.0f, 24.0f);
    glVertex2f(20.0f, 37.0f);glVertex2f(14.5f, 43.0f);
    glVertex2f(7.0f, 36.0f);
    glEnd();
    glColor3f(0.7f, 0.0f, 0.1f);
    glBegin(GL_QUADS);//right wall
    glVertex2f(20.0f, 24.0f); glVertex2f(31.0f, 26.5f);
    glVertex2f(31.0f, 37.0f);glVertex2f(20.0f, 37.0f);
    glEnd();
    glColor3f(0.3f, 0.15f, 0.05f);
    glBegin(GL_QUADS);//door
    glVertex2f(25.0f, 37.0f); glVertex2f(27.0f, 37.0f);
    glVertex2f(27.0f, 26.0f); glVertex2f(25.0f, 25.0f);
    glEnd();
    glColor3f(0.4f, 0.5f, 0.6f);
    glBegin(GL_QUADS);//main roof
    glVertex2f(20.8f, 35.0f); glVertex2f(32.3f, 36.0f);
    glVertex2f(25.0f, 43.0f); glVertex2f(13.0f, 43.0f);
    glEnd();
    //font window
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(11.7f, 36.0f); glVertex2f(15.3f, 36.0f);
    glVertex2f(15.3f, 29.0f); glVertex2f(11.7f, 29.0f);
    //right window
    glVertex2f(28.1f, 29.4f); glVertex2f(30.0f, 29.4f);
    glVertex2f(30.0f, 32.7f); glVertex2f(28.1f, 32.7f);
    glEnd();
    //left window
    glBegin(GL_QUAD_STRIP);
    glVertex2f(21.6f, 28.8f); glVertex2f(23.6f, 28.8f);
    glVertex2f(21.6f, 32.6f); glVertex2f(23.6f, 32.6f);
    glEnd();
    glColor3f(0.4f, 0.5f, 0.6f);
    glBegin(GL_QUADS);//base
    glVertex2f(6.0f, 24.0f);  glVertex2f(20.0f, 24.0f);
    glVertex2f(20.0f, 25.0f); glVertex2f(7.0f, 25.0f);
    glVertex2f(20.0f, 24.0f); glVertex2f(32.0f, 26.2f);
    glVertex2f(31.0f, 26.8f); glVertex2f(20.0f, 25.0f);
    glEnd();
    glPopMatrix();
}

void drawSittingPlace(float tx, float ty) {
    glPushMatrix();
    glTranslatef(tx, ty, 0);

    // (Pillars/Legs)
    glLineWidth(4);
    glColor3f(0.4f, 0.0f, 0.1f);
    glBegin(GL_LINES);
        glVertex2f(65.0f, 25.0f); glVertex2f(65.0f, 23.0f); // Leg 1
        glVertex2f(67.0f, 23.0f); glVertex2f(67.0f, 22.0f); // Leg 2
        glVertex2f(72.0f, 25.0f); glVertex2f(72.0f, 22.8f); // Leg 3
        glVertex2f(74.0f, 23.0f); glVertex2f(74.0f, 22.0f); // Leg 4
    glEnd();

    // Setting Place X & Y
    glLineWidth(2);
    glBegin(GL_LINES);
        // Horizontal Lines (X)
        for(float i = 0; i <= 2.0f; i += 0.5f) {
            glVertex2f(65.0f + i, 25.0f - i);
            glVertex2f(72.0f + i, 25.0f - i);
        }

        // Slanted/Vertical Lines (Y)
        for(float i = 0; i <= 7.0f; i += 1.0f) {
            glVertex2f(65.0f + i, 25.0f);
            glVertex2f(67.0f + i, 23.0f);
        }
    glEnd();
    glPopMatrix();
}
void drawWell(float tx, float ty) {
    glPushMatrix();
    glTranslatef(tx, ty, 0);
    // (Well Body)
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(30.5f, 16.5f); glVertex2f(35.5f, 16.5f);
    glVertex2f(35.5f, 19.5f); glVertex2f(30.5f, 19.5f);
    glEnd();
    //  (Upper Rim)
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(30.0f, 19.5f); glVertex2f(36.0f, 19.5f);
    glVertex2f(36.0f, 20.5f); glVertex2f(30.0f, 20.5f);
    glEnd();
    //  (Pillars)
    glColor3f(0.0f, 0.0f, 1.0f);
    glRectf(31.0f, 20.5f, 31.5f, 24.5f); // Left pillar
    glRectf(34.5f, 20.5f, 35.0f, 24.5f); // Right pillar
    // (Roof)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(29.5f, 24.5f); glVertex2f(36.5f, 24.5f);
    glVertex2f(36.5f, 25.0f); glVertex2f(29.5f, 25.0f);
    glEnd();
    //  (Rope)
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINES);
        glVertex2f(33.0f, 24.5f); glVertex2f(33.0f, 21.5f);
    glEnd();
   // (Bucket)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(32.3f, 20.0f); glVertex2f(33.7f, 20.0f);
    glVertex2f(33.9f, 21.5f); glVertex2f(32.1f, 21.5f);
    glEnd();
    glPopMatrix();
}

void drawClothesline(float tx, float ty) {
    glPushMatrix();
    glTranslatef(tx, ty, 0.0f);
    float t_cl, xt_cl, yt_cl;
    // Pillars
    glColor3f(0.3f, 0.15f, 0.05f);
    // Left Pillar
    glRectf(75.0f, 17.0f, 75.3f, 24.0f);
    // Right Pillar
    glRectf(85.0f, 17.0f, 85.3f, 24.0f);
    // Rope (Slight Curve)
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    for(t_cl = 0; t_cl <= 1.0f; t_cl += 0.05f) {

        xt_cl = 75.3f * (1 - t_cl) + 85.5f * t_cl;

        // Slight hanging rope
        yt_cl = 23.0f - 0.6f * sin(t_cl * 3.14159f);

        glVertex2f(xt_cl, yt_cl);
    }
    glEnd();

    if(!isNight && !isRaining) {
    // RED GAMCHA
        glPushMatrix();
        glTranslatef(76.75f, 22.6f, 0.0f);
        glRotatef(sin(clothWave) * 8.0f, 0.0f, 0.0f, 1.0f);
        glTranslatef(-76.75f, -22.6f, 0.0f);
        glColor3f(0.9f, 0.1f, 0.1f);
        glBegin(GL_POLYGON);
            glVertex2f(76.0f, 22.6f);glVertex2f(77.5f, 22.6f);
            glVertex2f(77.5f, 18.5f);glVertex2f(76.0f, 18.5f);
        glEnd();

        glPopMatrix();
        // BLUE PANT
        glPushMatrix();
        glTranslatef(79.25f, 22.4f, 0.0f);
        glRotatef(sin(clothWave + 1.0f) * 10.0f,0.0f, 0.0f, 1.0f);
        glTranslatef(-79.25f, -22.4f, 0.0f);
        glColor3f(0.0f, 0.3f, 0.7f);
        glBegin(GL_POLYGON);
            glVertex2f(78.0f, 22.4f);glVertex2f(80.5f, 22.4f);
            glVertex2f(80.8f, 18.0f);glVertex2f(79.8f, 18.0f);
            glVertex2f(79.2f, 20.5f);glVertex2f(78.7f, 18.0f);
            glVertex2f(77.8f, 18.0f);
        glEnd();
        glPopMatrix();
        // TEAL PANT (Right)
        glPushMatrix();
        glTranslatef(82.75f, 22.5f, 0.0f);
        glRotatef(sin(clothWave + 2.0f) * 12.0f,0.0f, 0.0f, 1.0f);
        glTranslatef(-82.75f, -22.5f, 0.0f);
        glColor3f(0.0f, 0.4f, 0.4f);
        glBegin(GL_POLYGON);
            glVertex2f(81.5f, 22.5f); glVertex2f(84.0f, 22.5f);
            glVertex2f(84.3f, 18.0f);glVertex2f(83.3f, 18.0f);
            glVertex2f(82.7f, 20.5f);glVertex2f(82.2f, 18.0f);
            glVertex2f(81.2f, 18.0f);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();
}


void drawSingleStick(float x, float yLower, float yUpper, float tipHeight) {
    glBegin(GL_POLYGON);
    glVertex2f(x, yLower);glVertex2f(x + 0.4f, yLower);
    glVertex2f(x + 0.4f, yUpper);glVertex2f(x, yUpper);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x, yUpper);glVertex2f(x + 0.4f, yUpper);
    glVertex2f(x + 0.2f, yUpper + tipHeight);
    glEnd();
}

//(Left Side Fence)
void drawYellowFenceLeft(float tx, float ty) {
    glPushMatrix();
    glTranslatef(tx, ty, 0);
    glColor3f(0.5f, 0.3f, 0.1f);
    //(Horizontal Rails)
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 21.5f); glVertex2f(29.0f, 21.5f); glVertex2f(29.0f, 21.2f); glVertex2f(0.0f, 21.2f); // Top
    glVertex2f(0.0f, 19.5f); glVertex2f(29.0f, 19.5f); glVertex2f(29.0f, 19.2f); glVertex2f(0.0f, 19.2f); // Bottom
    glEnd();
    // (Vertical Sticks using Loop)
    for (float x = 0.3f; x <= 28.9f; x += 1.2f) {
        drawSingleStick(x, 18.0f, 22.5f, 0.7f);
    }
    glPopMatrix();
}

// (Right Side Fence)
void drawYellowFenceRight(float tx, float ty) {
    glPushMatrix();
    glTranslatef(tx, ty, 0);
    glColor3f(0.5f, 0.3f, 0.1f);
    // (Horizontal Rails)
    glBegin(GL_QUADS);
    glVertex2f(86.0f, 22.5f); glVertex2f(100.0f, 22.5f); glVertex2f(100.0f, 22.2f); glVertex2f(86.0f, 22.2f); // Top
    glVertex2f(86.0f, 20.5f); glVertex2f(100.0f, 20.5f); glVertex2f(100.0f, 20.2f); glVertex2f(86.0f, 20.2f); // Bottom
    glEnd();
    // (Vertical Sticks using Loop)
    for (float x = 86.3f; x <= 100.7f; x += 1.2f) {
        drawSingleStick(x, 19.0f, 23.5f, 0.7f);
    }

    glPopMatrix();

}
void drawSingleBird(float x, float y, float angle, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    //(Left Wing)
    glPushMatrix();
    glTranslatef(-1.5f, 0.0f, 0.0f);
    glRotatef(wingAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);glVertex2f(-1.5f, 2.0f);
    glVertex2f(1.5f, 0.0f);
    glEnd();
    glPopMatrix();
    // (Right Wing)
    glPushMatrix();
    glTranslatef(1.5f, 0.0f, 0.0f);
    glRotatef(-wingAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);glVertex2f(1.5f, 2.0f);
    glVertex2f(-1.5f, 0.0f);
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

float cloudX = 0.0f;
//cloud
void drawSingleCloud(float tx, float ty, float sx, float sy, int index) {
    glPushMatrix();

    float currentX = tx + cloudOffsets[index];

    if (currentX > 110.0f) {
        cloudOffsets[index] = -tx - 20.0f;
    }
    glTranslatef(tx + cloudOffsets[index], ty, 0.0f);
    glScalef(sx, sy, 1.0f);
    if (isRaining) glColor3f(0.4f, 0.4f, 0.4f);
else glColor3f(1.0f, 1.0f, 1.0f);
    float angle;
    float centers[4] = {0.0f, 4.0f, 9.0f, 13.0f};
    float radii[4] = {2.2f, 3.8f, 3.8f, 2.2f};
    float y_scales[4] = {2.8f, 4.5f, 4.5f, 2.8f};

    for(int j = 0; j < 4; j++) {
        glBegin(GL_POLYGON);
        for(int i = 0; i < 100; i++) {
            angle = 2.0f * 3.14159f * i / 100;
            glVertex2f(centers[j] + cos(angle) * radii[j], 0.0f + sin(angle) * y_scales[j]);
        }
        glEnd();
    }
    glPopMatrix();
}

//road
void drawRoadSegment(float ux0, float uy0, float ux1, float uy1, float ux2, float uy2, float ux3, float uy3,
                      float lx0, float ly0, float lx1, float ly1, float lx2, float ly2, float lx3, float ly3) {

    glBegin(GL_QUAD_STRIP);
    for(float t = 0; t <= 1; t += 0.01) {
        // Upper Curve
        float uxt = (1-t)*(1-t)*(1-t)*ux0 + 3*(1-t)*(1-t)*t*ux1 + 3*(1-t)*t*t*ux2 + t*t*t*ux3;
        float uyt = (1-t)*(1-t)*(1-t)*uy0 + 3*(1-t)*(1-t)*t*uy1 + 3*(1-t)*t*t*uy2 + t*t*t*uy3;

        // lower curve
        float lxt = (1-t)*(1-t)*(1-t)*lx0 + 3*(1-t)*(1-t)*t*lx1 + 3*(1-t)*t*t*lx2 + t*t*t*lx3;
        float lyt = (1-t)*(1-t)*(1-t)*ly0 + 3*(1-t)*(1-t)*t*ly1 + 3*(1-t)*t*t*ly2 + t*t*t*ly3;

        glVertex2f(uxt, uyt);
        glVertex2f(lxt, lyt);
    }
    glEnd();
}

void drawFullRoad() {
    glColor3f(0.6f, 0.4f, 0.2f);

    drawRoadSegment(33, 14.5, 53, 22, 46, 25, 46, 30,
                    55, 13, 64, 18, 66, 23, 63, 30);

    drawRoadSegment(46, 30, 46, 36, 51, 40, 63, 45,
                    63, 30, 62, 32, 64, 35, 66, 37);

    drawRoadSegment(63, 45, 68, 47, 75, 50, 100, 52,
                    66, 37, 68, 38, 73, 45, 100, 49);
}

void drawSmallHouseRight(float tx, float ty) {
    glPushMatrix();
    glTranslatef(tx, ty, 0);

    // Body
    glColor3f(0.9f, 0.5f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(71.0f, 37.0f); glVertex2f(80.0f, 37.0f);
        glVertex2f(80.0f, 29.0f); glVertex2f(71.0f, 29.0f);
    glEnd();

    // Door
    glColor3f(0.3f, 0.15f, 0.05f);
    glBegin(GL_QUADS);
        glVertex2f(76.0f, 38.0f); glVertex2f(80.0f, 38.0f);
        glVertex2f(80.0f, 29.0f); glVertex2f(76.0f, 29.0f);
    glEnd();

    // Roof & Base
    glColor3f(0.6f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
        // Base
        glVertex2f(71.0f, 29.0f); glVertex2f(80.0f, 29.0f);
        glVertex2f(80.0f, 28.0f); glVertex2f(69.2f, 28.0f);
        // Roof Side
        glVertex2f(75.0f, 40.5f); glVertex2f(81.0f, 40.5f);
        glVertex2f(81.0f, 36.0f); glVertex2f(69.0f, 36.0f);
    glEnd();

    // Window
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(72.5f, 34.0f); glVertex2f(74.5f, 34.0f);
    glVertex2f(74.5f, 31.5f); glVertex2f(72.5f, 31.5f);
    glEnd();
    glPopMatrix();
}

void drawBigHouseRight(float tx, float ty) {
    glPushMatrix();
    glTranslatef(tx, ty, 0);
    // Walls
    glColor3f(0.2f, 0.2f, 0.6f);
    glBegin(GL_POLYGON);
    glVertex2f(88.0f, 36.0f); glVertex2f(92.5f, 41.5f);
    glVertex2f(98.0f, 36.0f); glVertex2f(98.0f, 25.0f);
    glVertex2f(88.0f, 25.0f);
    glEnd();

    // Side Wall
    glColor3f(0.2f, 0.1f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(88.0f, 25.0f); glVertex2f(78.0f, 28.0f);
    glVertex2f(78.0f, 35.5f); glVertex2f(88.0f, 36.0f);
    glEnd();
    // Door
    glColor3f(0.3f, 0.15f, 0.05f);
    glBegin(GL_QUADS);
    glVertex2f(81.8f, 26.1f); glVertex2f(84.2f, 25.8f);
    glVertex2f(84.2f, 35.2f); glVertex2f(81.8f, 35.2f);
    glEnd();
    //Roofs & Border
    glColor3f(0.7f, 0.4f, 0.0f);
    glBegin(GL_QUADS);
    // Right Roof Side
    glVertex2f(99.0f, 35.0f); glVertex2f(93.0f, 41.4);
    glVertex2f(92.4f, 41.0f); glVertex2f(98.0f, 35.0f);
    glVertex2f(93.0f, 41.5f); glVertex2f(81.0f, 41.5f);
    glVertex2f(76.8f, 34.5f); glVertex2f(87.2f, 33.5f);
    glVertex2f(78.0f, 28.0f); glVertex2f(88.0f, 25.0f);
    glVertex2f(88.0f, 24.3f); glVertex2f(77.2f, 27.5f);
    glVertex2f(88.0f, 25.0f); glVertex2f(88.0f, 24.3f);
    glVertex2f(98.6f, 24.3f); glVertex2f(98.0f, 25.0f);
    glEnd();
    //Windows
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);// Right Window
    glVertex2f(87.0f, 29.1f); glVertex2f(87.0f, 32.0f);
    glVertex2f(85.5f, 32.0f); glVertex2f(85.5f, 29.1f);
    // Front Window
    glVertex2f(91.6f, 34.5f); glVertex2f(94.9f, 34.5f);
    glVertex2f(94.9f, 27.5f); glVertex2f(91.6f, 27.5f);
    glEnd();
    glBegin(GL_QUAD_STRIP); // Left Window
    glVertex2f(80.5f, 30.1f); glVertex2f(79.0f, 30.1f);
    glVertex2f(80.5f, 32.8f); glVertex2f(79.0f, 32.8f);
    glEnd();
    glPopMatrix();
}

void drawRiver(float xPos, float yPos, float width, float height) {
    glPushMatrix();
    glTranslatef(xPos, yPos, 0);
    glBegin(GL_QUAD_STRIP);
    for (float x = 0; x <= width; x += 1.0f) {
        float wave = sin((x * 0.2f) + waveOffset) * 1.2f;
        glColor3f(0.0f, 0.45f, 0.85f);
        glVertex2f(x, height + wave);
        glColor3f(0.0f, 0.3f, 0.6f);
        glVertex2f(x, -10.0f);
    }
    glEnd();
    glPopMatrix();
}

void drawKochuripana(float cx, float cy, float scale) {
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glScalef(scale, scale, 1.0f);
    glRotatef(sin(hyacinthOffset+ cx * 0.4f) * 5.0f, 0.0f, 0.0f, 1.0f);
    float t, r, x, y;
    glColor3f(0.0f, 0.4f, 0.0f);
    glBegin(GL_POLYGON);
    for(t = 0; t <= 6.28; t += 0.02) {
        r = 2.0 + 0.1 * sin(t * 5);
        x = r * cos(t);
        y = (r * 0.4) * sin(t);
        glVertex2f(x, y);
    }
    glEnd();
    // Purple Flower
    glColor3f(0.8f, 0.6f, 0.9f);
    glBegin(GL_POLYGON);
    for(t = 0; t <= 6.28; t += 0.1) {
        r = 0.7 + 0.14 * sin(t * 4);
        x = 0.2 + r * cos(t);
        y = 1.2 + (r * 0.8) * sin(t);
        glVertex2f(x, y);
    }
    glEnd();
    // Center Point
    glColor3f(0.5f, 0.0f, 0.5f);
    glPointSize(3);
    glBegin(GL_POINTS);
        glVertex2f(0.2, 1.2);
    glEnd();
    glPopMatrix();
}

void drawBoatman(float tx, float ty, float sx, float sy) {
    glPushMatrix();
    glTranslatef(tx, ty, 0.0f);
    glScalef(sx, sy, 1.0f);
    // Head & Hair
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 100; i++) {
        float m_angle = 2.0f * 3.14159f * i / 100;
        glVertex2f(72.0f + cos(m_angle) * 0.7f, 15.1f + sin(m_angle) * 0.5f);
    }
    glEnd();
    // Face
    glColor3f(1.0f, 0.85f, 0.7f);
    glBegin(GL_POLYGON);
    glVertex2f(71.5f, 14.1f);
    glVertex2f(72.5f, 14.1f);
    glVertex2f(72.6f, 14.9f);
    glVertex2f(71.4f, 14.9f);
    glEnd();
    // Body/Shirt
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(71.3f, 12.0f); glVertex2f(72.8f, 12.0f);
    glVertex2f(72.4f, 14.1f); glVertex2f(71.5f, 14.1f);
    glEnd();
    // Lower Body
    glColor3f(0.6f, 0.1f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(71.4f, 9.5f); glVertex2f(72.6f, 9.5f);
    glVertex2f(72.7f, 12.0f); glVertex2f(71.2f, 12.0f);
    glEnd();
    glPushMatrix();
        glTranslatef(72.0f, 13.5f, 0.0f);
        glRotatef(oarAngle * 0.3f, 0.0f, 0.0f, 1.0f);
        glTranslatef(-72.0f, -13.5f, 0.0f);

        glLineWidth(2.5f);
        glEnable(GL_MAP1_VERTEX_3);
        // Right Arm Curve
        glColor3f(1.0f, 0.85f, 0.7f);
        GLfloat m_hand_top[3][3] = {
            {72.5f, 13.8f, 0.0f},
            {73.2f, 14.2f, 0.0f},
            {72.6f, 15.1f, 0.0f}
        };
        glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, 3, &m_hand_top[0][0]);
        glBegin(GL_LINE_STRIP);
        for(int i = 0; i <= 10; i++) {
            glEvalCoord1f((GLfloat)i / 10.0f);
        }
        glEnd();
        // Left Arm Curve
        glColor3f(1.0f, 0.85f, 0.7f);
        GLfloat m_hand_btm[3][3] = {
            {71.3f, 13.4f, 0.0f},
            {70.5f, 12.9f, 0.0f},
            {70.0f, 13.6f, 0.0f}
        };
        glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, 3, &m_hand_btm[0][0]);
        glBegin(GL_LINE_STRIP);
        for(int i = 0; i <= 10; i++) {
            glEvalCoord1f((GLfloat)i / 10.0f);
        }
        glEnd();
        // Boitha Stick
        glColor3f(0.25f, 0.15f, 0.10f);
        glLineWidth(3.5f);
        glBegin(GL_LINES);
        glVertex2f(73.5f, 16.5f); glVertex2f(64.0f, 8.0f);
        glEnd();
        // blade
        glColor3f(0.25f, 0.15f, 0.10f);
        glBegin(GL_POLYGON);
        glVertex2f(64.5f, 8.5f); glVertex2f(62.0f, 6.5f);
        glVertex2f(63.0f, 5.8f); glVertex2f(65.5f, 7.8f);
        glEnd();
    glPopMatrix();
    glPopMatrix();
}

void drawBoat() {
    glLineWidth(1);
    glColor3f(0.25f, 0.15f, 0.10f);
    glBegin(GL_QUADS);
        glVertex2f(70.0f, 5.0f);glVertex2f(83.0f, 5.0f);
        glVertex2f(83.0f, 10.0f);glVertex2f(70.0f, 10.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(70.0f, 5.0f);
        glVertex2f(70.0f, 10.0f);glVertex2f(65.0f, 12.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(83.0f, 5.0f);
        glVertex2f(83.0f, 10.0f);glVertex2f(88.0f, 12.0f);
    glEnd();
}

void drawBezierTrunk() {
    glColor3f(0.36f, 0.25f, 0.15f);
    glLineWidth(10);
    float t_bz, xt_bz, yt_bz;
    // 1st Curve
    glBegin(GL_LINE_STRIP);
    float x0=2, y0=24.7, x1=8, y1=40, x2=5, y2=45, x3=7, y3=51;
    for(t_bz=0; t_bz<=1; t_bz+=0.01){
        xt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*x0 + 3*(1-t_bz)*(1-t_bz)*t_bz*x1 + 3*(1-t_bz)*t_bz*t_bz*x2 + t_bz*t_bz*t_bz*x3;
        yt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*y0 + 3*(1-t_bz)*(1-t_bz)*t_bz*y1 + 3*(1-t_bz)*t_bz*t_bz*y2 + t_bz*t_bz*t_bz*y3;
        glVertex2f(xt_bz, yt_bz);
    }
    glEnd();
    // 2nd Curve
    glBegin(GL_LINE_STRIP);
    float qx0=7, qy0=51, qx1=9, qy1=54, qx2=11, qy2=54, qx3=13, qy3=54;
    for(t_bz=0; t_bz<=1; t_bz+=0.01){
        xt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*qx0 + 3*(1-t_bz)*(1-t_bz)*t_bz*qx1 + 3*(1-t_bz)*t_bz*t_bz*qx2 + t_bz*t_bz*t_bz*qx3;
        yt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*qy0 + 3*(1-t_bz)*(1-t_bz)*t_bz*qy1 + 3*(1-t_bz)*t_bz*t_bz*qy2 + t_bz*t_bz*t_bz*qy3;
        glVertex2f(xt_bz, yt_bz);
    }
    glEnd();
    // 3rd Curve
    glBegin(GL_LINE_STRIP);
    float px0=13, py0=54, px1=12, py1=55, px2=12, py2=55, px3=11, py3=55;
    for(t_bz=0; t_bz<=1; t_bz+=0.01){
        xt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*px0 + 3*(1-t_bz)*(1-t_bz)*t_bz*px1 + 3*(1-t_bz)*t_bz*t_bz*px2 + t_bz*t_bz*t_bz*px3;
        yt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*py0 + 3*(1-t_bz)*(1-t_bz)*t_bz*py1 + 3*(1-t_bz)*t_bz*t_bz*py2 + t_bz*t_bz*t_bz*py3;
        glVertex2f(xt_bz, yt_bz);
    }
    glEnd();
    // 4th Curve
    glBegin(GL_LINE_STRIP);
    float wx0=11, wy0=55, wx1=10, wy1=55, wx2=7, wy2=53, wx3=7, wy3=53;
    for(t_bz=0; t_bz<=1; t_bz+=0.01){
        xt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*wx0 + 3*(1-t_bz)*(1-t_bz)*t_bz*wx1 + 3*(1-t_bz)*t_bz*t_bz*wx2 + t_bz*t_bz*t_bz*wx3;
        yt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*wy0 + 3*(1-t_bz)*(1-t_bz)*t_bz*wy1 + 3*(1-t_bz)*t_bz*t_bz*wy2 + t_bz*t_bz*t_bz*wy3;
        glVertex2f(xt_bz, yt_bz);
    }
    glEnd();
    // 5th Curve
    glBegin(GL_LINE_STRIP);
    float mx0=7, my0=53, mx1=6, my1=58, mx2=7, my2=57, mx3=8, my3=60;
    for(t_bz=0; t_bz<=1; t_bz+=0.01){
        xt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*mx0 + 3*(1-t_bz)*(1-t_bz)*t_bz*mx1 + 3*(1-t_bz)*t_bz*t_bz*mx2 + t_bz*t_bz*t_bz*mx3;
        yt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*my0 + 3*(1-t_bz)*(1-t_bz)*t_bz*my1 + 3*(1-t_bz)*t_bz*t_bz*my2 + t_bz*t_bz*t_bz*my3;
        glVertex2f(xt_bz, yt_bz);
    }
    glEnd();
    // 6th Curve
    glBegin(GL_LINE_STRIP);
    float vx0=8, vy0=60, vx1=6, vy1=60, vx2=5, vy2=58, vx3=4.3, vy3=50;
    for(t_bz=0; t_bz<=1; t_bz+=0.01){
        xt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*vx0 + 3*(1-t_bz)*(1-t_bz)*t_bz*vx1 + 3*(1-t_bz)*t_bz*t_bz*vx2 + t_bz*t_bz*t_bz*vx3;
        yt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*vy0 + 3*(1-t_bz)*(1-t_bz)*t_bz*vy1 + 3*(1-t_bz)*t_bz*t_bz*vy2 + t_bz*t_bz*t_bz*vy3;
        glVertex2f(xt_bz, yt_bz);
    }
    glEnd();
    // 7th Curve
    glBegin(GL_LINE_STRIP);
    float nx0=4.3, ny0=50, nx1=4, ny1=50, nx2=3, ny2=50, nx3=2, ny3=60;
    for(t_bz=0; t_bz<=1; t_bz+=0.01){
        xt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*nx0 + 3*(1-t_bz)*(1-t_bz)*t_bz*nx1 + 3*(1-t_bz)*t_bz*t_bz*nx2 + t_bz*t_bz*t_bz*nx3;
        yt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*ny0 + 3*(1-t_bz)*(1-t_bz)*t_bz*ny1 + 3*(1-t_bz)*t_bz*t_bz*ny2 + t_bz*t_bz*t_bz*ny3;
        glVertex2f(xt_bz, yt_bz);
    }
    glEnd();
    // 8th Curve
    glBegin(GL_LINE_STRIP);
    float hx0=2, hy0=60, hx1=-2, hy1=58, hx2=6, hy2=45, hx3=0, hy3=35;
    for(t_bz=0; t_bz<=1; t_bz+=0.01){
        xt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*hx0 + 3*(1-t_bz)*(1-t_bz)*t_bz*hx1 + 3*(1-t_bz)*t_bz*t_bz*hx2 + t_bz*t_bz*t_bz*hx3;
        yt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*hy0 + 3*(1-t_bz)*(1-t_bz)*t_bz*hy1 + 3*(1-t_bz)*t_bz*t_bz*hy2 + t_bz*t_bz*t_bz*hy3;
        glVertex2f(xt_bz, yt_bz);
    }
    glEnd();
    // 9th Curve
    glBegin(GL_LINE_STRIP);
    float gx0=0, gy0=35, gx1=0, gy1=25, gx2=-3, gy2=25, gx3=2, gy3=25;
    for(t_bz=0; t_bz<=1; t_bz+=0.01){
        xt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*gx0 + 3*(1-t_bz)*(1-t_bz)*t_bz*gx1 + 3*(1-t_bz)*t_bz*t_bz*gx2 + t_bz*t_bz*t_bz*gx3;
        yt_bz = (1-t_bz)*(1-t_bz)*(1-t_bz)*gy0 + 3*(1-t_bz)*(1-t_bz)*t_bz*gy1 + 3*(1-t_bz)*t_bz*t_bz*gy2 + t_bz*t_bz*t_bz*gy3;
        glVertex2f(xt_bz, yt_bz);
    }
    glEnd();
}

void drawSolidTrunk() {
    glColor3f(0.36f, 0.25f, 0.15f);

    // (Base Part)
    glBegin(GL_QUADS);
        glVertex2f(-5.0f, 25.0f);glVertex2f(2.2f, 24.5f);
        glVertex2f(5.5f, 37.0f);glVertex2f(1.0f, 37.5f);
    glEnd();
    // (Middle Part)
    glBegin(GL_QUADS);
        glVertex2f(1.3f, 37.5f);glVertex2f(5.4f, 36.0f);
        glVertex2f(6.4f, 50.0f);glVertex2f(3.0f, 50.0f);
    glEnd();
    // (Left Branch)
    glBegin(GL_QUADS);
        glVertex2f(4.2f, 50.0f);glVertex2f(2.9f, 43.0f);
        glVertex2f(2.5f, 55.0f);glVertex2f(0.5f, 58.0f);
    glEnd();
    // (Center Top Branch)
    glBegin(GL_QUADS);
        glVertex2f(3.8f, 50.0f);glVertex2f(7.0f, 50.0f);
        glVertex2f(7.0f, 59.0f);glVertex2f(5.0f, 59.0f);
    glEnd();
    // (Right Branch)
    glBegin(GL_QUADS);
    glVertex2f(7.0f, 53.0f);glVertex2f(16.0f, 58.0f);
    glVertex2f(16.0f, 56.0f);glVertex2f(6.4f, 50.5f);
    glEnd();
}

void drawCircle(float x, float y, float rx, float ry) {
    glBegin(GL_POLYGON);
    for(int i = 0; i < 60; i++) {
        float angle = 2.0f * 3.14159f * i / 60;
        glVertex2f(x + cos(angle) * rx, y + sin(angle) * ry);
    }
    glEnd();
}

void drawBotLeaves() {
    glColor3f(0.0f, 0.35f, 0.0f);
    glPushMatrix();
    glTranslatef(16.0f, 57.0f, 0.0f);
    drawCircle(0, 0, 7.0f, 6.5f);
    drawCircle(0, 4.0f, 4.0f, 4.0f);
    for(int j = 0; j < 360; j += 45) {
    float rad = j * 3.14159f / 180.0f;
    drawCircle(cos(rad) * 5.0f, sin(rad) * 4.5f, 3.5f, 3.5f);
 }
    glPopMatrix();
    glPushMatrix();
    glScalef(0.5f, 0.5f, 1.0f);
    glTranslatef(0.2f, 113.0f, 0.0f);
    drawCircle(0, 0, 7.0f, 6.5f);
    for(int j = 0; j < 360; j += 45) {
    float rad = j * 3.14159f / 180.0f;
    drawCircle(cos(rad) * 5.0f, sin(rad) * 4.5f, 3.5f, 3.5f);
 }
    glPopMatrix();
    glPushMatrix();
    glTranslatef(8.0f, 68.0f, 0.0f);
    for(int j = -5; j <= 5; j += 5) {
    drawCircle((float)j, 0, 7.0f, 9.0f);
 }
    for(int j = 0; j < 360; j += 30) {
    float rad = j * 3.14159f / 180.0f;
    drawCircle(cos(rad) * 8.0f, sin(rad) * 7.0f, 4.0f, 5.5f);
}
    glPopMatrix();
}
void introScreen() {
    glClearColor(0.85f, 0.93f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    // Welcome Title
    glRasterPos2f(36.0f, 60.0f);
    char title[] = "WELCOME TO MY POOKIE VILLAGE";
    for(int i = 0; title[i] != '\0'; i++) {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,
                            title[i]);
    }
    // Course Name
    glRasterPos2f(40.0f, 50.0f);
    char course[] = "COURSE : COMPUTER GRAPHICS";
    for(int i = 0; course[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,
                            course[i]);
    }
    // Created By
    glRasterPos2f(39.0f, 45.0f);
    char creator[] =
    "A PROJECT BY: ATIKA RAHMAN RIA";
    for(int i = 0; creator[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,
                         creator[i]);
    }
    // Supervised By
    glRasterPos2f(38.0f, 40.0f);
    char supervisor1[] =
    "SUPERVISED BY: MAHFUJUR RAHMAN";

    for(int i = 0; supervisor1[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,
                            supervisor1[i]);
    }
    glRasterPos2f(45.0f, 35.0f);
    char supervisor2[] =
    "Assistant Professor";
    for(int i = 0; supervisor2[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,
                            supervisor2[i]);
    }
    // Instruction
    glRasterPos2f(32.0f, 23.0f);
    char instruction[] =
    "PRESS D FOR DAY VIEW, N FOR NIGHT VIEW, R FOR RAIN VIEW, M FOR MUTE, U FOR UNMUTE";

    for(int i = 0; instruction[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,
                            instruction[i]);
    }
    glutSwapBuffers();
}

void display() {
    if(showIntro) {
    introScreen();
    return;
}

if (isNight && isRaining) {
    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
    } else if (isNight) {
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
    } else if (isRaining) {
        glClearColor(0.4f, 0.5f, 0.6f, 1.0f);
    } else {
        glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();
    if (!isNight && !isRaining) {
        drawSun(48.0f, 72.5f, 4.0f);
    }
    if (isNight) {
            if (!isRaining) {
            drawMoon(85.0f, 70.0f, 4.0f);
        }
        if (!isRaining) {
            if (!starsInitialized) {
                initStars();
            }
            glColor3f(1.0f, 1.0f, 1.0f);
            glPointSize(2.0f);
            glBegin(GL_POINTS);
            for (int i = 0; i < 60; i++) {
                glVertex2f(starX[i], starY[i]);
            }
            glEnd();
        }
    }


if (!isNight && !isRaining) {
    glPushMatrix();
    glTranslatef(birdAutoX + birdControlX, birdControlY, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawSingleBird(33.0f, 50.5f, 30.0f, 0.4f);
    drawSingleBird(36.0f, 52.0f, 25.0f, 0.4f);
    drawSingleBird(39.0f, 53.3f, 35.0f, 0.4f);
    drawSingleBird(35.5f, 50.0f, 20.0f, 0.4f);
    drawSingleBird(38.5f, 51.2f, 15.0f, 0.4f);
    drawSingleBird(38.0f, 49.0f, 30.0f, 0.4f);
    glPopMatrix();
}
    if (!isNight) {
    if (isRaining) glColor3f(0.4f, 0.4f, 0.4f);
    else glColor3f(1.0f, 1.0f, 1.0f);
    drawSingleCloud(10.0f, 67.0f, 0.8f, 0.8f, 0);
    drawSingleCloud(35.0f, 72.0f, 0.5f, 0.5f, 1);
    drawSingleCloud(55.0f, 68.0f, 0.9f, 0.9f, 2);
    drawSingleCloud(85.0f, 70.0f, 0.6f, 0.6f, 3);
    drawSingleCloud(99.0f, 67.0f, 0.7f, 0.7f, 4);
    drawSingleCloud(-5.0f, 72.0f, 0.5f, 0.5f, 5);}
    drawAllBambooFences(0, 0);
    drawBezierTrunk();
    drawSolidTrunk();
    drawBotLeaves();
    drawFullRoad();
    float positions[8][2] = {{92.0f, 34.0f}, {96.0f, 37.0f}, {90.0f, 51.0f}, {62.0f, 44.5f}, {62.9f, 30.4f}, {76.0f, 43.0f}, {48.2f, 36.0f}, {58.0f, 15.0f}};
    for(int i = 0; i < 8; i++) drawCoconutTree(positions[i][0], positions[i][1], 0.4f);
    drawSmallHouse(0,-0.5);
    drawBigHouse(0,0.5);
    drawSmallHouseRight(0, 0);
    drawBigHouseRight(0, 0);
    drawSittingPlace(0, 0);
    drawWell(0, 0.9);
    drawClothesline(0, 0.5);
    drawYellowFenceLeft(0, 0);
    drawYellowFenceRight(0, 0);
    drawRiver(0.0f, 5.0f, 100.0f, 11.0f);
    drawKochuripana(25.0, 6.0, 1.0);
    drawKochuripana(13.0, 5.5, 0.9);
    drawKochuripana(88.0, 6.0, 1.0);
    drawKochuripana(17.0, 8.0, 1.0);
    glPushMatrix();
    glTranslatef(boatX, 0.0f, 0.0f);
    if (boatDirection == 2) {
        glTranslatef(77.0f, 0.0f, 0.0f);
        glScalef(-1.0f, 1.0f, 1.0f);
        glTranslatef(-77.0f, 0.0f, 0.0f);
    }
    if (!isNight && !isRaining) {
        drawBoatman(0.0f, 0.0f, 1.0f, 1.0f);
    }
  drawBoat();
  glPopMatrix();
    if (isRaining) {
        drawRain();
    }
  glFlush();
}

void update(int value) {
    for (int i = 0; i < 6; i++) {
        cloudOffsets[i] += cloudSpeeds[i];
        if (cloudOffsets[i] > 110.0f) cloudOffsets[i] = -120.0f;
    }
    if (isRaining) {
        for (int i = 0; i < 500; i++) {
            rainY[i] -= 1.5f;
            rainX[i] -= 0.5f;
            if (rainY[i] < 0) {
                rainY[i] = 80.0f;
                rainX[i] = rand() % 130;
            }
        }
    }
    if (!isNight && !isRaining) {
        birdAutoX -= 0.02f;
        birdControlY += 0.08f;
        if (birdAutoX < -50.0f) {
        birdControlY = 0.0f;
        }
    }
    if (wingUp) {
        wingAngle += 0.8f;
        if (wingAngle > 15.0f) wingUp = false;
    } else {
        wingAngle -= 0.8f;
        if (wingAngle < -15.0f) wingUp = true;
    }
    if (!isNight && !isRaining) {
        if (boatDirection == 1) {
            boatX += 0.15f;
            if (boatX > 40.0f) boatX = -80.0f;
        }
        else if (boatDirection == 2) {
            boatX -= 0.15f;
            if (boatX < -110.0f) boatX = 20.0f;
        }
    }
    else {
        boatX = 0.0f;
    }
    if (boatDirection != 0 && !isNight && !isRaining) {
        if (oarForward) {
            oarAngle += 1.5f;
        if (oarAngle > 15.0f) oarForward = false;
        }
        else {
            oarAngle -= 1.5f;
            if (oarAngle < -15.0f) oarForward = true;
        }
    }
    waveOffset += 0.02f;
    leafAngle += 0.05f;
    clothWave += 0.01f;
    waveOffset += 0.00f;
    hyacinthOffset += 0.07f;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}
void keyboard(unsigned char key, int x, int y) {
    if(showIntro) {
        showIntro = false;
        playCurrentSound();
        glutPostRedisplay();
        return;
    }
    switch (key) {
        // RAIN VIEW
        case 'r':
        case 'R':
            isRaining = !isRaining;
            playCurrentSound();
            break;
        // NIGHT VIEW
        case 'n':
        case 'N':
            isNight = !isNight;
            playCurrentSound();
            break;
        // DAY VIEW
        case 'd':
        case 'D':
            isNight = false;
            isRaining = false;
            playCurrentSound();
            break;
        // MUTE MUSIC
        case 'm':
        case 'M':
            PlaySound(NULL, NULL, 0);
            musicOn = false;
            break;
        // UNMUTE MUSIC
        case 'u':
        case 'U':
            musicOn = true;
            playCurrentSound();
            break;
    }

    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            birdControlY += 1.0f;
            break;
        case GLUT_KEY_DOWN:
            birdControlY -= 1.0f;
            break;
        case GLUT_KEY_LEFT:
            birdControlX -= 1.0f;
            break;
        case GLUT_KEY_RIGHT:
            birdControlX += 1.0f;
            break;
    }
    glutPostRedisplay();
}
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            boatDirection = 1;
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            boatDirection = 2;
        }
        else if (button == GLUT_MIDDLE_BUTTON) {
            boatDirection = 0;
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(80, 50);
    glutCreateWindow("WELCOME TO MY POOKIE VILLAGE");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}

