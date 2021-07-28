#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
#include <iostream>
#include <math.h>
#include <cmath>
void sizeChange(int w,int h);
void display();
void drawBase();
void drawFan();
void drawWheel();
void drawStool();
void drawLegs();
void key(unsigned char key, int x, int y);
void specialFunc(int key, int x, int y);
void timer(int x);
struct point {
float x,y,z;
};

struct circle {
circle(int seg = 20)
{
    float angle = (2 * 3.1415f) / seg;
    int i=0;
    while(i<seg){
        float xtheta = cos(i*angle);
        float ytheta = sin(i*angle);
        points.push_back(point{xtheta,ytheta,0});
        i++;
    }
}
std::vector<point> points;
};

struct Camera{
Camera(){
x=0.0f;y=0.0f;z=0.0f;
anglex=0.0f;angley=0.0f;anglez=0.0f;
}
float x,y,z,anglex,angley,anglez;
}camera;

struct Fan{
    Fan()
    {
        x=0.0f;y=2.5f;z=-30.0f;
        latSpeed=0.0f;fanSpeed=0.0f;
        lateral = false;
        on = false;
        point centre = {0, 0};

		point p1 = { 3 * (float)std::cos((150.0f + 15.0f) * 3.1415 / 180.0f),
				3 * (float)std::sin((150.0f + 15.0f) * 3.1415 / 180.0f) };

		point p2 = { 3 * (float)std::cos((150.0f - 15.0f) * 3.1415 / 180.0f),
				3 * (float)std::sin((150.0f - 15.0f) * 3.1415 / 180.0f) };

		point p3 = { 3 * (float)std::cos((30.0f + 15.0f) * 3.1415 / 180.0f),
				3 * (float)std::sin((30.0f + 15.0f) * 3.1415 / 180.0f) };

		point p4 = { 3 * (float)std::cos((30.0f - 15.0f) * 3.1415 / 180.0f),
				3 * (float)std::sin((30.0f - 15.0f) * 3.1415 / 180.0f) };

        point p5 = { 3 * (float)std::cos((270.0f - 15.0f) * 3.1415 / 180.0f),
				3 * (float)std::sin((270.0f - 15.0f) * 3.1415 / 180.0f) };

		point p6 = { 3 * (float)std::cos((270.0f + 15.0f) * 3.1415 / 180.0f),
				3 * (float)std::sin((270.0f + 15.0f) * 3.1415 / 180.0f) };

      /*  wheel.push_back(centre);
        wheel.push_back(p1);
        wheel.push_back(centre);
        wheel.push_back(p2);
        wheel.push_back(centre);
        wheel.push_back(p3);
        wheel.push_back(centre);
        wheel.push_back(p4);
        wheel.push_back(centre);
        wheel.push_back(p5);
        wheel.push_back(centre);
        wheel.push_back(p6);
        wheel.push_back(p1);
        wheel.push_back(p2);
        wheel.push_back(p3);
        wheel.push_back(p4);
        wheel.push_back(p5);
        wheel.push_back(p6); */
        wheel.push_back(p1);
        wheel.push_back(p2);
        wheel.push_back(centre);
        wheel.push_back(centre);
        wheel.push_back(p3);
        wheel.push_back(p4);
        wheel.push_back(p5);
        wheel.push_back(p6);
        wheel.push_back(centre);
    }
float x,y,z,latSpeed,fanSpeed;
bool lateral,on;
std::vector<point> wheel;
}fan;

void drawCube(point centre, float l, float b, float h);
void drawLine(const std::vector<point>& points);
void drawCircle(float r);

int main(int argc, char** argv){
glutInit(&argc,argv);
glutInitWindowSize(640,480);
glutInitWindowPosition(10,10);
glutInitDisplayMode(GLUT_DOUBLE);
glutCreateWindow("Assignment");
glutReshapeFunc(sizeChange);
glutDisplayFunc(display);
glutKeyboardFunc(key);
glutSpecialFunc(specialFunc);
glClearColor(0.0f,0.0f,0.0f,1.0f);
glClearDepth(1.0f);
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LEQUAL);      //To see only the front objects and not the back ones (Visible Surface Determination).
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0f,640.0f/480.0f,0.01f,100.0f);
glutTimerFunc(10,timer,1);
glutMainLoop();
return 0;
}

