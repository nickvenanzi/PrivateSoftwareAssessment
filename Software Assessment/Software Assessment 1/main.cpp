#ifndef MAIN_H
#define MAIN_H
// #include <windows.h>  
// #include <GL/glut.h> 

// #include <OpenGL/gl.h>
// #include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>

// #include "Grid.hpp"
#include "DigitizedCircle.hpp"

std::shared_ptr<Grid> grid_ptr;
std::unique_ptr<DigitizedCircle> dgCircle;
int window_width = 700;
bool trackDrag = false;
Coordinate circle_center;
float circle_radius = 0.f;

// draw square on screen centered at 'center' with side length 'sideLength'
void drawSquare(Coordinate center, float sideLength, bool highlighted) {
    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      highlighted ? glColor3f(0.f, 0.f, 1.f) : glColor3f(0.5f, 0.5f, 0.5f);
      glVertex2f(center.x - sideLength/2.f, center.y - sideLength/2.f);    // x, y
      glVertex2f(center.x + sideLength/2.f, center.y - sideLength/2.f);
      glVertex2f(center.x + sideLength/2.f, center.y + sideLength/2.f);
      glVertex2f(center.x - sideLength/2.f, center.y + sideLength/2.f);
   glEnd();
}

// draws grid onto screen
void drawGrid() {
    for (int j = 0; j < grid_ptr->height; j++) {
        for (int i = 0; i < grid_ptr->width; i++) {
            GridPoint gp = grid_ptr->getPoint(i, j);
            drawSquare(gp.center, gp.sideLength, false);
        }
    }
}

//draws circle with given radius. Blue if true, else red
void drawCircle(Coordinate center, float radius, bool blue) {
    int sections = 100;
    float pi = 3.141592654f;

    glBegin(GL_LINE_LOOP);
    blue ? glColor3f(0.f, 0.f, 1.f) : glColor3f(1.f, 0.f, 0.f);
    for(int i = 0; i < sections; i++) {
        float angle = 2.0f * pi * float(i) / float(sections);
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex2f(x + center.x, y + center.y);
    }
    glEnd();
}

void highlightGridPoints() {
    std::unordered_set<int>::iterator it = dgCircle->highlighted_pts.begin();
    
    // Iterate through GridPoints to highlight
    while (it != dgCircle->highlighted_pts.end()) {
        int index = *it;
        int i = index % grid_ptr->width;
        int j = index / grid_ptr->height;

        GridPoint gp = grid_ptr->getPoint(i, j);
        drawSquare(gp.center, gp.sideLength, true);
        //Increment the iterator
        it++;
    }
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)

    //Draw grid
    drawGrid();

    if (dgCircle != nullptr) {
        drawCircle(dgCircle->center_, dgCircle->radius_, true);
        highlightGridPoints();
        drawCircle(dgCircle->center_, dgCircle->innerRadius, false);
        drawCircle(dgCircle->center_, dgCircle->outerRadius, false);
    } else if (trackDrag) {
        drawCircle(circle_center, circle_radius, true);
    }
    
    glFlush();  // Render now
}

void mouseClicked(int button, int state, int x, int y) {
    float normalized_x = -1.f + 2 * x/float(window_width - 1);
    float normalized_y = 1.f - 2 * y/float(window_width - 1);

    if (button != GLUT_LEFT_BUTTON) {
        return;
    }

    if (state == GLUT_DOWN) {
        //begin tracking circle to drag
        trackDrag = true;
        dgCircle = nullptr;
        circle_center.x = normalized_x;
        circle_center.y = normalized_y;
        circle_radius = 0.f;
        display();
    } else {
        trackDrag = false;
        Coordinate outer_point = {normalized_x, normalized_y};
        float radius = Coordinate::computeDistance(circle_center, outer_point);
        dgCircle = std::unique_ptr<DigitizedCircle>(new DigitizedCircle(grid_ptr, circle_center, radius));
        display();
    }

   glFlush();
}

void mouseDragged(int x, int y) {
    float normalized_x = -1.f + 2 * x/float(window_width - 1);
    float normalized_y = 1.f - 2 * y/float(window_width - 1);

    if (!trackDrag) {
        return;
    }

    Coordinate outer_point = {normalized_x, normalized_y};
    circle_radius = Coordinate::computeDistance(circle_center, outer_point);
    display();
}

void resize(int width, int height) {
    glutReshapeWindow(window_width, window_width);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    Coordinate topLeftCorner = {-1.f, 1.f};
    int num_rows = 20, num_cols = 20;
    float side_length = 0.5f/float(num_rows + 1);
    grid_ptr = std::shared_ptr<Grid>(new Grid(topLeftCorner, num_cols, num_rows, 2.f/float(num_cols + 1), 2.f/float(num_rows + 1), side_length));

    glutInit(&argc, argv);                 // Initialize GLUT
    glutCreateWindow("Part 1"); // Create a window with the given title
    glutReshapeWindow(window_width, window_width);
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutDisplayFunc(display);
    glutMouseFunc(mouseClicked);
    glutMotionFunc(mouseDragged);
    glutMainLoop();  

    // Coordinate c1 = {1.5f, 3.f};
    // Coordinate c2 = {2.f, 2.5f};
    // Coordinate dist = c1 - c2;
    // std::cout << dist.x << ", " << dist.y << std::endl;
    // GridPoint g1 = {c1, 1.f, 2.f};
    // GridPoint g2 = {c2, 1.f, 2.f};
    // std::cout << (g1 == g2) << std::endl;

   return 0;
}

#endif