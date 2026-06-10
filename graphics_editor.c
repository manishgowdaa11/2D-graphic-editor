#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* ==================== CONSTANTS ==================== */
#define CANVAS_WIDTH 80
#define CANVAS_HEIGHT 25
#define MAX_OBJECTS 100
#define MAX_HISTORY 50
#define EMPTY_CELL '_'
#define DRAW_CHAR '*'

/* ==================== ENUMERATIONS ==================== */
typedef enum {
    LINE,
    RECTANGLE,
    TRIANGLE,
    CIRCLE
} ShapeType;

/* ==================== STRUCTURES ==================== */
typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int id;
    ShapeType type;
    Point p1;           // Start point or center
    Point p2;           // End point or for rectangle
    Point p3;           // For triangle
    int radius;         // For circle
    int visible;
    char description[100];
} GraphicObject;

typedef struct {
    char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
    GraphicObject objects[MAX_OBJECTS];
    int objectCount;
    int nextObjectId;
} Canvas;

/* ==================== GLOBAL VARIABLES ==================== */
Canvas mainCanvas;
Canvas history[MAX_HISTORY];
int historyIndex = 0;
int historySize = 0;

/* ==================== FUNCTION PROTOTYPES ==================== */
// Canvas Functions
void initializeCanvas();
void clearCanvas();
void displayCanvas();
void saveCanvasToHistory();
void undoOperation();

// Drawing Functions
void drawPixel(int x, int y, char ch);
void drawLine(int x1, int y1, int x2, int y2);
void drawRectangle(int x1, int y1, int x2, int y2);
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
void drawCircle(int cx, int cy, int radius);

// Bresenham's Line Algorithm
void bresenhamLine(int x1, int y1, int x2, int y2);

// Midpoint Circle Algorithm
void midpointCircle(int cx, int cy, int radius);

// Object Management
void addObject(ShapeType type, Point p1, Point p2, Point p3, int radius, const char* desc);
void deleteObject(int objectId);
void modifyObject(int objectId);
void listObjects();
void redrawCanvas();

// Utility Functions
int isValidCoordinate(int x, int y);
int getObjectIndex(int objectId);
void drawLineInteractive();
void drawRectangleInteractive();
void drawTriangleInteractive();
void drawCircleInteractive();
void deleteObjectInteractive();
void modifyObjectInteractive();
void saveCanvasToFile();
void loadCanvasFromFile();

// Menu Function
void displayMenu();
int getUserChoice();

/* ==================== CANVAS FUNCTIONS ==================== */

/**
 * Initialize canvas with empty cells
 * Time Complexity: O(H*W) where H=height, W=width
 * Space Complexity: O(1)
 */
void initializeCanvas() {
    int i, j;
    for (i = 0; i < CANVAS_HEIGHT; i++) {
        for (j = 0; j < CANVAS_WIDTH; j++) {
            mainCanvas.canvas[i][j] = EMPTY_CELL;
        }
    }
    mainCanvas.objectCount = 0;
    mainCanvas.nextObjectId = 1;
}

/**
 * Clear the canvas (redraw empty)
 * Time Complexity: O(H*W)
 * Space Complexity: O(1)
 */
void clearCanvas() {
    int i, j;
    for (i = 0; i < CANVAS_HEIGHT; i++) {
        for (j = 0; j < CANVAS_WIDTH; j++) {
            mainCanvas.canvas[i][j] = EMPTY_CELL;
        }
    }
    mainCanvas.objectCount = 0;
    mainCanvas.nextObjectId = 1;
    printf("\n✓ Canvas cleared successfully!\n");
}

/**
 * Display the canvas with borders
 * Time Complexity: O(H*W)
 * Space Complexity: O(1)
 */
