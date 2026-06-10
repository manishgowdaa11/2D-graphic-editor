# 2D Graphics Editor in C

A complete, menu-driven 2D graphics editor implementation in C with support for drawing lines, rectangles, triangles, and circles using advanced graphics algorithms.

## Features

✨ **Core Features:**
- **Interactive Canvas** (80×25 pixels) with real-time display
- **Shape Drawing** using optimized algorithms:
  - Lines: Bresenham's Line Algorithm
  - Circles: Midpoint Circle Algorithm
  - Rectangles: 4-line composition
  - Triangles: 3-line composition
- **Object Management**: Add, delete, modify, and list graphical objects
- **Persistent Storage**: Save and load canvas from files
- **Undo/Redo**: History-based operation reversal
- **Batch Operations**: Clear entire canvas with one command

## Technical Details

### Algorithms Used

#### 1. **Bresenham's Line Algorithm**
```
Time Complexity: O(max(|x2-x1|, |y2-y1|))
Space Complexity: O(1)

Purpose: Draw lines efficiently using integer-only arithmetic
Key Features:
- No floating-point calculations
- Minimal memory usage
- Works for all octants
```

#### 2. **Midpoint Circle Algorithm**
```
Time Complexity: O(radius)
Space Complexity: O(1)

Purpose: Draw circles efficiently using only integer arithmetic
Key Features:
- Draws all 8 octants symmetrically
- Uses decision parameter for efficiency
- No trigonometric functions needed
```

#### 3. **Object Management**
```
Time Complexity:
- Add Object: O(1)
- Delete Object: O(n) for search + O(n*perimeter) for redraw
- Modify Object: O(n) for search + O(n*perimeter) for redraw
- List Objects: O(n)
- Redraw Canvas: O(n*perimeter)

Space Complexity: O(n*p) where n=objects, p=avg perimeter
```

### Data Structures

```c
typedef struct {
    int id;              // Unique object identifier
    ShapeType type;      // LINE, RECTANGLE, TRIANGLE, CIRCLE
    Point p1;            // First point / Circle center
    Point p2;            // Second point
    Point p3;            // Third point (triangles only)
    int radius;          // Circle radius
    int visible;         // Visibility flag
    char description[100]; // User-defined description
} GraphicObject;
```

## Compilation

### Standard GCC Compilation:
```bash
gcc -o graphics_editor graphics_editor.c -lm
```

### With Optimization Flags:
```bash
gcc -O2 -Wall -Wextra -o graphics_editor graphics_editor.c -lm
```

### Debug Build:
```bash
gcc -g -Wall -Wextra -o graphics_editor graphics_editor.c -lm
```

### Compilation Notes:
- `-lm`: Links math library (required for circle algorithm)
- `-O2`: Optimization level 2 for better performance
- `-Wall -Wextra`: Enable all warnings for code quality

## Usage

### Running the Program:
```bash
./graphics_editor
```

### Menu Options:

```
═════════════════════════════════════════════
       2D GRAPHICS EDITOR - MAIN MENU
═════════════════════════════════════════════
1.  Display Canvas      - Show current canvas
2.  Draw Line           - Draw line between two points
3.  Draw Rectangle      - Draw rectangle from two corners
4.  Draw Triangle       - Draw triangle from three vertices
5.  Draw Circle         - Draw circle with center and radius
6.  Delete Object       - Remove object by ID
7.  Modify Object       - Edit object properties
8.  List All Objects    - Display all objects with details
9.  Clear Canvas        - Erase entire canvas
10. Save Canvas         - Save to file (canvas_backup.txt)
11. Load Canvas         - Restore from file
12. Undo Last Operation - Revert last action
0.  Exit                - Quit program
═════════════════════════════════════════════
```

## Example Usage

### Example 1: Drawing a Simple Scene

