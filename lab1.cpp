#include <GL/glut.h>

void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 20.0, 0.0, 40.0);
}

void parallelogram()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_QUADS);
    glVertex2i(5, 5);
    glVertex2i(15, 5);
    glVertex2i(20, 35);
    glVertex2i(10, 35);
    glEnd();

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("lab1: Parallelogram");
    init();
    glutDisplayFunc(parallelogram);
    glutMainLoop();
    return 0;
}