void displayCanvas() {
    int i, j;
    
    printf("\n");
    printf("╔");
    for (j = 0; j < CANVAS_WIDTH; j++) printf("═");
    printf("╗\n");
    
    for (i = 0; i < CANVAS_HEIGHT; i++) {
        printf("║");
        for (j = 0; j < CANVAS_WIDTH; j++) {
            printf("%c", mainCanvas.canvas[i][j]);
        }
        printf("║ %2d\n", i);
    }
    
    printf("╚");
    for (j = 0; j < CANVAS_WIDTH; j++) printf("═");
    printf("╝\n");
    
    printf("  ");
    for (j = 0; j < CANVAS_WIDTH; j++) {
        if (j % 10 == 0) printf("%d", (j / 10) % 10);
        else printf(" ");
    }
    printf("\n");
    
    printf("  ");
    for (j = 0; j < CANVAS_WIDTH; j++) printf("%d", j % 10);
    printf("\n\n");
}

/**
 * Validate if coordinate is within canvas bounds
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 */
int isValidCoordinate(int x, int y) {
    return (x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT);
}

/**
 * Draw a pixel on the canvas
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 */
void drawPixel(int x, int y, char ch) {
    if (isValidCoordinate(x, y)) {
        mainCanvas.canvas[y][x] = ch;
    }
}

/* ==================== DRAWING ALGORITHMS ==================== */

/**
 * Bresenham's Line Algorithm
 * Time Complexity: O(max(|x2-x1|, |y2-y1|))
 * Space Complexity: O(1)
 */
void bresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int x = x1, y = y1;
    
    while (1) {
        drawPixel(x, y, DRAW_CHAR);
        
        if (x == x2 && y == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

/**
 * Midpoint Circle Algorithm
 * Time Complexity: O(8*radius) = O(radius)
 * Space Complexity: O(1)
 */
void midpointCircle(int cx, int cy, int radius) {
    int x = radius;
    int y = 0;
    int d = 3 - 2 * radius;
    
    while (x >= y) {
        // Draw 8 octants
        drawPixel(cx + x, cy + y, DRAW_CHAR);
        drawPixel(cx - x, cy + y, DRAW_CHAR);
        drawPixel(cx + x, cy - y, DRAW_CHAR);
        drawPixel(cx - x, cy - y, DRAW_CHAR);
        drawPixel(cx + y, cy + x, DRAW_CHAR);
        drawPixel(cx - y, cy + x, DRAW_CHAR);
        drawPixel(cx + y, cy - x, DRAW_CHAR);
        drawPixel(cx - y, cy - x, DRAW_CHAR);
        
        if (d < 0) {
            d = d + 4 * y + 6;
        } else {
            d = d + 4 * (y - x) + 10;
            x--;
        }
        y++;
    }
}

/**
 * Draw line using Bresenham's algorithm
 */
void drawLine(int x1, int y1, int x2, int y2) {
    if (!isValidCoordinate(x1, y1) || !isValidCoordinate(x2, y2)) {
        printf("✗ Line coordinates out of bounds!\n");
        return;
    }
    bresenhamLine(x1, y1, x2, y2);
    printf("✓ Line drawn from (%d, %d) to (%d, %d)\n", x1, y1, x2, y2);
}

/**
 * Draw rectangle using 4 lines
 * Time Complexity: O(max(width, height))
 */
void drawRectangle(int x1, int y1, int x2, int y2) {
    if (!isValidCoordinate(x1, y1) || !isValidCoordinate(x2, y2)) {
        printf("✗ Rectangle coordinates out of bounds!\n");
        return;
    }
    
    // Ensure x1 <= x2 and y1 <= y2
    int minX = (x1 < x2) ? x1 : x2;
    int maxX = (x1 < x2) ? x2 : x1;
    int minY = (y1 < y2) ? y1 : y2;
    int maxY = (y1 < y2) ? y2 : y1;
    
    // Draw 4 sides
    bresenhamLine(minX, minY, maxX, minY); // Top
    bresenhamLine(maxX, minY, maxX, maxY); // Right
    bresenhamLine(maxX, maxY, minX, maxY); // Bottom
    bresenhamLine(minX, maxY, minX, minY); // Left
    
    printf("✓ Rectangle drawn from (%d, %d) to (%d, %d)\n", x1, y1, x2, y2);
}

/**
 * Draw triangle using 3 lines
 * Time Complexity: O(perimeter)
 */
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    if (!isValidCoordinate(x1, y1) || !isValidCoordinate(x2, y2) || !isValidCoordinate(x3, y3)) {
        printf("✗ Triangle coordinates out of bounds!\n");
        return;
    }
    
    // Draw 3 sides
    bresenhamLine(x1, y1, x2, y2);
    bresenhamLine(x2, y2, x3, y3);
    bresenhamLine(x3, y3, x1, y1);
    
    printf("✓ Triangle drawn with vertices at (%d, %d), (%d, %d), (%d, %d)\n", 
           x1, y1, x2, y2, x3, y3);
}

/**
 * Draw circle using Midpoint Circle Algorithm
 */
void drawCircle(int cx, int cy, int radius) {
    if (!isValidCoordinate(cx, cy)) {
        printf("✗ Circle center out of bounds!\n");
        return;
    }
    
    if (radius <= 0) {
        printf("✗ Invalid radius!\n");
        return;
    }
    
    midpointCircle(cx, cy, radius);
    printf("✓ Circle drawn at center (%d, %d) with radius %d\n", cx, cy, radius);
}

/* ==================== OBJECT MANAGEMENT ==================== */

/**
 * Find object index by ID
 * Time Complexity: O(n) where n = number of objects
 */
int getObjectIndex(int objectId) {
    int i;
    for (i = 0; i < mainCanvas.objectCount; i++) {
        if (mainCanvas.objects[i].id == objectId) {
            return i;
        }
    }
    return -1;
}

/**
 * Add object to the object list and draw it
 * Time Complexity: O(1) for adding, O(perimeter) for drawing
 */
void addObject(ShapeType type, Point p1, Point p2, Point p3, int radius, const char* desc) {
    if (mainCanvas.objectCount >= MAX_OBJECTS) {
        printf("✗ Maximum objects limit reached!\n");
        return;
    }
    
    GraphicObject obj;
    obj.id = mainCanvas.nextObjectId++;
    obj.type = type;
    obj.p1 = p1;
    obj.p2 = p2;
    obj.p3 = p3;
    obj.radius = radius;
    obj.visible = 1;
    strncpy(obj.description, desc, 99);
    
    mainCanvas.objects[mainCanvas.objectCount++] = obj;
    
    // Draw the shape
    switch (type) {
        case LINE:
            drawLine(p1.x, p1.y, p2.x, p2.y);
            break;
        case RECTANGLE:
            drawRectangle(p1.x, p1.y, p2.x, p2.y);
            break;
        case TRIANGLE:
            drawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
            break;
        case CIRCLE:
            drawCircle(p1.x, p1.y, radius);
            break;
    }
    
    printf("✓ Object #%d added successfully!\n", obj.id);
}

/**
 * Delete object by ID and redraw canvas
 * Time Complexity: O(n) for finding + O(n*perimeter) for redrawing
 */
void deleteObject(int objectId) {
    int index = getObjectIndex(objectId);
    
    if (index == -1) {
        printf("✗ Object with ID %d not found!\n", objectId);
        return;
    }
    
    // Remove object
    for (int i = index; i < mainCanvas.objectCount - 1; i++) {
        mainCanvas.objects[i] = mainCanvas.objects[i + 1];
    }
    mainCanvas.objectCount--;
    
    printf("✓ Object #%d deleted successfully!\n", objectId);
    
    // Redraw canvas
    redrawCanvas();
}

/**
 * Modify object properties
 * Time Complexity: O(n) for finding + O(n*perimeter) for redrawing
 */
void modifyObject(int objectId) {
    int index = getObjectIndex(objectId);
    
    if (index == -1) {
        printf("✗ Object with ID %d not found!\n", objectId);
        return;
    }
    
    GraphicObject* obj = &mainCanvas.objects[index];
    
    printf("\n--- Modify Object #%d ---\n", objectId);
    printf("Current Type: ");
    switch (obj->type) {
        case LINE: printf("Line"); break;
        case RECTANGLE: printf("Rectangle"); break;
        case TRIANGLE: printf("Triangle"); break;
        case CIRCLE: printf("Circle"); break;
    }
    printf("\n");
    
    printf("1. Modify Position\n");
    printf("2. Modify Dimensions\n");
    printf("3. Modify Description\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            printf("Enter new X coordinate for first point: ");
            scanf("%d", &obj->p1.x);
            printf("Enter new Y coordinate for first point: ");
            scanf("%d", &obj->p1.y);
            printf("✓ Position updated!\n");
            break;
        }
        case 2: {
            if (obj->type == CIRCLE) {
                printf("Enter new radius: ");
                scanf("%d", &obj->radius);
            } else {
                printf("Enter new X coordinate for second point: ");
                scanf("%d", &obj->p2.x);
                printf("Enter new Y coordinate for second point: ");
                scanf("%d", &obj->p2.y);
            }
            printf("✓ Dimensions updated!\n");
            break;
        }
        case 3: {
            printf("Enter new description: ");
            getchar(); // Clear newline
            fgets(obj->description, 99, stdin);
            printf("✓ Description updated!\n");
            break;
        }
        default:
            printf("✗ Invalid choice!\n");
            return;
    }
    
    redrawCanvas();
}

