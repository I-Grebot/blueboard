/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       physicals.c
 * @author     Paul
 * @date       2017/05/05
 * -----------------------------------------------------------------------------
 * @brief
 *   Management of physicals elements
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "main.h"

// -----------------------------------------------------------------------------
// GLOBALS
// -----------------------------------------------------------------------------

// Global containing all the game element constants
phys_t phys;

// Externs
extern match_t match;
extern path_t pf; // temp

// -----------------------------------------------------------------------------
// INITIALIZE GAME ELEMENTS COORDINATES
// -----------------------------------------------------------------------------

// Warning: all coordinates are expressed in case of a Blue match!
//          Coordinates for yellow match are automatically computed.

void phys_init(void)
{

  // Game elements, from the rules
  // -----------------------------

  // Mono modules on the table
  phys.mods_mono_table[PHYS_ID_MODS_M_TA].x   =  950;
  phys.mods_mono_table[PHYS_ID_MODS_M_TA].y   =  200;
  phys.mods_mono_table[PHYS_ID_MODS_M_TA].a   =    0;

  phys.mods_mono_table[PHYS_ID_MODS_M_TB].x   =  200;
  phys.mods_mono_table[PHYS_ID_MODS_M_TB].y   =  600;
  phys.mods_mono_table[PHYS_ID_MODS_M_TB].a   =    0;

  phys.mods_mono_table[PHYS_ID_MODS_M_TC].x   =  800;
  phys.mods_mono_table[PHYS_ID_MODS_M_TC].y   = 1850;
  phys.mods_mono_table[PHYS_ID_MODS_M_TC].a   =    0;

  // Mono modules in the spaceship
  phys.mods_mono_ship.x                       = 1150;
  phys.mods_mono_ship.y                       =   10;
  phys.mods_mono_ship.a                       =    0;

  // Ores small craters
  phys.ores_small_crater[PHYS_ID_ORES_S_A].x  =  SMALL_CRATER_CENTER_X;
  phys.ores_small_crater[PHYS_ID_ORES_S_A].y  =  SMALL_CRATER_CENTER_Y;
  phys.ores_small_crater[PHYS_ID_ORES_S_A].a  =    0;

  phys.ores_small_crater[PHYS_ID_ORES_S_B].x  = 1070;
  phys.ores_small_crater[PHYS_ID_ORES_S_B].y  = 1870;
  phys.ores_small_crater[PHYS_ID_ORES_S_B].a  =    0;

  // Ores big crater
  phys.ores_big_crater.x                      =  300; // TBC
  phys.ores_big_crater.y                      = 1600; // TBC
  phys.ores_big_crater.a                      =    0; // TBC

  // Path-finding static polygons
  // -----------------------------

  // Opponent starting area (also includes the border)
  phys.pf_opp_start_zone = path_add_new_poly(4);
  path_poly_set_points(phys.pf_opp_start_zone, 0, TABLE_X_MAX - 1100,    0);
  path_poly_set_points(phys.pf_opp_start_zone, 1, TABLE_X_MAX       ,    0);
  path_poly_set_points(phys.pf_opp_start_zone, 2, TABLE_X_MAX       ,  380);
  path_poly_set_points(phys.pf_opp_start_zone, 3, TABLE_X_MAX - 1100,  380);

  // Borders of the starting area
  phys.pf_start_border = path_add_new_poly(4);
  path_poly_set_points(phys.pf_start_border, 0,   0, 360);
  path_poly_set_points(phys.pf_start_border, 1, 720, 360);
  path_poly_set_points(phys.pf_start_border, 2, 720, 380);
  path_poly_set_points(phys.pf_start_border, 3,   0, 380);

  // Small craters
  phys.pf_small_crater_a = path_add_new_poly(6);
  path_poly_set_points(phys.pf_small_crater_a, 0,   SMALL_CRATER_CENTER_X - SMALL_CRATER_INTERNAL_SIZE/2 ,  SMALL_CRATER_CENTER_Y - SMALL_CRATER_SIDE_LENGTH/2);
  path_poly_set_points(phys.pf_small_crater_a, 1,   SMALL_CRATER_CENTER_X                                ,  SMALL_CRATER_CENTER_Y - SMALL_CRATER_INTERNAL_SIZE/2);
  path_poly_set_points(phys.pf_small_crater_a, 2,   SMALL_CRATER_CENTER_X + SMALL_CRATER_INTERNAL_SIZE/2 ,  SMALL_CRATER_CENTER_Y - SMALL_CRATER_SIDE_LENGTH/2);
  path_poly_set_points(phys.pf_small_crater_a, 3,   SMALL_CRATER_CENTER_X + SMALL_CRATER_INTERNAL_SIZE/2 ,  SMALL_CRATER_CENTER_Y + SMALL_CRATER_SIDE_LENGTH/2);
  path_poly_set_points(phys.pf_small_crater_a, 4,   SMALL_CRATER_CENTER_X                                ,  SMALL_CRATER_CENTER_Y + SMALL_CRATER_INTERNAL_SIZE/2);
  path_poly_set_points(phys.pf_small_crater_a, 5,   SMALL_CRATER_CENTER_X - SMALL_CRATER_INTERNAL_SIZE/2 ,  SMALL_CRATER_CENTER_Y + SMALL_CRATER_SIDE_LENGTH/2);

  phys.pf_opp_small_crater_a = path_add_new_poly(6);
  path_poly_set_points(phys.pf_opp_small_crater_a, 0, TABLE_X_MAX - SMALL_CRATER_CENTER_X - SMALL_CRATER_INTERNAL_SIZE/2 ,  SMALL_CRATER_CENTER_Y - SMALL_CRATER_SIDE_LENGTH/2);
  path_poly_set_points(phys.pf_opp_small_crater_a, 1, TABLE_X_MAX - SMALL_CRATER_CENTER_X                                ,  SMALL_CRATER_CENTER_Y - SMALL_CRATER_INTERNAL_SIZE/2);
  path_poly_set_points(phys.pf_opp_small_crater_a, 2, TABLE_X_MAX - SMALL_CRATER_CENTER_X + SMALL_CRATER_INTERNAL_SIZE/2 ,  SMALL_CRATER_CENTER_Y - SMALL_CRATER_SIDE_LENGTH/2);
  path_poly_set_points(phys.pf_opp_small_crater_a, 3, TABLE_X_MAX - SMALL_CRATER_CENTER_X + SMALL_CRATER_INTERNAL_SIZE/2 ,  SMALL_CRATER_CENTER_Y + SMALL_CRATER_SIDE_LENGTH/2);
  path_poly_set_points(phys.pf_opp_small_crater_a, 4, TABLE_X_MAX - SMALL_CRATER_CENTER_X                                ,  SMALL_CRATER_CENTER_Y + SMALL_CRATER_INTERNAL_SIZE/2);
  path_poly_set_points(phys.pf_opp_small_crater_a, 5, TABLE_X_MAX - SMALL_CRATER_CENTER_X - SMALL_CRATER_INTERNAL_SIZE/2 ,  SMALL_CRATER_CENTER_Y + SMALL_CRATER_SIDE_LENGTH/2);

  // Big crater
  phys.pf_big_crater = path_add_new_poly(5);
  path_poly_set_points(phys.pf_big_crater, 0,                   0 , TABLE_Y_MAX);
  path_poly_set_points(phys.pf_big_crater, 1,                   0 , TABLE_Y_MAX - BIG_CRATER_RADIUS);
  path_poly_set_points(phys.pf_big_crater, 2, BIG_CRATER_RADIUS/2 , TABLE_Y_MAX - BIG_CRATER_RADIUS);
  path_poly_set_points(phys.pf_big_crater, 3, BIG_CRATER_RADIUS   , TABLE_Y_MAX - BIG_CRATER_RADIUS/2);
  path_poly_set_points(phys.pf_big_crater, 4, BIG_CRATER_RADIUS   , TABLE_Y_MAX);

  phys.pf_opp_big_crater = path_add_new_poly(5);
  path_poly_set_points(phys.pf_opp_big_crater, 0, TABLE_X_MAX -                   0 , TABLE_Y_MAX);
  path_poly_set_points(phys.pf_opp_big_crater, 1, TABLE_X_MAX -                   0 , TABLE_Y_MAX - BIG_CRATER_RADIUS);
  path_poly_set_points(phys.pf_opp_big_crater, 2, TABLE_X_MAX - BIG_CRATER_RADIUS/2 , TABLE_Y_MAX - BIG_CRATER_RADIUS);
  path_poly_set_points(phys.pf_opp_big_crater, 3, TABLE_X_MAX - BIG_CRATER_RADIUS   , TABLE_Y_MAX - BIG_CRATER_RADIUS/2);
  path_poly_set_points(phys.pf_opp_big_crater, 4, TABLE_X_MAX - BIG_CRATER_RADIUS   , TABLE_Y_MAX);

  // Central area
  phys.pf_central_area = path_add_new_poly(5);
  path_poly_set_points(phys.pf_central_area, 0,  840, TABLE_Y_MAX);
  path_poly_set_points(phys.pf_central_area, 1,  840, 1300);
  path_poly_set_points(phys.pf_central_area, 2, 1500, 1150);
  path_poly_set_points(phys.pf_central_area, 3, TABLE_X_MAX - 840, 1300);
  path_poly_set_points(phys.pf_central_area, 4, TABLE_X_MAX - 840, TABLE_Y_MAX);

  // Path-finding dynamic polygons
  // -----------------------------

  // Dynamic polygon: opponent's robot #1
  // The opponent's robot model is an octogon which has identical segments length.
 // phys.pf_opponent1 = path_add_new_poly(8);
  
  // Sequencing and intermediate POIs
  // --------------------------------

  // Coordinate of the robot at startup
  phys.reset.x  = 0;//500;  // TBC
  phys.reset.y  = 0;//180;
  phys.reset.a  =   0; // Facing left

  // 1st exit point
  phys.exit_start.x  = 1050;
  phys.exit_start.y  = 180;
  phys.exit_start.a  =   0;

  // Waypoint for building modules on the side
  phys.mods_build_side.x =   80;
  phys.mods_build_side.y = 1150; // Starting at the bottom
  phys.mods_build_side.a =    0;

  // Shooting location for ores
  phys.ores_basket_shoot.x = 250;
  phys.ores_basket_shoot.y = 500;
  phys.ores_basket_shoot.a =  90; // Facing to the north

  // Offsets of the Robot's actuators / systems
  // ------------------------------------------

  //phys.offset_center.x = 20;
  //phys.offset_center.y = 0;
  //phys.offset_center.a = 0;


  // Strategic Point Of Interest
  // ---------------------------

}