```
> 2                          # Draw Line
Enter first point (x y): 10 5
Enter second point (x y): 70 5
Enter description: Horizontal Line
✓ Line drawn from (10, 5) to (70, 5)

> 3                          # Draw Rectangle
Enter top-left corner (x y): 15 10
Enter bottom-right corner (x y): 65 20
Enter description: Main Rectangle
✓ Rectangle drawn from (15, 10) to (65, 20)

> 5                          # Draw Circle
Enter center (x y): 40 15
Enter radius: 8
Enter description: Centered Circle
✓ Circle drawn at center (40, 15) with radius 8

> 1                          # Display Canvas
```

### Example 2: Object Management

```
> 8                          # List All Objects
╔════════════════════════════════════════════════════════════════╗
║                      OBJECT LIST                              ║
╠════════════════════════════════════════════════════════════════╣
║ ID:   1 | Type: Line      | Status: Visible                   ║
║ Description: Horizontal Line                                   ║
║ Point 1: ( 10,  5) | Point 2: ( 70,  5)                       ║
╟────────────────────────────────────────────────────────────────╢
║ ID:   2 | Type: Rectangle | Status: Visible                   ║
║ Description: Main Rectangle                                    ║
║ Point 1: ( 15, 10) | Point 2: ( 65, 20)                       ║
╚════════════════════════════════════════════════════════════════╝

> 7                          # Modify Object
Enter Object ID to modify: 2
--- Modify Object #2 ---
Current Type: Rectangle
1. Modify Position
2. Modify Dimensions
3. Modify Description
Enter choice: 2
Enter new X coordinate for second point: 60
Enter new Y coordinate for second point: 18
✓ Dimensions updated!

> 6                          # Delete Object
Enter Object ID to delete: 1
✓ Object #1 deleted successfully!
```

### Example 3: File Operations

```
> 10                         # Save Canvas
✓ Canvas saved to 'canvas_backup.txt'

> 9                          # Clear Canvas
✓ Canvas cleared successfully!

> 11                         # Load Canvas
✓ Canvas loaded from 'canvas_backup.txt'
```

## Canvas Output Example

```
╔════════════════════════════════════════════════════════════════════════════╗
║________________________________________________________________________________║  0
║________________________________________________________________________________║  1
║________________________________________________________________________________║  2
║________________________________________________________________________________║  3
║________****__________________________________________________________________║  4
║________*___*_________________________________________________________________║  5
║________*____*________________________________________________________________║  6
║________*_____*_______________________________________________________________║  7
║________*______*______________________________________________________________║  8
║________*_______*_____________________________________________________________║  9
║________*________*____________________________________________________________║ 10
║________*_____*_______________________________________________________________║ 11
║________*___*_________________________________________________________________║ 12
║________****__________________________________________________________________║ 13
║________________________________________________________________________________║ 14
║________________________________________________________________________________║ 15
╚════════════════════════════════════════════════════════════════════════════╝
  0         1         2         3         4         5         6         7
  0123456789012345678901234567890123456789012345678901234567890123456789012345678
```

## Performance Analysis

### Time Complexity

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Draw Line | O(max(Δx, Δy)) | Bresenham's algorithm |
| Draw Circle | O(radius) | Midpoint algorithm with 8-octant symmetry |
| Draw Rectangle | O(perimeter) | 4 Bresenham lines |
| Draw Triangle | O(perimeter) | 3 Bresenham lines |
| Add Object | O(1) | Append to array |
| Delete Object | O(n) + O(n×p) | Search + redraw all objects |
| List Objects | O(n) | Linear iteration |
| Redraw Canvas | O(n×p) | n objects × perimeter |

### Space Complexity

| Component | Complexity |
|-----------|-----------|
| Canvas Buffer | O(H×W) = O(80×25) = O(2000) |
| Object Array | O(n) = O(100) objects max |
| History Stack | O(n×m) = O(50×2000) |
| **Total** | **O(n×m + H×W)** |

## Constraints & Limitations

### Canvas Specifications:
- **Width**: 80 characters
- **Height**: 25 rows
- **Maximum Objects**: 100
- **History Depth**: 50 operations

### Validation:
- Coordinates must be within canvas bounds
- Circle radius must be positive
- Object IDs are immutable
- Descriptions limited to 100 characters