/**
 * Redraw entire canvas from objects
 * Time Complexity: O(n * perimeter) where n = number of objects
 */
void redrawCanvas() {
    // Clear canvas
    int i, j;
    for (i = 0; i < CANVAS_HEIGHT; i++) {
        for (j = 0; j < CANVAS_WIDTH; j++) {
            mainCanvas.canvas[i][j] = EMPTY_CELL;
        }
    }
    
    // Redraw all visible objects
    for (i = 0; i < mainCanvas.objectCount; i++) {
        if (!mainCanvas.objects[i].visible) continue;
        
        GraphicObject* obj = &mainCanvas.objects[i];
        switch (obj->type) {
            case LINE:
                bresenhamLine(obj->p1.x, obj->p1.y, obj->p2.x, obj->p2.y);
                break;
            case RECTANGLE: {
                int minX = (obj->p1.x < obj->p2.x) ? obj->p1.x : obj->p2.x;
                int maxX = (obj->p1.x < obj->p2.x) ? obj->p2.x : obj->p1.x;
                int minY = (obj->p1.y < obj->p2.y) ? obj->p1.y : obj->p2.y;
                int maxY = (obj->p1.y < obj->p2.y) ? obj->p2.y : obj->p1.y;
                bresenhamLine(minX, minY, maxX, minY);
                bresenhamLine(maxX, minY, maxX, maxY);
                bresenhamLine(maxX, maxY, minX, maxY);
                bresenhamLine(minX, maxY, minX, minY);
                break;
            }
            case TRIANGLE:
                bresenhamLine(obj->p1.x, obj->p1.y, obj->p2.x, obj->p2.y);
                bresenhamLine(obj->p2.x, obj->p2.y, obj->p3.x, obj->p3.y);
                bresenhamLine(obj->p3.x, obj->p3.y, obj->p1.x, obj->p1.y);
                break;
            case CIRCLE:
                midpointCircle(obj->p1.x, obj->p1.y, obj->radius);
                break;
        }
    }
}

/**
 * List all objects with their properties
 * Time Complexity: O(n)
 */
void listObjects() {
    if (mainCanvas.objectCount == 0) {
        printf("\n✓ No objects on canvas.\n\n");
        return;
    }
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                      OBJECT LIST                              ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < mainCanvas.objectCount; i++) {
        GraphicObject* obj = &mainCanvas.objects[i];
        printf("║ ID: %3d | Type: %-9s | Status: %s                 ║\n",
               obj->id,
               (obj->type == LINE ? "Line" : 
                obj->type == RECTANGLE ? "Rectangle" : 
                obj->type == TRIANGLE ? "Triangle" : "Circle"),
               obj->visible ? "Visible" : "Hidden");
        
        printf("║ Description: %-56s ║\n", obj->description);
        
        if (obj->type == CIRCLE) {
            printf("║ Center: (%3d, %3d) | Radius: %3d                               ║\n",
                   obj->p1.x, obj->p1.y, obj->radius);
        } else {
            printf("║ Point 1: (%3d, %3d) | Point 2: (%3d, %3d)                       ║\n",
                   obj->p1.x, obj->p1.y, obj->p2.x, obj->p2.y);
        }
        
        if (obj->type == TRIANGLE) {
            printf("║ Point 3: (%3d, %3d)                                            ║\n",
                   obj->p3.x, obj->p3.y);
        }
        
        printf("╟────────────────────────────────────────────────────────────────╢\n");
    }
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
}

