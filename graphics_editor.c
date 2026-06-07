#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 20
#define COLS 40
#define MAX_OBJECTS 50

char canvas[ROWS][COLS];

typedef enum { LINE, RECTANGLE, CIRCLE, TRIANGLE } ShapeType;

typedef struct {
    ShapeType type;
    int params[6];
} Shape;

Shape objects[MAX_OBJECTS];
int objectCount = 0;

// Initialize canvas
void initCanvas() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            canvas[i][j] = '_';
        }
    }
}

// Display canvas
void displayCanvas() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

// Draw line using DDA Algorithm
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    if (steps == 0) {
        if (x1 >= 0 && x1 < COLS && y1 >= 0 && y1 < ROWS)
            canvas[y1][x1] = '*';
        return;
    }

    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        if ((int)x >= 0 && (int)x < COLS &&
            (int)y >= 0 && (int)y < ROWS)
            canvas[(int)y][(int)x] = '*';

        x += xInc;
        y += yInc;
    }
}

// Draw rectangle
void drawRectangle(int x, int y, int w, int h) {

    for (int i = x; i < x + w; i++) {
        if (y >= 0 && y < ROWS && i >= 0 && i < COLS)
            canvas[y][i] = '*';

        if (y + h - 1 >= 0 && y + h - 1 < ROWS &&
            i >= 0 && i < COLS)
            canvas[y + h - 1][i] = '*';
    }

    for (int j = y; j < y + h; j++) {
        if (x >= 0 && x < COLS && j >= 0 && j < ROWS)
            canvas[j][x] = '*';

        if (x + w - 1 >= 0 && x + w - 1 < COLS &&
            j >= 0 && j < ROWS)
            canvas[j][x + w - 1] = '*';
    }
}

// Draw circle using Midpoint Circle Algorithm
void drawCircle(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (y >= x) {

        int points[8][2] = {
            {xc + x, yc + y},
            {xc - x, yc + y},
            {xc + x, yc - y},
            {xc - x, yc - y},
            {xc + y, yc + x},
            {xc - y, yc + x},
            {xc + y, yc - x},
            {xc - y, yc - x}
        };

        for (int i = 0; i < 8; i++) {
            int px = points[i][0];
            int py = points[i][1];

            if (px >= 0 && px < COLS &&
                py >= 0 && py < ROWS)
                canvas[py][px] = '*';
        }

        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

// Draw triangle
void drawTriangle(int x1, int y1,
                  int x2, int y2,
                  int x3, int y3) {

    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

// Redraw all objects
void redrawObjects() {
    initCanvas();

    for (int i = 0; i < objectCount; i++) {

        Shape s = objects[i];

        switch (s.type) {

            case LINE:
                drawLine(
                    s.params[0], s.params[1],
                    s.params[2], s.params[3]
                );
                break;

            case RECTANGLE:
                drawRectangle(
                    s.params[0], s.params[1],
                    s.params[2], s.params[3]
                );
                break;

            case CIRCLE:
                drawCircle(
                    s.params[0], s.params[1],
                    s.params[2]
                );
                break;

            case TRIANGLE:
                drawTriangle(
                    s.params[0], s.params[1],
                    s.params[2], s.params[3],
                    s.params[4], s.params[5]
                );
                break;
        }
    }
}

// Add object
void addObject(Shape s) {
    if (objectCount < MAX_OBJECTS) {
        objects[objectCount++] = s;
        redrawObjects();
    } else {
        printf("Object limit reached!\n");
    }
}

// Delete object
void deleteObject(int idx) {
    if (idx >= 0 && idx < objectCount) {

        for (int i = idx; i < objectCount - 1; i++) {
            objects[i] = objects[i + 1];
        }

        objectCount--;
        redrawObjects();
    }
}

// Modify object
void modifyObject(int idx, Shape newShape) {
    if (idx >= 0 && idx < objectCount) {
        objects[idx] = newShape;
        redrawObjects();
    }
}

int main() {

    int choice;

    initCanvas();

    while (1) {

        printf("\n===== ASCII GRAPHICS EDITOR =====\n");
        printf("1. Add Line\n");
        printf("2. Add Rectangle\n");
        printf("3. Add Circle\n");
        printf("4. Add Triangle\n");
        printf("5. Delete Object\n");
        printf("6. Modify Object\n");
        printf("7. Clear Canvas\n");
        printf("8. Display Canvas\n");
        printf("9. Exit\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        if (choice == 1) {

            Shape s;
            s.type = LINE;

            printf("Enter x1 y1 x2 y2: ");
            scanf("%d %d %d %d",
                  &s.params[0], &s.params[1],
                  &s.params[2], &s.params[3]);

            addObject(s);
        }

        else if (choice == 2) {

            Shape s;
            s.type = RECTANGLE;

            printf("Enter x y width height: ");
            scanf("%d %d %d %d",
                  &s.params[0], &s.params[1],
                  &s.params[2], &s.params[3]);

            addObject(s);
        }

        else if (choice == 3) {

            Shape s;
            s.type = CIRCLE;

            printf("Enter centerX centerY radius: ");
            scanf("%d %d %d",
                  &s.params[0],
                  &s.params[1],
                  &s.params[2]);

            addObject(s);
        }

        else if (choice == 4) {

            Shape s;
            s.type = TRIANGLE;

            printf("Enter x1 y1 x2 y2 x3 y3: ");

            scanf("%d %d %d %d %d %d",
                  &s.params[0], &s.params[1],
                  &s.params[2], &s.params[3],
                  &s.params[4], &s.params[5]);

            addObject(s);
        }

        else if (choice == 5) {

            int idx;

            printf("Enter object index (0-%d): ",
                   objectCount - 1);

            scanf("%d", &idx);

            deleteObject(idx);
        }

        else if (choice == 6) {

            int idx;

            printf("Enter object index (0-%d): ",
                   objectCount - 1);

            scanf("%d", &idx);

            if (idx >= 0 && idx < objectCount) {

                Shape s = objects[idx];

                if (s.type == LINE) {
                    printf("Enter new x1 y1 x2 y2: ");
                    scanf("%d %d %d %d",
                          &s.params[0], &s.params[1],
                          &s.params[2], &s.params[3]);
                }

                else if (s.type == RECTANGLE) {
                    printf("Enter new x y width height: ");
                    scanf("%d %d %d %d",
                          &s.params[0], &s.params[1],
                          &s.params[2], &s.params[3]);
                }

                else if (s.type == CIRCLE) {
                    printf("Enter new centerX centerY radius: ");
                    scanf("%d %d %d",
                          &s.params[0],
                          &s.params[1],
                          &s.params[2]);
                }

                else if (s.type == TRIANGLE) {
                    printf("Enter new x1 y1 x2 y2 x3 y3: ");
                    scanf("%d %d %d %d %d %d",
                          &s.params[0], &s.params[1],
                          &s.params[2], &s.params[3],
                          &s.params[4], &s.params[5]);
                }

                modifyObject(idx, s);
            }
        }

        else if (choice == 7) {
            objectCount = 0;
            initCanvas();
            printf("Canvas cleared.\n");
        }

        else if (choice == 8) {
            displayCanvas();
        }

        else if (choice == 9) {
            printf("Exiting...\n");
            break;
        }

        else {
            printf("Invalid choice!\n");
        }
    }

    return 0;
}