// -----------------------------------------------------------------------------
// COORDINATES TRANSLATORS
// -----------------------------------------------------------------------------

// Returns true whenever the north side is the left side
uint8_t phys_is_north_left(void) {
  return match.color == MATCH_COLOR_BLUE;
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

  /*
     The opponent's robot model is an octogon:

          (0)_________(1)
            /         \
        (7)/           \(2)
          |             |
          |    (x,y)    |
       (6)|             |(3)
           \           /
         (5)\_________/(4)

*/
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
// Helpers
// -----------------------------------------------------------------------------

// Print-out polygons and points
void phys_print_pf(void)
{

  uint8_t idx_poly;
  uint8_t idx_pt;
  path_poly_t* poly;

  DEBUG_INFO("              Poly   Points (X;Y)"DEBUG_EOL);
  DEBUG_INFO("-----------------------------------------------------------"DEBUG_EOL);

  // Avoid 1st poly (special)
  for(idx_poly = 1; idx_poly < pf.cur_poly_idx; idx_poly++)
  {
    poly = &pf.polys[idx_poly];

    printf("[PHYS] [POLY] %u ", idx_poly);

    for(idx_pt = 0; idx_pt < poly->n; idx_pt++)
    {
      printf("%u;%u ",
            10*poly->pts[idx_pt].x,
            10*poly->pts[idx_pt].y);
    }
    printf(DEBUG_EOL);

  }



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

