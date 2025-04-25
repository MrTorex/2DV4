#include <GL/glut.h>

void setPixel(const int x, const int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void circlePlotPoints(const int xc, const int yc, const int x, const int y)
{
    setPixel(xc + x, yc + y);
    setPixel(xc - x, yc + y);
    setPixel(xc + x, yc - y);
    setPixel(xc - x, yc - y);
    setPixel(xc + y, yc + x);
    setPixel(xc - y, yc + x);
    setPixel(xc + y, yc - x);
    setPixel(xc - y, yc - x);
}

void circleMiddlePoint(const int xc, const int yc, const int r)
{
    int x = 0;
    int y = r;
    int p = 5/4 - r;

    circlePlotPoints(xc, yc, x, y);

    while (x < y)
    {
        x++;
        if (p < 0)
            p += 2 * x + 1;
        else
        {
            y--;
            p += 2 * x + 1 - 2 * y;
        }

        circlePlotPoints(xc, yc, x, y);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    circleMiddlePoint(4, 14, 13);

    glFlush();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-30, 30, -30, 30);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("lab4: Circle");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
