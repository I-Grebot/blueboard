/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       physicals.c
 * @author     Pierrick
 * @date       21/04/2018
 * -----------------------------------------------------------------------------
 * @brief
 *   Management of physicals elements
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "../../2018_T1_R1/include/main.h"

// -----------------------------------------------------------------------------
// GLOBALS
// -----------------------------------------------------------------------------

// Global containing all the game element constants
phys_t phys;

// Externs
extern match_t match;
extern robot_t robot;
extern path_t pf; // temp

// -----------------------------------------------------------------------------
// INITIALIZE GAME ELEMENTS COORDINATES
// -----------------------------------------------------------------------------

// Warning: all coordinates are expressed in case of a Green match!
//          Coordinates for yellow match are automatically computed.

void phys_init(void)
{

  // Game elements, from the rules
  // -----------------------------

  // Construction cube arranged on the table
  phys.construction_cubes[PHYS_ID_CUBES_WG].x   =  850;
  phys.construction_cubes[PHYS_ID_CUBES_WG].y   =  540;
  phys.construction_cubes[PHYS_ID_CUBES_WG].a   =    0;

  phys.construction_cubes[PHYS_ID_CUBES_SG].x   =  300;
  phys.construction_cubes[PHYS_ID_CUBES_SG].y   = 1190;
  phys.construction_cubes[PHYS_ID_CUBES_SG].a   =    0;

  phys.construction_cubes[PHYS_ID_CUBES_EG].x   = 1100;
  phys.construction_cubes[PHYS_ID_CUBES_EG].y   = 1500;
  phys.construction_cubes[PHYS_ID_CUBES_EG].a   =    0;

  phys.construction_cubes[PHYS_ID_CUBES_WO].x   = 2150;
  phys.construction_cubes[PHYS_ID_CUBES_WO].y   =  540;
  phys.construction_cubes[PHYS_ID_CUBES_WO].a   =    0;

  phys.construction_cubes[PHYS_ID_CUBES_NO].x   = 2700;
  phys.construction_cubes[PHYS_ID_CUBES_NO].y   = 1190;
  phys.construction_cubes[PHYS_ID_CUBES_NO].a   =    0;

  phys.construction_cubes[PHYS_ID_CUBES_EO].x   = 1900;
  phys.construction_cubes[PHYS_ID_CUBES_EO].y   = 2700;
  phys.construction_cubes[PHYS_ID_CUBES_EO].a   =    0;

  // Recuperator containing only one color
  phys.wastewater_recuperator.x	=  WASTEWATER_RECUPERATOR_X;
  phys.wastewater_recuperator.y =  WASTEWATER_RECUPERATOR_Y;
  phys.wastewater_recuperator.a =  0;

  // Recuperator containing half green half orange
  phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_G].x  =  610  ;
  phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_G].y  =  TABLE_Y_MAX - MIXED_WASTEWATER_RECUPERATOR_DEPTH - ROBOT_RADIUS;
  phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_G].a  =  90;

  phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_O].x  =  TABLE_X_MAX - 610;
  phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_O].y  =  TABLE_Y_MAX - MIXED_WASTEWATER_RECUPERATOR_DEPTH - ROBOT_RADIUS;
  phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_O].a  =  90;

  // Sequencing and intermediate POIs
  // --------------------------------

  // Coordinate of the robot at startup
  phys.reset.x  = 		  	ROBOT_LENGTH-ROBOT_FRONT_TO_CENTER;
  phys.reset.y  =  		  	START_LINE_Y;
  phys.reset.a  =   		0;// facing opponent zone

  // 1st exit point
  phys.exit_start.x  = 		SWITCH_CENTER_X;
  phys.exit_start.y  = 		START_LINE_Y;
  phys.exit_start.a  = 		0;

  // starting area of the bee
  phys.beehive.x =  		BEE_START_X;
  phys.beehive.y = 			BEE_START_Y;
  phys.beehive.a =    		0;

  // Coordinate for shooting ores in the basket
  phys.home_automation_switch.x = 	SWITCH_CENTER_X;
  phys.home_automation_switch.y = 	SWITCH_CENTER_Y + ROBOT_RADIUS;
  phys.home_automation_switch.a =  	90;

  // Offsets of the Robot's actuators / systems
  // ------------------------------------------

  // When we don't want to apply any offset
  phys.offset_center.x =  0;
  phys.offset_center.y =  0;
  phys.offset_center.a =  0;

  // Strategic Point Of Interest
  // ---------------------------

  // Teammate position
  robot.teammate_pos.x = TEAMMATE_POS_INIT_X;
  robot.teammate_pos.y = TEAMMATE_POS_INIT_Y;
  robot.teammate_pos.a = 0;

  // Opponents robots position (best guess)
  robot.opp1_pos.x = OPPONENT1_POS_INIT_X;
  robot.opp1_pos.y = OPPONENT1_POS_INIT_Y;
  robot.opp1_pos.a = 0;

  robot.opp2_pos.x = OPPONENT2_POS_INIT_X;
  robot.opp2_pos.y = OPPONENT2_POS_INIT_Y;
  robot.opp2_pos.a = 0;

  // Define static & dynamic obstacles positions
  phys_set_obstacle_positions();
  phys_set_teammate_position(robot.teammate_pos.x, robot.teammate_pos.y);
  phys_set_opponent_position(1, robot.opp1_pos.x, robot.opp1_pos.y);
  phys_set_opponent_position(2, robot.opp2_pos.x, robot.opp2_pos.y);
}