/* ==================== INTERACTIVE FUNCTIONS ==================== */

/**
 * Interactive function to draw a line
 */
void drawLineInteractive() {
    int x1, y1, x2, y2;
    char desc[100] = "User drawn line";
    
    printf("\n--- Draw Line ---\n");
    printf("Enter first point (x y): ");
    scanf("%d %d", &x1, &y1);
    printf("Enter second point (x y): ");
    scanf("%d %d", &x2, &y2);
    printf("Enter description (optional): ");
    getchar(); // Clear newline
    fgets(desc, 99, stdin);
    
    Point p1 = {x1, y1};
    Point p2 = {x2, y2};
    Point p3 = {0, 0};
    
    addObject(LINE, p1, p2, p3, 0, desc);
}

/**
 * Interactive function to draw a rectangle
 */
void drawRectangleInteractive() {
    int x1, y1, x2, y2;
    char desc[100] = "User drawn rectangle";
    
    printf("\n--- Draw Rectangle ---\n");
    printf("Enter top-left corner (x y): ");
    scanf("%d %d", &x1, &y1);
    printf("Enter bottom-right corner (x y): ");
    scanf("%d %d", &x2, &y2);
    printf("Enter description (optional): ");
    getchar(); // Clear newline
    fgets(desc, 99, stdin);
    
    Point p1 = {x1, y1};
    Point p2 = {x2, y2};
    Point p3 = {0, 0};
    
    addObject(RECTANGLE, p1, p2, p3, 0, desc);
}

/**
 * Interactive function to draw a triangle
 */
void drawTriangleInteractive() {
    int x1, y1, x2, y2, x3, y3;
    char desc[100] = "User drawn triangle";
    
    printf("\n--- Draw Triangle ---\n");
    printf("Enter vertex 1 (x y): ");
    scanf("%d %d", &x1, &y1);
    printf("Enter vertex 2 (x y): ");
    scanf("%d %d", &x2, &y2);
    printf("Enter vertex 3 (x y): ");
    scanf("%d %d", &x3, &y3);
    printf("Enter description (optional): ");
    getchar(); // Clear newline
    fgets(desc, 99, stdin);
    
    Point p1 = {x1, y1};
    Point p2 = {x2, y2};
    Point p3 = {x3, y3};
    
    addObject(TRIANGLE, p1, p2, p3, 0, desc);
}

/**
 * Interactive function to draw a circle
 */
void drawCircleInteractive() {
    int cx, cy, radius;
    char desc[100] = "User drawn circle";
    
    printf("\n--- Draw Circle ---\n");
    printf("Enter center (x y): ");
    scanf("%d %d", &cx, &cy);
    printf("Enter radius: ");
    scanf("%d", &radius);
    printf("Enter description (optional): ");
    getchar(); // Clear newline
    fgets(desc, 99, stdin);
    
    Point p1 = {cx, cy};
    Point p2 = {0, 0};
    Point p3 = {0, 0};
    
    addObject(CIRCLE, p1, p2, p3, radius, desc);
}

/**
 * Interactive function to delete object
 */
void deleteObjectInteractive() {
    printf("\n--- Delete Object ---\n");
    listObjects();
    printf("Enter Object ID to delete: ");
    int id;
    scanf("%d", &id);
    deleteObject(id);
}

/**
 * Interactive function to modify object
 */
void modifyObjectInteractive() {
    printf("\n--- Modify Object ---\n");
    listObjects();
    printf("Enter Object ID to modify: ");
    int id;
    scanf("%d", &id);
    modifyObject(id);
}

/* ==================== FILE I/O FUNCTIONS ==================== */

/**
 * Save canvas to a text file
 * Time Complexity: O(H*W + n)
 */
