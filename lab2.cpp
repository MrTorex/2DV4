#include <GL/glut.h>
#include <cstdlib>
#include <cmath>

void init()
{
    glClearColor(0, 0, 0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 20.0, 0.0, 20.0);
}

void set_pixel(const int x, const int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

int round_k(const float a)
{
    if (a < 0)
		return  static_cast<int>(a - 0.5);
    return  static_cast<int>(a + 0.5);
}

void line_cda(const int x0, const int y0, const int xend, const int yend)
{
	const int dx = xend - x0;
	const int dy = yend - y0;

	float x = static_cast<float>(x0), y = static_cast<float>(y0);

	const int step = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	const float x_inc = static_cast<float>(dx) / static_cast<float>(step);
	const float y_inc = static_cast<float>(dy) / static_cast<float>(step);

	set_pixel(round_k(x), round_k(y));

	for (int k = 0; k < step; ++k)
    {
        x += x_inc;
        y += y_inc;
        set_pixel(round_k(x), round_k(y));
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    line_cda(4, 14, 8, 8);
    glBegin(GL_LINES);
    glVertex2i(4, 14);
    glVertex2i(8, 8);
    glEnd();
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("lab2: CDA line");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}