### Known Limitations:
1. No filled shapes (outline only)
2. No shape rotation capability
3. No layer/z-order management
4. Line drawing uses single-pixel width

## File Format

### Saved Canvas File (`canvas_backup.txt`):

```
=== CANVAS DATA ===
Width: 80
Height: 25

=== CANVAS ===
[80x25 character grid]

=== OBJECTS ===
Object Count: 3
Next ID: 4

Object 0:
  ID: 1
  Type: 0
  P1: 10 5
  P2: 70 5
  P3: 0 0
  Radius: 0
  Visible: 1
  Description: Horizontal Line

[Additional objects...]
```

## Error Handling

The program validates:
- **Coordinate Bounds**: All drawing coordinates must be within (0-79, 0-24)
- **Object ID**: Must exist in object list
- **Circle Radius**: Must be positive
- **File Operations**: Graceful handling of missing files
- **Array Bounds**: Maximum 100 objects enforced

## Advanced Features

### Undo System
- Maintains 50-level deep history
- Automatic snapshot on each operation
- Full canvas + object state restoration

### Save/Load
- Plain text format for inspection
- Preserves all object metadata
- Easy manual editing if needed

### Object Descriptions
- User-defined text for each object
- Helps track complex drawings
- Visible in object list

## Code Statistics

- **Total Lines**: ~1000
- **Functions**: 25+
- **Data Structures**: 3
- **Comments**: Comprehensive inline documentation

## Compilation & Execution Example

```bash
# Compile
$ gcc -O2 -Wall -o graphics_editor graphics_editor.c -lm
gcc -O2 -Wall -o graphics_editor graphics_editor.c -lm

# Run
$ ./graphics_editor

╔════════════════════════════════════════════════╗
│   Welcome to 2D Graphics Editor in C           │
│   Canvas: 80x25 pixels                         │
│   Max Objects: 100                             │
╚════════════════════════════════════════════════╝

✓ Canvas initialized successfully!
Canvas size: 80 x 25

╔════════════════════════════════════════════════╗
║     2D GRAPHICS EDITOR - MAIN MENU             ║
╠════════════════════════════════════════════════╣
║  1. Display Canvas                             ║
║  2. Draw Line                                  ║
║  ...
║  0. Exit                                       ║
╚════════════════════════════════════════════════╝
Enter your choice: _
```

## Testing Checklist

- [x] Canvas initialization and display
- [x] Line drawing (horizontal, vertical, diagonal)
- [x] Rectangle drawing (normal orientation)
- [x] Triangle drawing (various angles)
- [x] Circle drawing (various radii)
- [x] Object addition and list display
- [x] Object deletion and canvas refresh
- [x] Object modification
- [x] Canvas save to file
- [x] Canvas load from file
- [x] Undo operations
- [x] Clear canvas
- [x] Boundary validation
- [x] Large object count handling

## Learning Outcomes

This project demonstrates:
1. **Algorithm Implementation**: Bresenham and Midpoint algorithms
2. **Data Structure Management**: Arrays and structs for object storage
3. **Graphics Programming**: Canvas rendering and pixel manipulation
4. **File I/O**: Save/load functionality
5. **Menu-Driven Architecture**: Interactive user interface
6. **Memory Management**: Efficient resource allocation
7. **Error Handling**: Input validation and bounds checking
8. **Code Organization**: Modular function design

## Future Enhancements

- [ ] Filled shapes support
- [ ] Multiple color/pattern support
- [ ] Shape rotation and transformation
- [ ] Layer management with transparency
- [ ] Bezier curve drawing
- [ ] Polygon tool
- [ ] GUI using ncurses library
- [ ] Real-time redo stack
- [ ] Interactive shape preview
- [ ] Batch operation macros

## Author Notes

This implementation follows good C programming practices:
- Comprehensive function documentation
- Clear variable naming conventions
- Proper memory allocation patterns
- Modular design for maintainability
- Input validation and error handling

Perfect for Data Structures and Graphics Programming courses!

## License

Educational - Free to use and modify for learning purposes.

---

**Last Updated**: June 2026
**Compiler Tested**: GCC 11.0+
**Standard**: C99/C11
