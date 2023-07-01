#include<windows.h>
#include<gl\gl.h>
#include<gl\glu.h>
#include<gl\glut.h>
#include<stdio.h>
#include<math.h>

typedef enum { false, true } bool;
int width = 1524;
int height = 780;
GLfloat r = 0.0, g = 0.0, b = 0.0;
int brushSize = 5;
bool isDrawingTriangle = false;
int triangleX1, triangleY1, triangleX2, triangleY2, triangleX3, triangleY3;
bool isDrawingSquare = false;
int squareX1, squareY1, squareX2, squareY2;
bool isDrawingCircle = false;
int circleX, circleY;
int circleRadius = 0;
bool isDrawing = false;
int startX, startY;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void motion(int x, int y)
{
    if (isDrawing)
    {
        int cx = x - width / 2;
        int cy = height / 2 - y;
        glColor3f(r, g, b);
        glLineWidth(brushSize);
        glBegin(GL_POINTS);
        glVertex2i(startX, startY);
        glVertex2i(cx, cy);
        startX = cx;
        startY = cy;
        glEnd();
        glFlush();
    }
}

void reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
    static int firstClick = 1;
    static int startX, startY;
    int cx = x - width / 2;
    int cy = height / 2 - y;
    circleX = cx;
    circleY = cy;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        glColor3f(r, g, b);
        glPointSize(brushSize);
        glBegin(GL_POINTS);
        glVertex2i(cx, cy);
        isDrawing = true;
        startX = cx;
        startY = cy;
        glEnd();
        glFlush();
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        isDrawing = false;
        glColor3f(r, g, b);
        glLineWidth(brushSize);
        glBegin(GL_LINES);
        glVertex2i(cx, cy);
        glEnd();
        glFlush();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        if (firstClick)
        {
            if (isDrawingTriangle && firstClick < 2) {
                triangleX1 = cx;
                triangleY1 = cy;
                firstClick++;
            }
            else if (isDrawingSquare) {
                squareX1 = cx;
                squareY1 = cy;
                firstClick = 0;
            }
            else if (isDrawingCircle) {
                startX = cx;
                startY = cy;
                firstClick = 0;
            }
            else {
                startX = cx;
                startY = cy;
                firstClick = 0;
            }
        }
        else
        {
            glColor3f(r, g, b);
            glLineWidth(brushSize);
            if (isDrawingTriangle) {
                triangleX2 = cx;
                triangleY2 = cy;
                triangleX3 = startX;
                triangleY3 = startY;
                glBegin(GL_TRIANGLES);
                glVertex2i(triangleX1, triangleY1);
                glVertex2i(triangleX2, triangleY2);
                glVertex2i(triangleX3, triangleY3);
                glEnd();
                glFlush();
                firstClick = 1;
            }
            else if (isDrawingSquare) {
                squareX2 = cx;
                squareY2 = cy;
                glBegin(GL_QUADS);
                glVertex2i(squareX1, squareY1);
                glVertex2i(squareX1, squareY2);
                glVertex2i(squareX2, squareY2);
                glVertex2i(squareX2, squareY1);
                glEnd();
                glFlush();
                firstClick = 1;
            }
            else if (isDrawingCircle) {
            if (firstClick) {
                startX = cx;
                startY = cy;
                firstClick = 0;
            } else {
                int radius = sqrt(pow(cx - startX, 2) + pow(cy - startY, 2));
                int x = 0;
                int y = radius;
                int d = 3 - 2 * radius;
                glBegin(GL_TRIANGLE_STRIP);
                while (x <= y) {
                    glVertex2i(startX + x, startY + y);
                    glVertex2i(startX + x, startY - y);
                    glVertex2i(startX - x, startY + y);
                    glVertex2i(startX - x, startY - y);
                    glVertex2i(startX + y, startY + x);
                    glVertex2i(startX + y, startY - x);
                    glVertex2i(startX - y, startY + x);
                    glVertex2i(startX - y, startY - x);
                    x++;
                    if (d < 0) {
                        d += 4 * x + 6;
                    } else {
                        d += 4 * (x - y) + 10;
                        y--;
                    }
                }
                glEnd();
                glFlush();
                firstClick = 1;
                }
            } else {
                glBegin(GL_LINES);
                glVertex2i(startX, startY);
                glVertex2i(cx, cy);
                glEnd();
                glFlush();
                firstClick = 1;
            }
        }
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        r = 1.0;
        g = 0.0;
        b = 0.0;
        break;
    case 'g':
        r = 0.0;
        g = 1.0;
        b = 0.0;
        break;
    case 'b':
        r = 0.0;
        g = 0.0;
        b = 1.0;
        break;
    case 'w':
        r = 1.0;
        g = 1.0;
        b = 1.0;
        break;
    case 'd':
        r = 0.0;
        g = 0.0;
        b = 0.0;
        break;
    case '+':
        brushSize+=2;
        break;
    case '-':
        brushSize-=2;
        if (brushSize < 2) brushSize = 2;
        break;
    case '3':
        isDrawingTriangle = true;
        isDrawingSquare = false;
        isDrawingCircle = false;
        break;
    case '4':
        isDrawingSquare = true;
        isDrawingTriangle = false;
        isDrawingCircle = false;
        break;
    case '0':
        isDrawingCircle = true;
        isDrawingTriangle = false;
        isDrawingSquare = false;
        break;
    case '.':
        isDrawingTriangle = false;
        isDrawingSquare = false;
        isDrawingCircle = false;
        isDrawing = false;
        break;
    }
}

void menu(int choice)
{
    switch(choice)
    {
        case 1:
            glColor3f(0.0, 0.0, 0.0);
            glLineWidth(2.0);
            glBegin(GL_LINE_LOOP);
            float radius = 50;
            float x = 0.0, y = 0.0;
            for(int i = 0; i < 360; i++)
            {
                x = radius * cos(i * M_PI/180) + circleX;
                y = radius * sin(i * M_PI/180) + circleY;
                glVertex2f(x, y);
            }
            glEnd();
            glFlush();
            break;
        case 2:
            glColor3f(0.0, 0.0, 0.0);
            glLineWidth(2.0);
            glBegin(GL_LINE_LOOP);
            glVertex2f(-50.0, -50.0);
            glVertex2f(50.0, -50.0);
            glVertex2f(0.0, 50.0);
            glEnd();
            glFlush();
            break;
        default:
            break;
    }
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-width / 2, width / 2, -height / 2, height / 2);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Project Team Unique");
    glutDisplayFunc(display);
    glutCreateMenu(menu);
    glutAddMenuEntry("Draw Circle", 1);
    glutAddMenuEntry("Draw Triangular", 2);
    glutAttachMenu(GLUT_MIDDLE_BUTTON);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    return 0;
}