void saveCanvasToFile() {
    FILE* file = fopen("canvas_backup.txt", "w");
    if (file == NULL) {
        printf("✗ Error opening file for writing!\n");
        return;
    }
    
    // Save canvas
    fprintf(file, "=== CANVAS DATA ===\n");
    fprintf(file, "Width: %d\nHeight: %d\n", CANVAS_WIDTH, CANVAS_HEIGHT);
    fprintf(file, "\n=== CANVAS ===\n");
    
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            fprintf(file, "%c", mainCanvas.canvas[i][j]);
        }
        fprintf(file, "\n");
    }
    
    // Save objects
    fprintf(file, "\n=== OBJECTS ===\n");
    fprintf(file, "Object Count: %d\n", mainCanvas.objectCount);
    fprintf(file, "Next ID: %d\n", mainCanvas.nextObjectId);
    
    for (int i = 0; i < mainCanvas.objectCount; i++) {
        GraphicObject* obj = &mainCanvas.objects[i];
        fprintf(file, "\nObject %d:\n", i);
        fprintf(file, "  ID: %d\n", obj->id);
        fprintf(file, "  Type: %d\n", obj->type);
        fprintf(file, "  P1: %d %d\n", obj->p1.x, obj->p1.y);
        fprintf(file, "  P2: %d %d\n", obj->p2.x, obj->p2.y);
        fprintf(file, "  P3: %d %d\n", obj->p3.x, obj->p3.y);
        fprintf(file, "  Radius: %d\n", obj->radius);
        fprintf(file, "  Visible: %d\n", obj->visible);
        fprintf(file, "  Description: %s\n", obj->description);
    }
    
    fclose(file);
    printf("✓ Canvas saved to 'canvas_backup.txt'\n");
}

/**
 * Load canvas from a text file
 * Time Complexity: O(H*W + n)
 */
void loadCanvasFromFile() {
    FILE* file = fopen("canvas_backup.txt", "r");
    if (file == NULL) {
        printf("✗ No backup file found!\n");
        return;
    }
    
    char buffer[256];
    
    // Skip to canvas section
    while (fgets(buffer, sizeof(buffer), file) && strcmp(buffer, "=== CANVAS ===\n") != 0);
    
    // Load canvas
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        for (int j = 0; j < CANVAS_WIDTH && j < strlen(buffer); j++) {
            if (buffer[j] != '\n') {
                mainCanvas.canvas[i][j] = buffer[j];
            }
        }
    }
    
    // Skip to objects section
    while (fgets(buffer, sizeof(buffer), file) && strcmp(buffer, "=== OBJECTS ===\n") != 0);
    
    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "Object Count: %d", &mainCanvas.objectCount);
    
    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "Next ID: %d", &mainCanvas.nextObjectId);
    
    // Load objects
    for (int i = 0; i < mainCanvas.objectCount; i++) {
        GraphicObject* obj = &mainCanvas.objects[i];
        
        fscanf(file, "\nObject %*d:\n");
        fscanf(file, "  ID: %d\n", &obj->id);
        fscanf(file, "  Type: %d\n", (int*)&obj->type);
        fscanf(file, "  P1: %d %d\n", &obj->p1.x, &obj->p1.y);
        fscanf(file, "  P2: %d %d\n", &obj->p2.x, &obj->p2.y);
        fscanf(file, "  P3: %d %d\n", &obj->p3.x, &obj->p3.y);
        fscanf(file, "  Radius: %d\n", &obj->radius);
        fscanf(file, "  Visible: %d\n", &obj->visible);
        fgets(buffer, sizeof(buffer), file); // Skip "Description: " line
        fgets(obj->description, sizeof(obj->description), file);
        
        // Remove newline from description
        size_t len = strlen(obj->description);
        if (len > 0 && obj->description[len-1] == '\n') {
            obj->description[len-1] = '\0';
        }
    }
    
    fclose(file);
    printf("✓ Canvas loaded from 'canvas_backup.txt'\n");
}

/**
 * Save canvas state to history
 * Time Complexity: O(H*W + n)
 */
