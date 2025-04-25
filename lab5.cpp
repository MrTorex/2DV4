#include <GL/glut.h>
#include <cmath>

void plotEllipsePoints(int xc, int yc, int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glEnd();
}

void drawMidpointEllipse(int xc, int yc, int rx, int ry)
{
    int x = 0;
    int y = ry;

    double dx = 2 * pow(ry, 2) * x;
    double dy = 2 * pow(rx, 2) * y;
    double p1 = pow(ry, 2) - pow(rx, 2) * ry + 0.25 * pow(rx, 2);

    while (dx < dy)
    {
        plotEllipsePoints(xc, yc, x, y);
        x++;
        dx += 2 * pow(ry, 2);

        if (p1 < 0)
            p1 += pow(ry, 2) + dx;
        else
        {
            y--;
            dy -= 2 * pow(rx, 2);
            p1 += pow(ry, 2) + dx - dy;
        }
    }

    double p2 = pow(ry, 2) * (x + 0.5) * (x + 0.5)
                + pow(rx, 2) * (y - 1) * (y - 1)
                - pow(rx, 2) * pow(ry, 2);

    while (y >= 0)
    {
        plotEllipsePoints(xc, yc, x, y);
        y--;
        dy -= 2 * pow(rx, 2);

        if (p2 > 0)
            p2 += pow(rx, 2) - dy;
        else
        {
            x++;
            dx += 2 * pow(ry, 2);
            p2 += pow(rx, 2) - dy + dx;
        }
    }
}

void drawDerivedFigures(int xc, int yc, int rx, int ry)
{
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    const int numSegments = 100;
    for (int i = 0; i < numSegments; ++i)
    {
        float theta = 2.0f * static_cast<float>(M_PI) * static_cast<float>(i) / static_cast<float>(numSegments);
        float x = static_cast<float>(rx) * cosf(theta);
        float y = static_cast<float>(ry) * sinf(theta);
        glVertex2f(static_cast<float>(xc) + x, static_cast<float>(yc) + y);
    }
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2i(xc - rx, yc);
    glVertex2i(xc + rx, yc);
    glVertex2i(xc, yc - ry);
    glVertex2i(xc, yc + ry);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    int xc = 4;
    int yc = 4;
    int rx = 13;
    int ry = 6;

    drawDerivedFigures(xc, yc, rx, ry);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawMidpointEllipse(xc, yc, rx, ry);

    glFlush();
}

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);
    gluOrtho2D(-20, 20, -15, 15);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("lab5: Ellipse");
    initGL();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}