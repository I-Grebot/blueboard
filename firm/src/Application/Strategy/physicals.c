/* -----------------------------------------------------------------------------
 * Igrebrain
 * I-Grebot strategy board
 * -----------------------------------------------------------------------------
 * File        : physicals.c
 * Language    : C
 * Author      : Paul M.
 * Date        : 2015-03-20
 * -----------------------------------------------------------------------------
 * Description
 *   See main module file
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev:: 1382                                                                 $
 * $LastChangedBy:: Paul.M                                                     $
 * $LastChangedDate:: 2015-05-19 22:25:29 +0200 (mar., 19 mai 2015)            $
 * -----------------------------------------------------------------------------
 * Version     Comment                                   Author       Date
 * 1.0         Creation                                  Paul M.      2015-03-20
 * 1.1         Adaptation to 2016 rules                  Pierrick B.  2016-04-17
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

// -----------------------------------------------------------------------------
// GLOBALS
// -----------------------------------------------------------------------------

// Global containing all the game element constants
phys_t phys;

// Externs
extern match_t match;
//extern path_t pf;

// -----------------------------------------------------------------------------
// INITIALIZE GAME ELEMENTS COORDINATES
// -----------------------------------------------------------------------------

// Warning: all coordinates are expressed in case of a Purple match!
//          Coordinates for green match are automatically computed.

void phys_init(void) {


  phys.huts[PHYS_ID_HUT_1].x = 300;
  phys.huts[PHYS_ID_HUT_1].y = 100;
  phys.huts[PHYS_ID_HUT_1].a = 0;

  phys.huts[PHYS_ID_HUT_2].x = 600;
  phys.huts[PHYS_ID_HUT_2].y = 100;
  phys.huts[PHYS_ID_HUT_2].a = 0;

  phys.cube[PHYS_ID_CUBE_1].x = 600;
  phys.cube[PHYS_ID_CUBE_1].y = 900;
  phys.cube[PHYS_ID_CUBE_1].a = 0;

  phys.cube[PHYS_ID_CUBE_2].x = 915;
  phys.cube[PHYS_ID_CUBE_2].y = 200;
  phys.cube[PHYS_ID_CUBE_2].a = 0;

  phys.shells[PHYS_ID_SHELL_NW].x =200;
  phys.shells[PHYS_ID_SHELL_NW].y =1250;
  phys.shells[PHYS_ID_SHELL_NW].a =0;

  phys.shells[PHYS_ID_SHELL_SW].x =200;
  phys.shells[PHYS_ID_SHELL_SW].y =1550;
  phys.shells[PHYS_ID_SHELL_SW].a =0;

  phys.shells[PHYS_ID_SHELL_NE].x =600;
  phys.shells[PHYS_ID_SHELL_NE].y =1250;
  phys.shells[PHYS_ID_SHELL_NE].a =0;

  phys.shells[PHYS_ID_SHELL_SE].x =600;
  phys.shells[PHYS_ID_SHELL_SE].y =1550;
  phys.shells[PHYS_ID_SHELL_SE].a =0;


  // Initialize path-finding polygons
  // --------------------------------

  // Static polygon: Stairs are formed with a basic rectangle
  //phys.pf_construction_area = path_add_new_poly(4);

  // Dynamic polygon: opponent's robot #1
  // The opponent's robot model is an octogon which has identical segments length.
 // phys.pf_opponent1 = path_add_new_poly(8);
  
  // Sequencing and intermediate POIs
  // --------------------------------
  
  // Position of the robot after init
  phys.reset.x = ROBOT_BACK_TO_CENTER;
  phys.reset.y = 600+ROBOT_BASE_WIDTH/2;
  phys.reset.a = 0 ;

  // Very first exit point
  phys.exit_start.x = 400;
  phys.exit_start.y = 900;
  phys.exit_start.a = 0;

  // Very first exit point
  phys.drop.x = 1000;
  phys.drop.y = 900;
  phys.drop.a = 0;

  // Offsets of the Robot's actuators / systems
  // ------------------------------------------
  phys.offset_center.x = 20;
  phys.offset_center.y = 0;
  phys.offset_center.a = 0;


  // Strategic Point Of Interest
  // ---------------------------
  
}

// -----------------------------------------------------------------------------
// COORDINATES TRANSLATORS
// -----------------------------------------------------------------------------

// Returns true whenever the north side is the left side (i.e. Yellow color)
uint8_t phys_is_north_left(void) {
  return match.color == MATCH_COLOR_PURPLE;
}

// Update a POI's position depending on the match color
// When we are Purple, nothing needs to be done since the definitions are
// made with this color.
// When we are Green: - the Y axis is the same
//                  : - the X axis is mirrored
//                  : - the A angle is mirrored (+ 180° mod 360)
void phys_update_with_color(poi_t* poi) {
    if(!phys_is_north_left()) {

        poi->x = TABLE_LENGTH - poi->x;
        //poi->y = TABLE_HEIGHT - poi->y;
        poi->a = 180 + poi->a;
        if(poi->a > 360) {
            poi->a-= 360;
        }
    }
}


void phys_update_with_color_xya(int16_t* x, int16_t* y, int16_t* a) {

    poi_t poi;
    poi.x = *x;
    poi.y = *y;
    poi.a = *a;

    phys_update_with_color(&poi);

    *x = poi.x;
    *y = poi.y;
    *a = poi.a;
    
}

// When we don't care with the angle
void phys_update_with_color_xy(int16_t* x, int16_t* y) {

    int16_t a = 0;
    phys_update_with_color_xya(x, y, &a);

}

// Apply a cartesian offset on the dst point, knowing the current location.
void phys_apply_offset(poi_t* src, poi_t* dest, const poi_t* offset) {

  int16_t dX_src_dest;
  int16_t dY_src_dest;
  double dA_src_dest;
  double cos_dA;
  double sin_dA;
  double offset_x;
  double offset_y;

  int16_t dX_dest_offset;
  int16_t dY_dest_offset;

  // Offset can be mirrored depending on color config
  offset_x = (double) offset->x;

  if(!phys_is_north_left()) {
      offset_y = (double) -offset->y;
  } else {
      offset_y = (double) offset->y;
  }
  
  // Compute the angle between the src and the dest.
  dX_src_dest = dest->x - src->x;
  dY_src_dest = dest->y - src->y;

  // In case source and destination points are the same, we simply assume a null
  // angle. It would be better to consider the current robot's angular position.
  // This is only to avoid a divide by 0 case.
  if((dX_src_dest == 0) && (dY_src_dest == 0)) {
    dA_src_dest = 0;
  } else {
    dA_src_dest = atan2((double) dY_src_dest, (double) dX_src_dest);
  }

  // Project this distance onto the angle formed between the src and dest points to get
  // the actual cartesian offset.
  // This time dX & dY are expressend in the table's coordinate, not robot coordinate.
  cos_dA = cos(dA_src_dest);
  sin_dA = sin(dA_src_dest);
  dX_dest_offset = (int16_t) (offset_x * cos_dA - offset_y * sin_dA);
  dY_dest_offset = (int16_t) (offset_y * cos_dA + offset_x * sin_dA);

  // Finally update the dest vector with updated values
  dest->x -= dX_dest_offset;
  dest->y -= dY_dest_offset;
}

// Add a (dX,dY) offset to (x,y) arguments correspondingto the (d, a_deg) vector
void phys_apply_polar_offset(int16_t* x, int16_t* y, int16_t d, int16_t a_deg) {

  double a_rad;
  double dX;
  double dY;

  // Ensure a is in the [-180;+180[ range
  if(a_deg >= 180)
    a_deg -= 360;
  else if(a_deg < -180)
    a_deg += 360;
  
  // Convert it to radians
  a_rad = DEG_TO_RAD(a_deg);

  dX =  d * cos(a_rad);
  dY =  d * sin(a_rad);
  
  (*x) += (int16_t) dX;
  (*y) += (int16_t) dY;
  
}

// -----------------------------------------------------------------------------
// PATH-FINDING STATIC POLYGON DEFINITIONS
// -----------------------------------------------------------------------------

// Define obstacle positions depending on the color
void phys_set_obstacle_positions(void) {
    
  // Static polygon: Stairs are formed with a basic rectangle
  //path_poly_set_points(phys.pf_construction_area, 0, CONSTRUCTION_AREA_X_MIN - ROBOT_RADIUS, CONSTRUCTION_AREA_Y_MIN - ROBOT_RADIUS);
  //path_poly_set_points(phys.pf_construction_area, 1, CONSTRUCTION_AREA_X_MAX + ROBOT_RADIUS, CONSTRUCTION_AREA_Y_MIN - ROBOT_RADIUS);
  //path_poly_set_points(phys.pf_construction_area, 2, CONSTRUCTION_AREA_X_MAX + ROBOT_RADIUS, CONSTRUCTION_AREA_Y_MAX + ROBOT_RADIUS);
  //path_poly_set_points(phys.pf_construction_area, 3, CONSTRUCTION_AREA_X_MIN - ROBOT_RADIUS, CONSTRUCTION_AREA_Y_MAX + ROBOT_RADIUS);
  
}


// -----------------------------------------------------------------------------
// PATH-FINDING DYNAMIC POLYGON DEFINITIONS
// -----------------------------------------------------------------------------

// Redefine the path-finder polygon associated with the opponent's robot
void phys_set_opponent_position(uint8_t robot_idx, int16_t x, int16_t y) {

  // The opponent's robot model is an octogon:
  //
  //      (0)_________(1)
  //        /         \
  //    (7)/           \(2)
  //      |             |
  //      |    (x,y)    |
  //   (6)|             |(3)
  //       \           /
  //     (5)\_________/(4)

  // Secondary robot: smaller lengths
//  if(robot_idx >= 2) {
//    path_poly_set_points(phys.pf_opponent2, 0, x -   OPPONENT2_SIZE/2,  y - 3*OPPONENT2_SIZE/2);
//    path_poly_set_points(phys.pf_opponent2, 1, x +   OPPONENT2_SIZE/2,  y - 3*OPPONENT2_SIZE/2);
//    path_poly_set_points(phys.pf_opponent2, 2, x + 3*OPPONENT2_SIZE/2,  y -   OPPONENT2_SIZE/2);
//    path_poly_set_points(phys.pf_opponent2, 3, x + 3*OPPONENT2_SIZE/2,  y +   OPPONENT2_SIZE/2);
//    path_poly_set_points(phys.pf_opponent2, 4, x +   OPPONENT2_SIZE/2,  y + 3*OPPONENT2_SIZE/2);
//    path_poly_set_points(phys.pf_opponent2, 5, x -   OPPONENT2_SIZE/2,  y + 3*OPPONENT2_SIZE/2);
//    path_poly_set_points(phys.pf_opponent2, 6, x - 3*OPPONENT2_SIZE/2,  y +   OPPONENT2_SIZE/2);
//    path_poly_set_points(phys.pf_opponent2, 7, x - 3*OPPONENT2_SIZE/2,  y -   OPPONENT2_SIZE/2);

  // Primary robot
//  } else {
    //path_poly_set_points(phys.pf_opponent1, 0, x -   OPPONENT1_SIZE/2,  y - 3*OPPONENT1_SIZE/2);
	// path_poly_set_points(phys.pf_opponent1, 1, x +   OPPONENT1_SIZE/2,  y - 3*OPPONENT1_SIZE/2);
	//path_poly_set_points(phys.pf_opponent1, 2, x + 3*OPPONENT1_SIZE/2,  y -   OPPONENT1_SIZE/2);
	//path_poly_set_points(phys.pf_opponent1, 3, x + 3*OPPONENT1_SIZE/2,  y +   OPPONENT1_SIZE/2);
	//path_poly_set_points(phys.pf_opponent1, 4, x +   OPPONENT1_SIZE/2,  y + 3*OPPONENT1_SIZE/2);
	//path_poly_set_points(phys.pf_opponent1, 5, x -   OPPONENT1_SIZE/2,  y + 3*OPPONENT1_SIZE/2);
	//path_poly_set_points(phys.pf_opponent1, 6, x - 3*OPPONENT1_SIZE/2,  y +   OPPONENT1_SIZE/2);
	//path_poly_set_points(phys.pf_opponent1, 7, x - 3*OPPONENT1_SIZE/2,  y -   OPPONENT1_SIZE/2);
//  }
  
}

// -----------------------------------------------------------------------------
// Define some global POI constants
// -----------------------------------------------------------------------------



//                                X     Y     A

/*
// Starting point of the robot (value at which it is reset)
const poi_t POI_RESET         = { 175, 1060,  -180};

// First POI so the robot can exit its zone safely
const poi_t POI_EXIT_START    = {1100, 1000, -180};

// Coordinates to grab the bulb in the starting area
// Bulb will be grabbed in the right builder (TBC)
const poi_t POI_BULB_GRAB     = { 100, 1200, -180};

// Coordinates of our spots feets
const poi_t POI_FEET1         = { 870, 1485,    0};
const poi_t POI_FEET2         = {1300, 1400,    0};
const poi_t POI_FEET3         = {  90, 1750,    0};
const poi_t POI_FEET4         = {  90, 1850,    0};
const poi_t POI_FEET5         = {1100, 1770,    0};
const poi_t POI_FEET6         = {  90,  200,    0};
const poi_t POI_FEET7         = { 850,  100,    0};
const poi_t POI_FEET8         = { 850,  200,    0};

// POI to get around the corner of 2 feets
const poi_t POI_FEET_CORNER   = {  90, 1500,    0};

// Coordinate of the cups on our side
const poi_t POI_CUP1          = { 350, 1540,   0};
const poi_t POI_CUP2          = { 910, 830,    0};

// Central cup
const poi_t POI_CUP_CENTRAL   = {1500, 1650,    0};

// Claps on our side
const poi_t POI_CLAP1         = { 250, 1900,   90};
const poi_t POI_CLAP2         = { 800, 1900,   90};

// Clap on the other side
const poi_t POI_CLAP3         = {2500, 1900,   -90};

// Position to approach the pop-corn (final approch is done with sensors)
const poi_t POI_POP_PUNCH_FAR = { 450,  350,    90};

// Reset position to reset after the approach is done)
const poi_t POI_POP_PUNCH_RESET = { 450,  150,  90};
*/