// Update POIs depending on the match color
void phys_update_color_pois(void)
{
  phys_update_with_color(&phys.reset);
  phys_update_with_color(&robot.teammate_pos);
  phys_update_with_color(&robot.opp1_pos);
  phys_update_with_color(&robot.opp2_pos);
  // TODO: add other POIs
}

// Update polygons depending on the match color (mirror them or not)
void phys_update_color_polys(void)
{
  phys_update_with_color_poly(phys.pf_opp_start_zone);
  phys_update_with_color_poly(phys.pf_teammate);
  phys_update_with_color_poly(phys.pf_opponent1);
  phys_update_with_color_poly(phys.pf_opponent2);
}

// -----------------------------------------------------------------------------
// COORDINATES TRANSLATORS
// -----------------------------------------------------------------------------

// Returns true whenever the north side is the left side
uint8_t phys_is_north_left(void) {
  return match.color == MATCH_COLOR_GREEN;
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
        poi->a = 180 - poi->a;
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

// Apply the color transform on all points of the poly
void phys_update_with_color_poly(path_poly_t* poly) {

  uint8_t idx_pt;
  int16_t x;
  int16_t y;

  for(idx_pt = 0; idx_pt < poly->n; idx_pt++)
  {
    x = (int16_t) 10*poly->pts[idx_pt].x;
    y = (int16_t) 10*poly->pts[idx_pt].y;
    phys_update_with_color_xy(&x, &y);
    poly->pts[idx_pt].x = (int32_t) x/10;
    poly->pts[idx_pt].y = (int32_t) y/10;

  }
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

// Define obstacle positions.
// Warning: all coordinates are given assuming to be the 1st team (green).
//          final coordinates are mirrored (if required), depending on the color
//          thanks to the function phys_update_color_polys().
void phys_set_obstacle_positions(void) {

  // Path-finding static polygons
  // -----------------------------

  // Opponent starting area (also includes the border)
  phys.pf_opp_start_zone = path_add_new_poly(4);
  path_poly_set_points(phys.pf_opp_start_zone, 0, TABLE_X_MAX - 400 - ROBOT_RADIUS 	, 0);
  path_poly_set_points(phys.pf_opp_start_zone, 1, TABLE_X_MAX       				, 0);
  path_poly_set_points(phys.pf_opp_start_zone, 2, TABLE_X_MAX       				, 650 + ROBOT_RADIUS);
  path_poly_set_points(phys.pf_opp_start_zone, 3, TABLE_X_MAX - 400 - ROBOT_RADIUS 	, 650 + ROBOT_RADIUS);

  // Treatment plant
  phys.pf_treatment_plant = path_add_new_poly(4);
  path_poly_set_points(phys.pf_treatment_plant, 0, 2106 + ROBOT_RADIUS 	, TABLE_Y_MAX);
  path_poly_set_points(phys.pf_treatment_plant, 1, 2106 + ROBOT_RADIUS 	, TABLE_Y_MAX - 250 - ROBOT_RADIUS);
  path_poly_set_points(phys.pf_treatment_plant, 2, 894 - ROBOT_RADIUS 	, TABLE_Y_MAX - 250 - ROBOT_RADIUS);
  path_poly_set_points(phys.pf_treatment_plant, 3, 894 - ROBOT_RADIUS 	, TABLE_Y_MAX);

  // Path-finding dynamic polygons
  // -----------------------------

  // Other robots:
  // They are all represented as an octogon with identical segments length.
  phys.pf_teammate  = path_add_new_poly(8);
  phys.pf_opponent1 = path_add_new_poly(8);
  phys.pf_opponent2 = path_add_new_poly(8);
  
}

// -----------------------------------------------------------------------------
// PATH-FINDING DYNAMIC POLYGON DEFINITIONS
// -----------------------------------------------------------------------------

/*
   Other robots model is an octogon:

        (0)_________(1)
          /         \
      (7)/           \(2)
        |             |
        |    (x,y)    |
     (6)|             |(3)
         \           /
       (5)\_________/(4)

*/

// Redefine the path-finder polygon associated with the teammate's robot
void phys_set_teammate_position(int16_t x, int16_t y)
{
  path_poly_set_points(phys.pf_teammate, 0, x -   TEAMMATE_SIZE/2,  y - 3*TEAMMATE_SIZE/2);
  path_poly_set_points(phys.pf_teammate, 1, x +   TEAMMATE_SIZE/2,  y - 3*TEAMMATE_SIZE/2);
  path_poly_set_points(phys.pf_teammate, 2, x + 3*TEAMMATE_SIZE/2,  y -   TEAMMATE_SIZE/2);
  path_poly_set_points(phys.pf_teammate, 3, x + 3*TEAMMATE_SIZE/2,  y +   TEAMMATE_SIZE/2);
  path_poly_set_points(phys.pf_teammate, 4, x +   TEAMMATE_SIZE/2,  y + 3*TEAMMATE_SIZE/2);
  path_poly_set_points(phys.pf_teammate, 5, x -   TEAMMATE_SIZE/2,  y + 3*TEAMMATE_SIZE/2);
  path_poly_set_points(phys.pf_teammate, 6, x - 3*TEAMMATE_SIZE/2,  y +   TEAMMATE_SIZE/2);
  path_poly_set_points(phys.pf_teammate, 7, x - 3*TEAMMATE_SIZE/2,  y -   TEAMMATE_SIZE/2);
}

// Redefine the path-finder polygon associated with the opponent's robot
void phys_set_opponent_position(uint8_t robot_idx, int16_t x, int16_t y)
{

  // Primary robot
  if(robot_idx == 1) {
    path_poly_set_points(phys.pf_opponent1, 0, x -   OPPONENT1_SIZE/2,  y - 3*OPPONENT1_SIZE/2);
    path_poly_set_points(phys.pf_opponent1, 1, x +   OPPONENT1_SIZE/2,  y - 3*OPPONENT1_SIZE/2);
    path_poly_set_points(phys.pf_opponent1, 2, x + 3*OPPONENT1_SIZE/2,  y -   OPPONENT1_SIZE/2);
    path_poly_set_points(phys.pf_opponent1, 3, x + 3*OPPONENT1_SIZE/2,  y +   OPPONENT1_SIZE/2);
    path_poly_set_points(phys.pf_opponent1, 4, x +   OPPONENT1_SIZE/2,  y + 3*OPPONENT1_SIZE/2);
    path_poly_set_points(phys.pf_opponent1, 5, x -   OPPONENT1_SIZE/2,  y + 3*OPPONENT1_SIZE/2);
    path_poly_set_points(phys.pf_opponent1, 6, x - 3*OPPONENT1_SIZE/2,  y +   OPPONENT1_SIZE/2);
    path_poly_set_points(phys.pf_opponent1, 7, x - 3*OPPONENT1_SIZE/2,  y -   OPPONENT1_SIZE/2);

    // Secondary robot: smaller lengths
  } else {
    path_poly_set_points(phys.pf_opponent2, 0, x -   OPPONENT2_SIZE/2,  y - 3*OPPONENT2_SIZE/2);
    path_poly_set_points(phys.pf_opponent2, 1, x +   OPPONENT2_SIZE/2,  y - 3*OPPONENT2_SIZE/2);
    path_poly_set_points(phys.pf_opponent2, 2, x + 3*OPPONENT2_SIZE/2,  y -   OPPONENT2_SIZE/2);
    path_poly_set_points(phys.pf_opponent2, 3, x + 3*OPPONENT2_SIZE/2,  y +   OPPONENT2_SIZE/2);
    path_poly_set_points(phys.pf_opponent2, 4, x +   OPPONENT2_SIZE/2,  y + 3*OPPONENT2_SIZE/2);
    path_poly_set_points(phys.pf_opponent2, 5, x -   OPPONENT2_SIZE/2,  y + 3*OPPONENT2_SIZE/2);
    path_poly_set_points(phys.pf_opponent2, 6, x - 3*OPPONENT2_SIZE/2,  y +   OPPONENT2_SIZE/2);
    path_poly_set_points(phys.pf_opponent2, 7, x - 3*OPPONENT2_SIZE/2,  y -   OPPONENT2_SIZE/2);
  }
  
}

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

void phys_pf_poly_to_str(path_poly_t* poly, uint8_t idx_poly, char* str, size_t len)
{
  uint8_t idx_pt;

  snprintf(str, len, "[PHYS] [POLY] %u ", idx_poly);
  str += strlen(str);

  for(idx_pt = 0; idx_pt < poly->n; idx_pt++)
  {
    snprintf(str, len, "%d;%d ",
             10*poly->pts[idx_pt].x,
             10*poly->pts[idx_pt].y);
    str += strlen(str);
  }
  snprintf(str, len, SHELL_EOL);
}


// Print out resulting path-finder path
void phys_pf_path_to_str(char* ret, size_t len)
{
  uint8_t idx_checkpoint;

  // Add the robot current position
  snprintf(ret, len, "[PHYS] [PATH] %d;%d ",
           robot.cs.pos.pos_s16.x,
           robot.cs.pos.pos_s16.y);
  ret += strlen(ret);

  // Print each checkpoint if result is valid
  if(pf.nb_checkpoint != PATH_RESULT_ERROR)
  {
    for(idx_checkpoint = 0; idx_checkpoint < pf.nb_checkpoint; idx_checkpoint++)
    {
     snprintf(ret, len, "%d;%d ",
              pf.u.res[idx_checkpoint].x,
              pf.u.res[idx_checkpoint].y);
    ret += strlen(ret);
    }
  }

  snprintf(ret, len, SHELL_EOL);
}

// Print-out polygons and points
BaseType_t phys_print_pf_polys(char* ret, size_t len)
{

  static uint8_t idx_poly;
  static path_poly_t* poly = NULL;
  uint8_t idx_pt;

  const char* header = "              Poly   Points (X;Y)"SHELL_EOL
                       "-----------------------------------------------------------"SHELL_EOL;

  // Starting
  if(poly == NULL)
  {
    idx_poly = 1;
    poly = &pf.polys[idx_poly];

    // Header
    strcpy(ret, header);
    ret += strlen(ret);
  }

  // Print all points of the poly
  phys_pf_poly_to_str(poly, idx_poly, ret, len);


  // Avoid 1st poly (special)
  /*for(idx_poly = 1; idx_poly < pf.cur_poly_idx; idx_poly++)
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

  }*/

  // Manage next call
  idx_poly++;
  if(idx_poly < pf.cur_poly_idx)
  {
    poly++;
    return pdTRUE;

  // Finished
  } else {
    poly = NULL;
    return pdFALSE;
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

