Build a complete menu-driven 2D Graphics Editor in C.

Requirements:

1. Canvas
    * Use a 2D character array as the drawing canvas.
    * Canvas size should be configurable (e.g., 50x20).
    * Initialize every cell with ‘_’ (underscore).
    * Use ‘*’ (asterisk) to draw graphical objects.
2. Supported Shapes
    Implement separate functions for:
    * drawLine()
    * drawRectangle()
    * drawTriangle()
    * drawCircle()
3. Object Management
    Each shape should be stored as an object in a structure containing:
    * Object ID
    * Shape type
    * Coordinates
    * Dimensions/radius
    * Visibility status
    Maintain an array of objects.
4. Menu-Driven Operations
    Create a loop-based menu with options:
    1. Display Canvas
    2. Draw Line
    3. Draw Rectangle
    4. Draw Triangle
    5. Draw Circle
    6. Delete Object
    7. Modify Object
    8. List All Objects
    9. Clear Canvas
    10. Exit
5. Delete Object
    * Delete by Object ID.
    * Remove the object from the object list.
    * Redraw the entire canvas from remaining objects.
6. Modify Object
    * Select object by ID.
    * Allow changing position and dimensions.
    * Update object data.
    * Redraw the canvas.
7. Drawing Algorithms
    * Use Bresenham’s Line Algorithm for lines.
    * Use Midpoint Circle Algorithm for circles.
    * Rectangles should be drawn using four lines.
    * Triangles should be drawn using three lines.
8. Display Function
    * Create displayCanvas() function.
    * Print the entire 2D canvas neatly.
    * Include row and column borders if possible.
9. Program Structure
    Organize code into functions:
    * initializeCanvas()
    * clearCanvas()
    * displayCanvas()
    * drawPixel()
    * drawLine()
    * drawRectangle()
    * drawTriangle()
    * drawCircle()
    * addObject()
    * deleteObject()
    * modifyObject()
    * redrawCanvas()
    * listObjects()
10. Data Structures
    Use:
    * enum for shape types
    * struct for graphical objects
    * global canvas array
    * object array capable of storing at least 100 objects
11. Error Handling
    * Prevent drawing outside canvas boundaries.
    * Validate user input.
    * Handle invalid object IDs.
12. Extra Features (Bonus)
    * Save canvas to a text file.
    * Load canvas from a text file.
    * Use ncurses menus instead of standard console menus.
    * Support undo and redo operations.
13. Output
    Generate:
    * Fully working C source code
    * Proper comments
    * Modular design
    * GCC compilation command
    * Sample program execution output
    * Time and space complexity explanation

The solution should be suitable for a Data Structures and Graphics Programming laboratory assignment and follow good C programming practices.