void sizeChange(int w,int h){
const float ar = (float) w / (float) h;
glViewport(0,0,w,h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0f,ar,0.01f,100.0f);
}

void display(){
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glRotatef(camera.angley,0.0f,1.0f,0.0f);  // Rotation of the reference point along the Y-Axis
glRotatef(camera.anglex,1.0f,0.0f,0.0f);  // Rotation of the reference point along the X-Axis
glRotatef(camera.anglez,0.0f,0.0f,1.0f);  // Rotation of the reference point along the Z-Axis
glTranslatef(0.0f-camera.x,1.0f-camera.y,-7.0f-camera.z);    //Reference Point of the room.
glPushMatrix();
glTranslatef(fan.x,fan.y,fan.z);
glColor3f(0.75f,0.5f,0.5f);
drawBase();
//Back Wall
glColor3f(0.55f,0.55f,0.55f);
glTranslatef(0.0f,0.0f,fan.z-0.3f);
drawCube(point{0,0,0},20.0f,20.0f,0.0f);
//Left Wall
glColor3f(0.2f,0.2f,0.2f);
glTranslatef(-10.0f,0.0f,10.0f);
drawCube(point{0,0,0},0.0f,20.0f,20.0f);
//Right Wall
glColor3f(0.2f,0.2f,0.2f);
glTranslatef(20.0f,0.0f,0.0f);
drawCube(point{0,0,0},0.0f,20.0f,20.0f);
//Ceiling
glColor3f(0.4f,0.4f,0.4f);
glTranslatef(-10.0f,10.0f,0.0f);
drawCube(point{0,0,0},20.0f,0.0f,20.0f);
//Ground
glColor3f(0.4f,0.4f,0.4f);
glTranslatef(0.0f,-20.0f,0.0f);
drawCube(point{0,0,0},20.0f,0.0f,20.0f);
//Stool
glTranslatef(0.0f,5.0f,4.0f);
drawStool();
glutSwapBuffers();
}

void drawBase(){
glColor3f(1.0f,1.0f,1.0f);
drawCube(point{0,0,0},2.0f,3.0f,1.0f);
glColor3f(0,0,0);
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
drawCube(point{0,0,0},2.0f,3.0f,1.0f);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
drawFan();
glPopMatrix();
}

void drawFan(){
glPushMatrix();
glTranslatef(0.0f,2.0f,1.0f);
glRotatef(45,1.0f,0.0f,0.0f);
glRotatef(-fan.latSpeed,0.0f,1.0f,0.0f);
glColor3f(0.85f,0.85f,0.85f);
drawCube(point{0.0f,1.5f,0.0f},0.75f,4.0f,0.75f);
glColor3f(0.0f,0.0f,0.0f);
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
drawCube(point{0.0f,1.5f,0.0f},0.75f,4.0f,0.75f);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
glTranslatef(0.0f,3.5f,0.0f);
glColor3f(0.0f,0.0f,0.0f);
drawCircle(3.5f);
drawWheel();
glTranslatef(0.0f,0.0f,0.5f);
glColor3f(0.0f, 0.0f, 0.0f);
drawCircle(3.5f);
drawWheel();
glPopMatrix();
}

void drawWheel(){
glPushMatrix();
glColor3f(0.0f,0.0f,0.0f);
glRotatef(fan.fanSpeed,0.0f,0.0f,1.0f);
drawLine(fan.wheel);
glPopMatrix();
}

void key(unsigned char key, int x, int y){
switch(key){
case 'q':
    exit(0);
    break;
case 's':
    fan.on=true;
    break;
case 't':
    fan.on=false;
    break;
case 'h':
    fan.lateral=true;
    break;
case 'o':
    fan.lateral=false;
    break;
case 'u':
    fan.y+=1.0f;
    break;
case 'd':
    fan.y-=1.0f;
    break;
case 'l':
    fan.x-=1.0f;
    break;
case 'r':
    fan.x+=1.0f;
    break;
case '2':
    camera.anglex+=15.0f;
    break;
case '6':
    camera.angley+=15.0f;
    break;
case '8':
    camera.anglex-=15.0f;
    break;
case '4':
    camera.angley-=15.0f;
    break;
case '+':
    camera.anglez+=15.0f;
    break;
case '-':
    camera.anglez-=15.0f;
    break;
}
}

