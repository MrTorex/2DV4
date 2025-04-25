#include <GL/glut.h>
#include <cmath>

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 100, 0, 100);
}

void set_pixel(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void line_cda(int x1, int y1, int x2, int y2) {
    auto dx = static_cast<float>(x2 - x1), dy = static_cast<float>(y2 - y1);
    float steps = std::max(abs(dx), abs(dy));
    auto x = static_cast<float>(x1), y = static_cast<float>(y1);
    float x_inc = dx / steps, y_inc = dy / steps;
    glColor3f(1.0, 0.8, 0.8);
    for (int i = 0; static_cast<float>(i) <= steps; i++)
    {
        set_pixel(static_cast<int>(round(x)), static_cast<int>(round(y)));
        x += x_inc;
        y += y_inc;
    }
}

void bresenham_line(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    glColor3f(0.7, 0.9, 1.0);
    while (true)
    {
        set_pixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void plotCirclePoints(int xc, int yc, int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
    glEnd();
}

void drawCircle(int xc, int yc, int r, float rCol, float gCol, float bCol)
{
    int x = 0, y = r;
    int d = 1 - r;
    glColor3f(rCol, gCol, bCol);
    while (x <= y) {
        plotCirclePoints(xc, yc, x, y);
        x++;
        if (d < 0) d += 2 * x + 1;
        else {
            y--;
            d += 2 * (x - y) + 1;
        }
    }
}

void plotEllipsePoints(int xc, int yc, int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glEnd();
}

void drawEllipse(int xc, int yc, int rx, int ry, float r, float g, float b) {
    int x = 0, y = ry;
    float dx = 0, dy = static_cast<float>(2 * rx * rx * y);
    float p1 = static_cast<float>(ry * ry) - static_cast<float>(rx * rx) * static_cast<float>(ry) + 0.25f * static_cast<float>(rx * rx);
    glColor3f(r, g, b);
    while (dx < dy) {
        plotEllipsePoints(xc, yc, x, y);
        x++;
        dx += static_cast<float>(2 * ry * ry);
        if (p1 < 0) p1 += dx + static_cast<float>(ry * ry);
        else {
            y--;
            dy -= static_cast<float>(2 * rx * rx);
            p1 += dx - dy + static_cast<float>(ry * ry);
        }
    }
    float p2 = static_cast<float>(ry * ry) * (static_cast<float>(x) + 0.5f) * (static_cast<float>(x) + 0.5f) + static_cast<float>(rx * rx)
            * static_cast<float>(y - 1) * static_cast<float>(y - 1) - static_cast<float>(rx * rx * ry * ry);
    while (y >= 0) {
        plotEllipsePoints(xc, yc, x, y);
        y--;
        dy -= static_cast<float>(2 * rx * rx);
        if (p2 > 0) p2 += static_cast<float>(rx * rx) - dy;
        else {
            x++;
            dx += static_cast<float>(2 * ry * ry);
            p2 += dx - dy + static_cast<float>(rx * rx);
        }
    }
}

void drawPolygon(bool concave, int offsetX, int offsetY) {
    glBegin(GL_POLYGON);
    if (!concave) {
        glColor3f(1.0, 1.0, 0.8);
        glVertex2i(offsetX, offsetY);
        glVertex2i(offsetX + 20, offsetY);
        glVertex2i(offsetX + 15, offsetY + 15);
        glVertex2i(offsetX + 5, offsetY + 15);
    } else {
        glColor3f(0.8, 1.0, 1.0);
        glVertex2i(offsetX, offsetY);
        glVertex2i(offsetX + 20, offsetY);
        glVertex2i(offsetX + 10, offsetY + 15);
        glVertex2i(offsetX + 5, offsetY + 5);
        glVertex2i(offsetX, offsetY + 15);
    }
    glEnd();
}

void drawHouse(int x, int y) {
    glColor3f(0.9, 0.6, 0.4);
    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + 15, y);
    glVertex2i(x + 15, y + 15);
    glVertex2i(x, y + 15);
    glEnd();
    glColor3f(0.6, 0.3, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2i(x, y + 15);
    glVertex2i(x + 7, y + 25);
    glVertex2i(x + 15, y + 15);
    glEnd();
}

void drawMan(int x, int y) {
    drawCircle(x, y + 20, 3, 1.0, 1.0, 1.0);
    line_cda(x, y + 17, x, y + 10);
    line_cda(x, y + 16, x - 3, y + 12);
    line_cda(x, y + 16, x + 3, y + 12);
    line_cda(x, y + 10, x - 2, y + 5);
    line_cda(x, y + 10, x + 2, y + 5);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawCircle(0, 95, 5, 1.0, 1.0, 0.0);
    drawCircle(0, 95, 10, 1.0, 0.5, 0.0);
    drawEllipse(25, 80, 5, 10, 0.7, 0.8, 1.0);
    drawEllipse(75, 70, 10, 5, 0.8, 1.0, 0.9);
    drawPolygon(false, 10, 10);
    drawPolygon(true, 70, 10);
    line_cda(10, 5, 90, 5);
    bresenham_line(10, 95, 90, 95);
    drawMan(50, 20);
    drawHouse(20, 40);
    drawHouse(60, 40);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("lab7: Le Finalle");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}