void saveCanvasToHistory() {
    if (historySize < MAX_HISTORY) {
        historySize++;
    }
    historyIndex = (historyIndex + 1) % MAX_HISTORY;
    
    // Copy current canvas state
    memcpy(&history[historyIndex], &mainCanvas, sizeof(Canvas));
}

/**
 * Undo last operation
 * Time Complexity: O(H*W + n)
 */
void undoOperation() {
    if (historySize == 0) {
        printf("✗ Nothing to undo!\n");
        return;
    }
    
    historyIndex = (historyIndex - 1 + MAX_HISTORY) % MAX_HISTORY;
    historySize--;
    
    // Restore canvas state
    memcpy(&mainCanvas, &history[historyIndex], sizeof(Canvas));
    printf("✓ Operation undone!\n");
}

/* ==================== MENU FUNCTIONS ==================== */

/**
 * Display main menu
 */
void displayMenu() {
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║     2D GRAPHICS EDITOR - MAIN MENU             ║\n");
    printf("╠════════════════════════════════════════════════╣\n");
    printf("║  1. Display Canvas                             ║\n");
    printf("║  2. Draw Line                                  ║\n");
    printf("║  3. Draw Rectangle                             ║\n");
    printf("║  4. Draw Triangle                              ║\n");
    printf("║  5. Draw Circle                                ║\n");
    printf("║  6. Delete Object                              ║\n");
    printf("║  7. Modify Object                              ║\n");
    printf("║  8. List All Objects                           ║\n");
    printf("║  9. Clear Canvas                               ║\n");
    printf("║ 10. Save Canvas                                ║\n");
    printf("║ 11. Load Canvas                                ║\n");
    printf("║ 12. Undo Last Operation                        ║\n");
    printf("║  0. Exit                                       ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    printf("Enter your choice: ");
}

/**
 * Get user choice from menu
 */
int getUserChoice() {
    int choice;
    scanf("%d", &choice);
    return choice;
}

/* ==================== MAIN FUNCTION ==================== */

int main() {
    int choice;
    int running = 1;
    
    printf("\n");
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║   Welcome to 2D Graphics Editor in C           ║\n");
    printf("║   Canvas: %dx%d pixels                       ║\n", CANVAS_WIDTH, CANVAS_HEIGHT);
    printf("║   Max Objects: %d                              ║\n", MAX_OBJECTS);
    printf("╚════════════════════════════════════════════════╝\n");
    
    // Initialize canvas
    initializeCanvas();
    saveCanvasToHistory();
    
    printf("\n✓ Canvas initialized successfully!\n");
    printf("Canvas size: %d x %d\n", CANVAS_WIDTH, CANVAS_HEIGHT);
    
    // Main loop
    while (running) {
        displayMenu();
        choice = getUserChoice();
        
        switch (choice) {
            case 1:
                displayCanvas();
                break;
            
            case 2:
                saveCanvasToHistory();
                drawLineInteractive();
                break;
            
            case 3:
                saveCanvasToHistory();
                drawRectangleInteractive();
                break;
            
            case 4:
                saveCanvasToHistory();
                drawTriangleInteractive();
                break;
            
            case 5:
                saveCanvasToHistory();
                drawCircleInteractive();
                break;
            
            case 6:
                saveCanvasToHistory();
                deleteObjectInteractive();
                break;
            
            case 7:
                saveCanvasToHistory();
                modifyObjectInteractive();
                break;
            
            case 8:
                listObjects();
                break;
            
            case 9:
                saveCanvasToHistory();
                clearCanvas();
                break;
            
            case 10:
                saveCanvasToFile();
                break;
            
            case 11:
                saveCanvasToHistory();
                loadCanvasFromFile();
                break;
            
            case 12:
                undoOperation();
                break;
            
            case 0:
                printf("\n╔════════════════════════════════════════════════╗\n");
                printf("║     Thank you for using 2D Graphics Editor!     ║\n");
                printf("║                   Goodbye!                      ║\n");
                printf("╚════════════════════════════════════════════════╝\n\n");
                running = 0;
                break;
            
            default:
                printf("✗ Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}