void timer(int x){
static float increase = 0.5f;
static float rot = 4.5f;
if(fan.lateral == true) fan.latSpeed+=increase;
if(fan.latSpeed>45){
    fan.latSpeed=45;
    increase *=-1;
}
else if(fan.latSpeed<-45){
    fan.latSpeed=-45;
    increase *=-1;
}
if(fan.on==true) fan.fanSpeed+=rot;
glutPostRedisplay();
glutTimerFunc(10,timer,1);
}

void drawStool(){
glColor3f(0.58f,0.29f,0.0f);
drawCube(point{0,0,0},5.0f,1.0f,5.0f);
glColor3f(0.0f,0.0f,0.0f);
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
drawCube(point{0,0,0},5.0f,1.0f,5.0f);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
drawLegs();
}

void drawLegs(){
glColor3f(0.48f,0.19f,0.0f);
//Left-Front Leg
glTranslatef(-2.0f,-3.0f,2.0f);
drawCube(point{0,0,0},1.0f,5.0f,1.0f);
glColor3f(0.0f,0.0f,0.0f);
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
drawCube(point{0,0,0},1.0f,5.0f,1.0f);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//Right-Front Leg
glTranslatef(4.0f,0.0f,0.0f);
glColor3f(0.48f,0.19f,0.0f);
drawCube(point{0,0,0},1.0f,5.0f,1.0f);
glColor3f(0.0f,0.0f,0.0f);
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
drawCube(point{0,0,0},1.0f,5.0f,1.0f);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//Right-Back Leg
glTranslatef(0.0f,0.0f,-4.0f);
glColor3f(0.48f,0.19f,0.0f);
drawCube(point{0,0,0},1.0f,5.0f,1.0f);
glColor3f(0.0f,0.0f,0.0f);
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
drawCube(point{0,0,0},1.0f,5.0f,1.0f);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//Left-Back Leg
glTranslatef(-4.0f,0.0f,0.0f);
glColor3f(0.48f,0.19f,0.0f);
drawCube(point{0,0,0},1.0f,5.0f,1.0f);
glColor3f(0.0f,0.0f,0.0f);
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
drawCube(point{0,0,0},1.0f,5.0f,1.0f);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawCube(point centre, float l, float b, float h)
{
    float lm = l/2.0f;  //Mid point distances
    float bm = b/2.0f;
    float hm = h/2.0f;

    glBegin(GL_QUADS);
    //Front Face
    glVertex3f(-lm,bm,hm);
    glVertex3f(-lm,-bm,hm);
    glVertex3f(lm,-bm,hm);
    glVertex3f(lm,bm,hm);
    //Left Face
    glVertex3f(-lm,bm,-hm);
    glVertex3f(-lm,-bm,-hm);
    glVertex3f(-lm,-bm,hm);
    glVertex3f(-lm,bm,hm);
    //Back Face
    glVertex3f(-lm,bm,-hm);
    glVertex3f(lm,bm,-hm);
    glVertex3f(lm,-bm,-hm);
    glVertex3f(-lm,-bm,-hm);
    //Right Face
    glVertex3f(lm,bm,hm);
    glVertex3f(lm,-bm,hm);
    glVertex3f(lm,-bm,-hm);
    glVertex3f(lm,bm,-hm);
    //Top Face
    glVertex3f(-lm,bm,-hm);
    glVertex3f(-lm,bm,hm);
    glVertex3f(lm,bm,hm);
    glVertex3f(lm,bm,-hm);
    //Bottom Face
    glVertex3f(-lm,-bm,-hm);
    glVertex3f(lm,-bm,-hm);
    glVertex3f(lm,-bm,hm);
    glVertex3f(-lm,-bm,hm);
    glEnd();
}

void drawLine(const std::vector<point>& points) {
glBegin(GL_TRIANGLES);
for (const point& p : points){
    glVertex3f(p.x,p.y,p.z);
}
glEnd();
}

void drawCircle(float r){
static circle c(45);
glBegin(GL_LINE_LOOP);
for(const point& p : c.points){
    glVertex3f(p.x*r,p.y*r,0);
}
glEnd();
}

void specialFunc(int key, int x, int y){
switch(key){
case GLUT_KEY_UP:
    camera.z-=0.2f;
    break;
case GLUT_KEY_DOWN:
    camera.z+=0.2f;
    break;
case GLUT_KEY_LEFT:
    camera.x-=0.2f;
    break;
case GLUT_KEY_RIGHT:
    camera.x+=0.2f;
    break;
}
}
