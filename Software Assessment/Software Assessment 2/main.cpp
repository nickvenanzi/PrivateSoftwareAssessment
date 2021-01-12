#ifdef _WIN32
#	include <windows.h> 
#endif

#include <GLUT/glut.h>
#include <iostream>

#include "MatrixUtils/Row.hpp"
#include "MatrixUtils/Matrix.hpp"
#include "CircleGenerator.hpp"
#include "EllipseGenerator.hpp"

std::shared_ptr<Grid> grid_ptr;
std::unique_ptr<CircleGenerator> circGen_ptr;
std::unique_ptr<EllipseGenerator> ellipseGen_ptr;
int window_width = 700;
std::unordered_set<int> pts;

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
    glRasterPos2f(-0.95f, -0.98f);
    std::string stringC("Generate Circle!");
    for (const char* c = stringC.c_str(); *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);  // Updates the position
    }

    glRasterPos2f(0.55f, -0.98f);
    std::string stringE("Generate Ellipse!");
    for (const char* c = stringE.c_str(); *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);  // Updates the position
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

//draw ellipse with given parameters
void drawEllipse(Coordinate center, float a, float b, float alpha) {
    int sections = 100;
    float pi = 3.141592654f;

    glBegin(GL_LINE_LOOP);

    glColor3f(0.f, 1.f, 0.f);
    for(int i = 0; i < sections; i++) {
        float phi = 2.0f * pi * float(i) / float(sections);
        float x = a*cosf(alpha)*cosf(phi) - b*sinf(alpha)*sinf(phi);
        float y = a*sinf(alpha)*cosf(phi) + b*cosf(alpha)*sinf(phi);
        glVertex2f(x + center.x, y + center.y);
    }
    glEnd();
}

void highlightGridPoints() {
    std::unordered_set<int>::iterator it = pts.begin();
    
    // Iterate through GridPoints to highlight
    while (it != pts.end()) {
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

    // if circle is generated, draw it
    if (circGen_ptr != nullptr) {
        Coordinate circle_c = {circGen_ptr->params[0], circGen_ptr->params[1]};
        drawCircle(circle_c, circGen_ptr->params[2], false);
    }
    // if ellipse is generated, draw it
    if (ellipseGen_ptr != nullptr) {
        Coordinate ellipse_c = {ellipseGen_ptr->params[0], ellipseGen_ptr->params[1]};
        float a = ellipseGen_ptr->params[2], b = ellipseGen_ptr->params[3];
        float alpha = ellipseGen_ptr->params[4];
        drawEllipse(ellipse_c, a, b, alpha);
    }
    for (auto iter = pts.begin(); iter != pts.end(); ++iter) {
        int index = *iter;
        int i = index % grid_ptr->width;
        int j = index / grid_ptr->height;
        GridPoint gp = grid_ptr->getPoint(i, j);
        drawSquare(gp.center, gp.sideLength, true);
    }
    glFlush();
}

void mouseClicked(int button, int state, int x, int y) {
    float normalized_x = -1.f + 2 * x/float(window_width - 1);
    float normalized_y = 1.f - 2 * y/float(window_width - 1);

    if (button != GLUT_LEFT_BUTTON) {
        return;
    }

    if (state == GLUT_DOWN) {
        //if over gridpoint or generate button, handle appropriately
 
        //if over generate button
        if (normalized_x < -0.55f && normalized_y < -0.92f && pts.size() >= 2) {
            circGen_ptr = std::unique_ptr<CircleGenerator>(new CircleGenerator(grid_ptr, pts));
        } else if (normalized_x > 0.55f && normalized_y < -0.92f && pts.size() >= 5) {
            ellipseGen_ptr = std::unique_ptr<EllipseGenerator>(new EllipseGenerator(grid_ptr, pts));
        } else {

            float x = (normalized_x + 1.f) * 21.f / 2.f;
            float y = (-normalized_y + 1.f) * 21.f / 2.f;

            int rounded_x = round(x);
            int rounded_y = round(y);

            float diff_x = abs(float(rounded_x) - x);
            float diff_y = abs(float(rounded_y) - y);

            if (diff_x > 0.25f || diff_y > 0.25f || rounded_x <= 0 || 
                rounded_y <= 0 || rounded_x > grid_ptr->width || rounded_y > grid_ptr->height) {
                //not over a square
                Coordinate center = {normalized_x, normalized_y};
                // drawSquare(center, 0.03f, false);
                return;
            } 
            int index = rounded_x - 1 + grid_ptr->width * (rounded_y - 1);
            if (pts.find(index) == pts.end()) {
                pts.insert(index);
            } else {
                pts.erase(index);
            }
        }
        
        display();
    }

    glFlush();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    Coordinate topLeftCorner = {-1.f, 1.f};
    int num_rows = 20, num_cols = 20;
    float side_length = 0.5f/float(num_rows + 1);
    grid_ptr = std::shared_ptr<Grid>(new Grid(topLeftCorner, num_cols, num_rows, 2.f/float(num_cols + 1), 2.f/float(num_rows + 1), side_length));

    glutInit(&argc, argv);                 // Initialize GLUT
    glutCreateWindow("Part 2"); // Create a window with the given title
    glutReshapeWindow(window_width, window_width);
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner

    glutDisplayFunc(display);
    glutMouseFunc(mouseClicked);
    glutMainLoop(); 

   return 0;
}
