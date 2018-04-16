#include "glut.h"
#include <vector>
#include <chrono>
#include <math.h>
#include <glm.hpp>
#define PI 3.14159265
#define GRAVITY 0.00000981
class Box
{
public:
    Box(float cx, float cy, float width, float height,bool staticObject);
    ~Box();
    float cx;
    float cy;

    float width;
    float height;
    float angle;

    bool staticObject;

    float v1[2];
    float v2[2];
    float v3[2];
    float v4[2];
    float velocity[2];
    float angularVelocity = 0.0f;
    float acceleration;
    float mass;
    float force = mass*acceleration;
    void Draw();
    void Update(double dt);
    void RotatePoint(float* v, float* _angle);
    void Rotate(float* _angle);
    void applyGravity(float* _angle,float dt);
    void collision(Box a, Box b);
};
Box::Box(float _cx, float _cy, float _width, float _height,bool _static)
{
    cx = _cx;
    cy = _cy;
    width = _width;
    height = _height;
    staticObject = _static;
    velocity[0] = 0;
    velocity[1] = 0;
    angularVelocity = 11;
    acceleration = 0;
    angle = 0.0f * PI / 180.f;
    v1[0] = cx - (width / 2.f);
    v1[1] = cy - (height / 2.f);
    v2[0] = cx + (width / 2.f);
    v2[1] = cy - (height / 2.f);
    v3[0] = cx + (width / 2.f);
    v3[1] = cy + (height / 2.f);
    v4[0] = cx - (width / 2.f);
    v4[1] = cy + (height / 2.f);
}
Box::~Box()
{

}
void Box::Draw()
{
    if (!staticObject) { glColor3f(0.9f, 0.2f, 1.2f); }
    else { glColor3f(0.5f, 0.2f, 1.f); }
    
    glBegin(GL_QUADS);
    glVertex2f(v1[0], v1[1]);
    glVertex2f(v2[0], v2[1]);
    glVertex2f(v3[0], v3[1]);
    glVertex2f(v4[0], v4[1]);
    glEnd();
}

void Box::RotatePoint(float* v, float* _angle)
{
    float s = sin(*_angle);
    float c = cos(*_angle);
    v[0] -= cx;
    v[1] -= cy;
    float xnew = v[0] * c - v[1] * s;
    float ynew = v[0] * s + v[1] * c;
    v[0] = xnew + cx;
    v[1] = ynew + cy;
}
void Box::Rotate(float* _angle)
{
    RotatePoint(v1, _angle);
    RotatePoint(v2, _angle);
    RotatePoint(v3, _angle);
    RotatePoint(v4, _angle);
    angle = 0.f;
}
void Box::applyGravity(float* _angle, float dt)
{
    velocity[1] -= GRAVITY*dt;
    cy += velocity[1];
    v1[1] += velocity[1];
    v2[1] += velocity[1];
    v3[1] += velocity[1];
    v4[1] += velocity[1];
}
void Box::collision(Box a, Box b)
{



}
void Box::Update(double dt)
{
    
    if (!staticObject)
    {
        Rotate(&angle);
      //  if (angularVelocity != 0) {angularVelocity -= 1 / 12 * mass*(width*width + height*height); };
        angle += angularVelocity * (PI / 180.f)*dt;
        //Rotate(&angle);
        applyGravity(&angle,dt);
    }
    Draw();
}

void changeSize(int w, int h)
{
    if (h == 0)
        h = 1;
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

std::vector<Box> boxes;
std::chrono::time_point<std::chrono::steady_clock> start;
using clock2 = std::chrono::high_resolution_clock;

void renderScene(void)
{
    auto duration = clock2::now() - start;
    double dt = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    start = clock2::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 10.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);

    if (boxes.size() < 1)
    {
        Box a(-1.f, 1.f, 4.f, 0.2f,true);
        Box b(-5.f, -3.f, 4.f, 0.2f,true);
        Box c(2.f, -2.f, 4.f, 0.2f,true);
        Box d(0.f, 3.f, 0.5f, 0.5f,false);
        boxes.push_back(a);
        boxes.push_back(b);
        boxes.push_back(c);
        boxes.push_back(d);
    }

    if (boxes.size() > 0)
    {
        for (unsigned int i = 0; i < boxes.size(); i++)
        {
            boxes[i].Update(dt);
        }
    }

    glutSwapBuffers();
}

int main(int argc, char **argv) {
    start = std::chrono::high_resolution_clock::now();
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Lighthouse3D- GLUT Tutorial");

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    // enter GLUT event processing cycle
    glutMainLoop();
    return 1;
}