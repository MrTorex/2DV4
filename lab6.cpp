#include <GL/glut.h>
#include <iostream>
#include <vector>

float shape_points[][2] = {
        {0, 20},
        {20, 20},
        {30, 40},
        {25, 10},
        {20, 0}
};

int vertex_count = sizeof(shape_points) / sizeof(shape_points[0]);

struct vec2
{
    float x, y;
};

std::vector<std::vector<vec2>> triangle_data;

void setup_render_state()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    gluOrtho2D(-50, 50, -50, 50);
}

void render_polygon()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.5, 0.5, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < vertex_count; i++)
    {
        glVertex2fv(shape_points[i]);
    }
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < vertex_count; i++)
    {
        glVertex2fv(shape_points[i]);
    }
    glEnd();

    glutSwapBuffers();
}

void render_triangles()
{
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& tri : triangle_data)
    {
        glColor3f(0.5, 0.5, 0);
        glBegin(GL_TRIANGLES);
        for (const auto& v : tri)
        {
            glVertex2f(v.x, v.y);
        }
        glEnd();

        glColor3f(0, 0, 0);
        glBegin(GL_LINE_LOOP);
        for (const auto& v : tri)
        {
            glVertex2f(v.x, v.y);
        }
        glEnd();
    }

    glutSwapBuffers();
}

bool is_shape_convex()
{
    int dir = 0;
    for (int i = 0; i < vertex_count; i++)
    {
        float dx1 = shape_points[(i + 1) % vertex_count][0] - shape_points[i][0];
        float dy1 = shape_points[(i + 1) % vertex_count][1] - shape_points[i][1];
        float dx2 = shape_points[(i + 2) % vertex_count][0] - shape_points[i][0];
        float dy2 = shape_points[(i + 2) % vertex_count][1] - shape_points[i][1];
        float cross = dx1 * dy2 - dy1 * dx2;

        if (cross != 0)
        {
            int sign = (cross > 0) ? 1 : -1;
            if (dir == 0)
            {
                dir = sign;
            }
            else if (dir != sign)
            {
                return false;
            }
        }
    }
    return true;
}

bool point_inside_triangle(const vec2& p, const vec2& a, const vec2& b, const vec2& c)
{
    float area = 0.5f * (-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y);
    float s = 1 / (2 * area) * (a.y * c.x - a.x * c.y + (c.y - a.y) * p.x + (a.x - c.x) * p.y);
    float t = 1 / (2 * area) * (a.x * b.y - a.y * b.x + (a.y - b.y) * p.x + (b.x - a.x) * p.y);
    return (s >= 0) && (t >= 0) && (1 - s - t >= 0);
}

bool is_ear(const std::vector<vec2>& poly, int idx)
{
    int n = static_cast<int>(poly.size());
    vec2 p0 = poly[(idx - 1 + n) % n];
    vec2 p1 = poly[idx];
    vec2 p2 = poly[(idx + 1) % n];

    float dx1 = p1.x - p0.x;
    float dy1 = p1.y - p0.y;
    float dx2 = p2.x - p1.x;
    float dy2 = p2.y - p1.y;
    float cross = dx1 * dy2 - dy1 * dx2;

    if (cross >= 0)
    {
        return false;
    }

    for (int i = 0; i < n; i++)
    {
        if (i == (idx - 1 + n) % n || i == idx || i == (idx + 1) % n)
        {
            continue;
        }
        if (point_inside_triangle(poly[i], p0, p1, p2))
        {
            return false;
        }
    }

    return true;
}

std::vector<std::vector<vec2>> triangulate(const std::vector<vec2>& poly)
{
    std::vector<std::vector<vec2>> tris;
    std::vector<vec2> temp = poly;
    int n = static_cast<int>(temp.size());

    while (n > 3)
    {
        bool clipped = false;
        for (int i = 0; i < n; i++)
        {
            if (is_ear(temp, i))
            {
                tris.push_back({ temp[(i - 1 + n) % n], temp[i], temp[(i + 1) % n] });
                temp.erase(temp.begin() + i);
                n--;
                clipped = true;
                break;
            }
        }
        if (!clipped)
        {
            break;
        }
    }

    if (n == 3)
    {
        tris.push_back({ temp[0], temp[1], temp[2] });
    }

    return tris;
}

int main(int argc, char** argv)
{
    std::vector<vec2> polygon;
    for (int i = 0; i < vertex_count; i++)
    {
        polygon.push_back({ shape_points[i][0], shape_points[i][1] });
    }

    bool convex = is_shape_convex();
    std::cout << "Polygon is " << (convex ? "convex" : "concave") << std::endl;

    if (!convex)
    {
        triangle_data = triangulate(polygon);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("lab6: polygon");

    setup_render_state();

    if (convex)
    {
        glutDisplayFunc(render_polygon);
    }
    else
    {
        glutDisplayFunc(render_triangles);
    }

    glutMainLoop();

    return 0;
}
