/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       path.h
 * @author     Paul
 * @date       2015/04/10
 * -----------------------------------------------------------------------------
 * @brief
 *   Definitions for the path finder module
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef _PATH_H_
#define _PATH_H_


/**
********************************************************************************
**
**  Constants
**
********************************************************************************
*/

// Maximum number of points used by the polygons.
// Since we mostly use rectangles, this number should be at least 4 times
// larger than the PATH_MAX_POLY value
#define PATH_MAX_POINTS 100 // must be < 256

// Maximum number of polygons used to represent objects to avoid.
#define PATH_MAX_POLYS 16 // must be < 256

// Maximum number of rays, i.e. resulting segments the "visible point" algorithm.
#define PATH_MAX_RAYS 1000   // i.e. twice more points references

// Maximum number of pass-by points allowed for a resulting trajectory path.
#define PATH_MAX_CHECKPOINTS 8

// Defines the precision of a line equation. The algorithm will only use line
// coefficients smaller than this value. A too small value will reduce the precision,
// while a too large value induces a potential overflow error.
#define PATH_MAX_LINE_COEFF 5000

// Value returned by the get_result() and process() functions when there is
// no valid path.
#define PATH_RESULT_ERROR -1

/**
********************************************************************************
**
**  Macros
**
********************************************************************************
*/

// Expression to know if a point is in the playground (table)
// Table edges (typically 0 and 2000/3000 mm) are considered OUTSIDE,
// because it is convinient to define polygons with 0/2000/3000 values as limits.
#define PATH_IS_IN_PLAYGROUND(pt) ( (pt).x > TABLE_X_MIN/10 && (pt).x < TABLE_X_MAX/10 && \
                                    (pt).y > TABLE_Y_MIN/10 && (pt).y < TABLE_Y_MAX/10 )

/**
********************************************************************************
**
**  Types & Enumerations
**
********************************************************************************
*/

// Enumeration type to represent how 2 lines can cross each others
typedef enum
{
    PATH_LINE_NO_CROSS,         // Lines do not cross
    PATH_LINE_CROSS,            // Lines cross in a single point
    PATH_LINE_PARALLEL_CROSS    // Lines cross but are parallel
} path_line_cross_e;

// Enumeration type to represent how 2 segments can cross each others
typedef enum
{
    PATH_SEG_NO_CROSS,          // Segments do not cross
    PATH_SEG_CROSS,             // Segments cross in a single point
    PATH_SEG_CROSS_POINT,       // Segments cross in one of the segment's point
    PATH_SEG_PARALLEL_CROSS     // Segments cross but are parallel
} path_seg_cross_e;

// Enumeration type to represent the satus of a a point inside a polygon
typedef enum
{
    PATH_PT_POLY_OUTSIDE,       // The point is outside the polygon
    PATH_PT_POLY_INSIDE,        // The point is inside the polygon
    PATH_PT_POLY_ON_EDGE        // The point is on one of the polygon's edge
} path_pt_in_poly_e;

// Enumeration type to represent how a segment can cross a polygon
typedef enum
{
    PATH_SEG_POLY_NO_CROSS,     // The segment doesn't cross the polygon
    PATH_SEG_POLY_CROSS,        // The segment crosses the polygon
    PATH_SEG_POLY_SIDE,         // The segment is on one of the polygon's side
    PATH_SEG_POLY_TOUCH_EDGE,   // One of the segment's boundary is on one of the polygon's edge
} path_seg_cross_poly_e;

// Enumeration type to represent the dijkstra algorithm points state
typedef enum
{
    PATH_DIJ_PT_NOT_VISITED,
    PATH_DIJ_PT_VISITED,
    PATH_DIJ_PT_MUST_VISIT,
} path_dijkstra_pt_e;

// -----------------------------------------------------------------------------
// STRUCTURES DEFINITIONS
// -----------------------------------------------------------------------------

// Structure representing a point used for the entire processing
typedef struct
{
    int32_t x;      // X coordinate
    int32_t y;      // Y coordinate

    // Variables used for Dijkstra
    int32_t weight;
    uint8_t poly;
    uint8_t pt;
    path_dijkstra_pt_e valid;
} path_proc_pt_t;

// Generic representation of a line.
// Use a a.x+b.y+c = 0 equation to represent a line in a 2D plane.
// This complies with horizontal and vertical lines,
// which is impossible with traditionnal a.x + b = 0 equation.
typedef struct
{
    int32_t a;
    int32_t b;
    int32_t c;
} path_line_t;

// Structure representing a polygon.
// A polygon is formed by n individual points and enclosed by segments.
typedef struct
{
    uint8_t n;              // Number of points
    path_proc_pt_t* pts;    // Pointer on an array containing those points
                            // Actual data is not held by this structure.
} path_poly_t;

// Main structure holding the different elements for the path-finding algorithm.
typedef struct
{
    path_proc_pt_t pts[PATH_MAX_POINTS];    // Points of the different polygons
    path_poly_t polys[PATH_MAX_POLYS];      // Polygons (obstacles), using pts.

    uint16_t nb_rays;
    uint8_t cur_poly_idx;
    uint8_t cur_pt_idx;

    uint16_t weight[PATH_MAX_RAYS];
    union {
        uint8_t rays[PATH_MAX_RAYS*2];
        poi_t res[PATH_MAX_CHECKPOINTS];
    } u;
    uint8_t nb_checkpoint;
} path_t;


#endif  /* _PATH_H_ */

