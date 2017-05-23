/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       path.c
 * @author     Paul
 * @date       2015/04/10
 * -----------------------------------------------------------------------------
 * @brief
 *   Path-finder module
 *   This module was ported and optimized from Microb's Technology Aversive
 *   "obstacle_avoidance" device module. Major modifications consist of:
 *   - Increased computation capacities, allow more than 256 rays (uint16_t)
 *   - Code refactoring to comply with project code style
 *   - Optimized some modulo computations and extensive array accesses
 *   - Fixed a bug in the "is point in poly" function, preventing the use with
 *     overlapping polygon definitions
 *   - Added a lot of comments, renamed some variables for better readability
 *     and understanding of the algorithm
 *
 *  All credits go to Microb Technology:
 *    Main code and algorithm: Fabrice DESCLAUX <serpilliere@droids-corp.org>
 *    Integration in Aversive: Olivier MATZ <zer0@droids-corp.org>
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "main.h"

// -----------------------------------------------------------------------------
// GLOBALS
// -----------------------------------------------------------------------------

// Main path-finding container
path_t pf;

// -----------------------------------------------------------------------------
// INITIALIZER
// -----------------------------------------------------------------------------

void path_init(void) {

  // Cleanup
  memset(&pf, 0, sizeof(pf));

  // Define a default objective and reserve memory for these 2 points
  pf.polys[0].pts = pf.pts;
  pf.polys[0].n = 2;
  path_set_objective(0, 0, 100, 100);

  // Default indexes
  pf.cur_pt_idx = 2;
  pf.cur_poly_idx = 1;
}

// -----------------------------------------------------------------------------
// STATIC AND USEFUL HANDLERS
// -----------------------------------------------------------------------------

// Returns the scalar product
static int32_t prod_scal(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  return x1 * x2 + y1 * y2;
}

// Returns the sign of the scalar product
static int8_t prod_scal_sign(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  int32_t z = prod_scal(x1, y1, x2, y2);
  return z==0?0:(z>0?1:-1);
}

// Returns the vectorial product
static int32_t prod_vect(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  return x1 * y2 - y1 * x2;
}

// Returns the sign of the vectorial product
static int8_t prod_vect_sign(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  int32_t z = prod_vect(x1, y1, x2, y2);
  return z==0?0:(z>0?1:-1);
}

// Norm 2 of a vector
static int32_t norm2_vect(int32_t x, int32_t y) {
  return (int32_t) sqrt(x*x + y*y);
}

// Get the next point of a polygon from a given index in the list of points.
// idx must be smaller or equal to poly->n
static uint8_t get_next_poly_pt(const path_poly_t* poly, uint8_t idx) {
  if(idx+1 == poly->n)
    return 0;
  else
    return idx+1;
}

// -----------------------------------------------------------------------------
// FUNCTIONS USED FOR DEFINING THE ALGORITHM INPUTS
// -----------------------------------------------------------------------------

// Set the path-finder objective, i.e. define the start (src) and destination points.
void path_set_objective(int32_t src_x, int32_t src_y, int32_t dst_x, int32_t dst_y) {

    uint16_t idx;

  // The 2 first points in the list are used for storing the destination.
  pf.pts[0].x = dst_x/10;
  pf.pts[0].y = dst_y/10;
  pf.pts[0].valid = PATH_DIJ_PT_NOT_VISITED;
  pf.pts[0].weight = 1;

  pf.pts[1].x = src_x/10;
  pf.pts[1].y = src_y/10;
  pf.pts[1].valid = PATH_DIJ_PT_NOT_VISITED;
  pf.pts[1].weight = 0;

  // Re-init all dijkstra visited status
  for(idx = 0; idx < pf.cur_pt_idx; idx++) {
      pf.pts[idx].valid = PATH_DIJ_PT_NOT_VISITED;
      pf.pts[idx].weight = 0;
  }

//  pf.cur_pt_idx = 2;
//  pf.cur_poly_idx = 1;
}

// Create a new polygon and add it into the structure.
// Return NULL if the structure is full and cannot add it.
path_poly_t* path_add_new_poly(uint8_t nb_points) {

  // Check to see if we are full
  if(  (pf.cur_pt_idx + nb_points > PATH_MAX_POINTS)
    || (pf.cur_poly_idx + 1 > PATH_MAX_POLYS)) {
    return NULL;
  }

  // If not, add it to record
  pf.polys[pf.cur_poly_idx].n = nb_points;
  pf.polys[pf.cur_poly_idx].pts = &pf.pts[pf.cur_pt_idx];
  pf.cur_pt_idx += nb_points;

  return &pf.polys[pf.cur_poly_idx++];
}


// Add a point to a polygon
void path_poly_set_points(path_poly_t* poly, uint8_t idx, int32_t x, int32_t y) {
  poly->pts[idx].x = x/10;
  poly->pts[idx].y = y/10;
  poly->pts[idx].valid = false;
  poly->pts[idx].weight = 0;
}

// -----------------------------------------------------------------------------
// CORE MATHEMATICAL FUNCTIONS
// -----------------------------------------------------------------------------

// Convert 2 points to a line. Also reduces the coefficient to avoid overflow for
// future calculations. The maximum coefficient is defined by the PATH_MAX_LINE_COEFF.
void path_point_to_line(const path_proc_pt_t* p1, const path_proc_pt_t* p2, path_line_t* l) {

  int32_t max;

  l->a = -(p2->y - p1->y);                // max 400
  l->b =  (p2->x - p1->x);                // max 600
  l->c = -(l->a * p1->x + l->b * p1->y);  // max 400 x 300 + 600 x 200 = 240000

  // Find the maximum coefficient
  max = MAX3(ABS(l->a), ABS(l->b), ABS(l->c));

  // Normalize
  l->a = (l->a * PATH_MAX_LINE_COEFF) / max;
  l->b = (l->b * PATH_MAX_LINE_COEFF) / max;
  l->c = (l->c * PATH_MAX_LINE_COEFF) / max; // PATH_MAX_LINE_COEFF absolute max = 8947
}

// Returns the crossing status of 2 lines. If they cross, update the p parameter.
path_line_cross_e path_interesect_line(const path_line_t* l1,
                                       const path_line_t* l2,
                                       path_proc_pt_t* p) {

  // Test dummy lines
  if ((l1->a == 0 && l1->b == 0) || (l2->a == 0 && l2->b == 0))
    return PATH_LINE_NO_CROSS;

  // Horizontal check
  if (l1->a == 0) {
    if (l2->a == 0) {
      if (l1->b * l2->c == l2->b * l1->c)
        return PATH_LINE_PARALLEL_CROSS;
      return PATH_LINE_NO_CROSS;
    }

    // Lines cross in p
    //       by  + c  = 0
    // a'x + b'y + c' = 0
    p->y = -l1->c / l1->b;
    p->x = -(l2->b * p->y + l2->c) / l2->a;
    return PATH_LINE_CROSS;
  } // l1'a coeff = 0

  // Vertical check
  if (l1->b == 0) {
    if (l2->b == 0) {
      if (l1->a * l2->c == l2->a * l1->c)
        return PATH_LINE_PARALLEL_CROSS;
      return PATH_LINE_NO_CROSS;
    }

    // Lines cross in p
    // ax        + c  = 0
    // a'x + b'y + c' = 0
    p->x = -l1->c / l1->a;
    p->y = -(l2->a * p->x + l2->c) / l2->b;
    return PATH_LINE_CROSS;
  } // l2'a coeff = 0

  // Parallel lines
  if (l2->a * l1->b - l1->a * l2->b == 0) {
    if (l1->a * l2->c == l2->a * l1->c)
      return PATH_LINE_PARALLEL_CROSS;
    return PATH_LINE_NO_CROSS;
  }

  // General case
  p->y =  (l1->a * l2->c - l1->c * l2->a) / (l2->a * l1->b - l1->a * l2->b);
  p->x = -(l1->b * p->y + l1->c) / l1->a;
  return PATH_LINE_CROSS;
}

// Returns the crossing status of 2 segments delimited by [s1;s2] and [t1;t2].
// If the segment cross on a point, update the p value with the crossing coordinates.
path_seg_cross_e path_intersect_segment(path_proc_pt_t* s1, path_proc_pt_t* s2,
                                        path_proc_pt_t* t1, path_proc_pt_t* t2,
                                        path_proc_pt_t* p) {

  path_line_cross_e line_cross;
  path_line_t ls; // Line formed by [s1; s2] segment
  path_line_t lt; // Line formed by [t1; t2] segment
  int8_t u1;
  int8_t u2;

  DEBUG_TRACE_NOPFX("    Seg: [(%d;%d) ; (%d;%d)] / [(%d;%d) ; (%d;%d)] ",
      s1->x, s1->y, s2->x, s2->y, t1->x, t1->y, t2->x, t2->y);

  // Check to see if at least, lines cross
  path_point_to_line(s1, s2, &ls);
  path_point_to_line(t1, t2, &lt);

  line_cross = path_interesect_line(&ls, &lt, p);

  // Lines do not cross: no chance that segments would cross
  if(line_cross == PATH_LINE_NO_CROSS) {
    DEBUG_TRACE_NOPFX("-> Line no cross"DEBUG_EOL);
    return PATH_SEG_NO_CROSS;
  }

  // Lines are parallels and cross
  if(line_cross == PATH_LINE_PARALLEL_CROSS) {
    if(prod_scal_sign(t1->x-s1->x, t1->y-s1->y, t1->x-s2->x, t1->y-s2->y ) <= 0 ) {
      DEBUG_TRACE_NOPFX("-> Parallel Cross"DEBUG_EOL);
      return PATH_SEG_PARALLEL_CROSS;
    }
    if (prod_scal_sign(t2->x-s1->x, t2->y-s1->y, t2->x-s2->x, t2->y-s2->y ) <= 0 ) {
      DEBUG_TRACE_NOPFX("-> Segment parallel cross"DEBUG_EOL);
      return PATH_SEG_PARALLEL_CROSS;
    }
    DEBUG_TRACE_NOPFX("-> No cross parallel"DEBUG_EOL);
    return PATH_SEG_NO_CROSS;
  }

  // else: line_cross = PATH_LINE_CROSS
  // p is updated with the coordinates of the cross point of the 2 lines

  // Check to see if some points are equal
  if (s1->x == t1->x && s1->y == t1->y) {
    *p = *s1;
    return PATH_SEG_CROSS_POINT;
  }
  if (s1->x == t2->x && s1->y == t2->y) {
    *p = *s1;
    return PATH_SEG_CROSS_POINT;
  }
  if (s2->x == t1->x && s2->y == t1->y) {
    *p = *s2;
    return PATH_SEG_CROSS_POINT;
  }
  if (s2->x == t2->x && s2->y == t2->y) {
    *p = *s2;
    return PATH_SEG_CROSS_POINT;
  }

  DEBUG_TRACE_NOPFX("[cross = (%d;%d) ] ", p->x, p->y);
  // General case
  // We need to check if p (crossing point of lines) is contained in the segments
  u1 = prod_scal_sign(p->x - s1->x, p->y - s1->y, p->x - s2->x, p->y - s2->y);
  u2 = prod_scal_sign(p->x - t1->x, p->y - t1->y, p->x - t2->x, p->y - t2->y);

  // Disjoint
  if (u1>0 || u2>0) {
    DEBUG_TRACE_NOPFX("-> No cross disjoint (u1 = %d , u2 = %d)"DEBUG_EOL, u1, u2);
    return PATH_SEG_NO_CROSS;
  }


  // Segment point is just equal to the line crossing point
  if (u1==0 || u2==0)
    return PATH_SEG_CROSS_POINT;

  // Else the crossing point for the segment is the same than the one for the line
  // (p is already updated)
  return PATH_SEG_CROSS;
}

// Check to see if a point is inside a polygon
path_pt_in_poly_e path_pt_is_in_poly(const path_proc_pt_t* p, const path_poly_t* poly) {

  uint8_t i;
  uint8_t j;
  int8_t z;
  int8_t z_max = -1;
  int8_t z_min = +1;

  // Check to see if the point is one of the polygon point
  for(i=0; i < poly->n; i++) {
    if(p->x == poly->pts[i].x && p->y == poly->pts[i].y)
      return PATH_PT_POLY_ON_EDGE;
  }

  // General case
  DEBUG_TRACE("    Pt in poly z = ");
  for(i = 0; i < poly->n; i++) {

    // Get the index of the next point.
    // Exception taken for the last point (polygon closure):
    // the next point is the first in the list.
    j = get_next_poly_pt(poly, i);

    // Check the sign of the vectorial product of the given point and the segment
    // formed by the current polygon's point and the next point
    // To be inside of a polygon, this vectorial product must have the same
    // sign for all segments.
    z = prod_vect_sign(poly->pts[j].x - p->x, poly->pts[j].y - p->y,
                       poly->pts[i].x - p->x, poly->pts[i].y - p->y);
    z_min = MIN(z_min, z);
    z_max = MAX(z_max, z);
    DEBUG_TRACE_NOPFX("%d ", z);

  } // for i

  DEBUG_TRACE_NOPFX(DEBUG_EOL);

  // If at least on vectorial product is null, it means that the point is on
  // one of the polygon's edge
  if(z_min == 0 || z_max == 0) {
    return PATH_PT_POLY_ON_EDGE;

  } else if(z_min != z_max) {
    return PATH_PT_POLY_OUTSIDE;

  } else {
    return PATH_PT_POLY_INSIDE;
  }
}

// Check to see if a segment crosses a polygon (including edges)
path_seg_cross_poly_e path_seg_is_crossing_poly(path_proc_pt_t p1,
                                                path_proc_pt_t p2,
                                                const path_poly_t* poly) {

  path_pt_in_poly_e ret1;
  path_pt_in_poly_e ret2;
  path_seg_cross_poly_e ret;
  path_proc_pt_t p;
  uint8_t i;
  uint8_t j;
  uint8_t cnt = 0;

  // Go through all polygon's segment and check if it intersects with the given segment
  for(i = 0; i < poly->n; i++) {

    // Get the index of the next point.
    j = get_next_poly_pt(poly, i);

    ret = path_intersect_segment(&p1, &p2, &(poly->pts[i]), &(poly->pts[j]), &p);

    switch(ret) {

      // Lines don't cross, we need to continue checking
      case PATH_SEG_NO_CROSS:
        break;

      // Cross completely: we've found at least one crossing so we can stop
      case PATH_SEG_CROSS:
        return PATH_SEG_POLY_CROSS;
        break;

      // Just one of the segment boundary is on the other segment
      case PATH_SEG_CROSS_POINT:
        cnt++;
        break;

      // The segment is parallel to one of the polygon's segment so it is on the side
      case PATH_SEG_PARALLEL_CROSS:
        return PATH_SEG_POLY_SIDE;
        break;
    } // switch(ret)
  } // for

  // Cnt can reach a maximum of 4 if both segment boundaries (2 points)
  // are coincident with a polygon's point. In this case each boundary will flag
  // 2 times, one for each segment nearby the polygon's point.

  // Cases where the segment is from one of the polygon's point and to
  // another polygon segment (3) or to another point (4) but not consecutive.
  // In these cases, the segment must be inside the polygon.
  // Think as a diagonal for example.
  if(cnt == 3 || cnt == 4)
    return PATH_SEG_POLY_CROSS;

  // For other cases we need to know if the segment's boundaries are
  // inside or outside the polygon.
  ret1 = path_pt_is_in_poly(&p1, poly);
  ret2 = path_pt_is_in_poly(&p2, poly);

  // If at least one of the point is inside the polygon,
  // then the segment cross it.
  if(ret1 == PATH_PT_POLY_INSIDE || ret2 == PATH_PT_POLY_INSIDE)
    return PATH_SEG_POLY_CROSS;

  // None of the point are inside the polygon
  switch(cnt) {
    case 0: // segment doesn't touch the polygon
      return PATH_SEG_POLY_NO_CROSS;
      break;

    case 1: // segment touches a side of the polygon
      return PATH_SEG_POLY_TOUCH_EDGE;
      break;

    case 2: // segment touches a corner of the polygon
      if(ret1 == PATH_PT_POLY_OUTSIDE || ret2 == PATH_PT_POLY_OUTSIDE) {
        DEBUG_TRACE("    Touch Edge: 1 = %u , 2 = %u"DEBUG_EOL, ret1, ret2);
        return PATH_SEG_POLY_TOUCH_EDGE;
      } else // at least one point is on edge
        return PATH_SEG_POLY_CROSS;
      break;
  }

  // Just in case; but we shouldn't reach here
  return PATH_SEG_POLY_CROSS;
}

// -----------------------------------------------------------------------------
// MAIN ALGORITHM PROCESSING FUNCTIONS
// -----------------------------------------------------------------------------

// Compute the "visibility rays" algorithm, given the list of polygons.
// The rays array is composed of indexes representing 2 polygon vertices that
// can "see" each others:
//
//  i  : the first polygon number in the input polygon list
//  i+1: the vertex index of this polygon (vertex 1)
//  i+2: the second polygon number in the input polygon list
//  i+3: the vertex index of this polygon (vertex 2)
//
//  Here, vertex 1 can "see" vertex 2 in our visibility graph
//
//  As the first polygon is not a real polygon but the start/stop
//  point, the polygon is NOT an ocluding polygon (but its vertices
//  are used to compute visibility to start/stop points)
//
// Returns the "i" value corresponding to 4x the number of rays found.
uint16_t path_compute_rays(path_poly_t* polys, uint8_t n_polys, uint8_t* rays) {

  uint8_t i;
  uint8_t j;
  uint8_t k;
  uint8_t idx;
  uint16_t ray_n = 0;
  bool is_ok;
  uint8_t pt1;
  uint8_t pt2;
  path_seg_cross_poly_e seg_crossing_poly;

  // Warning: First poly is the starting point

  // Pass #1
  // Compute the inner polygon rays:
  // For each polygon edge, if the vertices can see each others.
  // This is useful only in case of overlaping polygons
  for(i = 0; i < n_polys; i++) {
    for(j = 0; j < polys[i].n; j++) {

      // Get the index of the next point.
      k = get_next_poly_pt(&polys[i], j);

      // If one of the polygon's point is not in the playground,
      // it's no use to compute rays to this point (they are invalid)
      if(!PATH_IS_IN_PLAYGROUND(polys[i].pts[j]))
        continue;

      if(!PATH_IS_IN_PLAYGROUND(polys[i].pts[k]))
        continue;

      // Check to see if the current ray crosses the polygon
      is_ok = true;
      for(idx = 1; idx < n_polys; idx++) {

        // Don't check polygon against itself
        if(idx == i)
          continue;

        seg_crossing_poly = path_seg_is_crossing_poly(polys[i].pts[j], polys[i].pts[k], &polys[idx]);

        if(seg_crossing_poly == PATH_SEG_POLY_CROSS) {
          is_ok = false;
          break;
        }
      } // for(idx)

      // If rays doesn't cross the polygon, add it
      if(is_ok) {
        rays[ray_n++] = i;
        rays[ray_n++] = j;
        rays[ray_n++] = i;
        rays[ray_n++] = k;
        DEBUG_TRACE("Inner Ray #%u : %d"DEBUG_EOL, ray_n>>2, seg_crossing_poly);
      }
    } // for(j)
  } // for(i)

  // Pass #2
  // Compute inter-polygon rays
  for(i = 0; i < n_polys-1; i++) {
    for(pt1 = 0; pt1 < polys[i].n ; pt1++) {

      // If a given point of a polygon is not in the playground, no rays is computed form it
      if(!PATH_IS_IN_PLAYGROUND(polys[i].pts[pt1]))
        continue;

      // Sweep other polygons.
      // Polygons from 0 to i were already checked
      for(j = i+1; j < n_polys; j++) {
        for(pt2 = 0; pt2 < polys[j].n; pt2++) {

          // Same thing
          if(!PATH_IS_IN_PLAYGROUND(polys[j].pts[pt2]))
            continue;

          // Test if the [pt1;pt2] segment crosses a polygon
          is_ok = true;
          for(idx = 1; idx < n_polys; idx++) {
            seg_crossing_poly = path_seg_is_crossing_poly(polys[i].pts[pt1], polys[j].pts[pt2], &polys[idx]);

            if(seg_crossing_poly == PATH_SEG_POLY_CROSS) {
              is_ok = false;
              break;
            }
          } // for(idx)

          // If not crossed, add it
          if(is_ok) {
            rays[ray_n++] = i;
            rays[ray_n++] = pt1;
            rays[ray_n++] = j;
            rays[ray_n++] = pt2;
            DEBUG_TRACE("Inter-Ray #%u : %d"DEBUG_EOL, ray_n>>2, seg_crossing_poly);
          }

        } // for(pt2)
      } // for(j)
    } // for(pt1)
  } // for(i)

  return ray_n;

}

// Compute the weight of all rays.
// The weighting function used here is the distance.
void path_compute_rays_weight(const path_poly_t* polys, const uint8_t* rays, uint16_t ray_n,
                              uint16_t* weight) {

  uint16_t i;
  int32_t x1, x2, y1, y2;

  int32_t norm2;

  for(i = 0; i < ray_n; i+=4) {

    x1 = polys[rays[i]].pts[rays[i+1]].x;
    x2 = polys[rays[i+2]].pts[rays[i+3]].x;
    y1 = polys[rays[i]].pts[rays[i+1]].y;
    y2 = polys[rays[i+2]].pts[rays[i+3]].y;

    norm2 = norm2_vect(x1 - x2, y1 - y2);

    weight[i>>2] = norm2 + 1;

    // Display Ray infos
    DEBUG_INFO_NOPFX("[PHYS] [RAY] %d %d;%d %d;%d"DEBUG_EOL,
        weight[i>>2],
        10*x1, 10*y1,
        10*x2, 10*y2);
  }
}


// Iterative Dijkstra algorithm used for finding the shortest path
// The valid field of a proc_pt is used to determine:
// - If the point was not visited yet (no weight affected)
// - The point has been visited and its weight is correct
// - The point must be visited (the point is part of the solution path)
//
// The algorithm's result will be stored in the point's field "poly" and "pt",
// which corresponds to the parent's point in the solution path.
// Thus, from the end point (destination), we can go back to the start with the
// optimal solution.
void path_compute_dijkstra(uint8_t start_poly, uint8_t start_pt) {

  uint16_t i;
  int8_t add;
  path_proc_pt_t* cur_pt;
  path_proc_pt_t* pt_ray_i_j;
  uint8_t ray_i;
  uint8_t ray_j;
  uint8_t ray_i_add;
  uint8_t ray_j_add;
  bool finished = false;

  // Starting point must be visited
  pf.polys[start_poly].pts[start_pt].valid = PATH_DIJ_PT_MUST_VISIT;

  while(!finished) {

    finished = true;

    // Check all polygons
    for(start_poly = 0; start_poly < PATH_MAX_POLYS; start_poly++) {

      // Check all points of the current polygon
      for(start_pt = 0; start_pt < pf.polys[start_poly].n; start_pt++) {

        // Store current point to avoid extensive array access
        // and make the code more readable
        cur_pt = &(pf.polys[start_poly].pts[start_pt]);

        // If the point must be already visited, no go on for the next point
        if(cur_pt->valid != PATH_DIJ_PT_MUST_VISIT)
          continue;

        add = -2;

        // Look for rays that start or stop at a point that must be visited
        for(i = 0; i < pf.nb_rays ; i += 2) {

          // If index is even it is a start point and connected point is i+2
          // If index is odd, it is a stop  point and connected point is i-2
          add = -add;

          // Use some local values to avoid extensive array access
          // and make the code more readable
          ray_i = pf.u.rays[i];
          ray_j = pf.u.rays[i+1];
          ray_i_add = pf.u.rays[i+add];
          ray_j_add = pf.u.rays[i+add+1];
          pt_ray_i_j = &(pf.polys[ray_i_add].pts[ray_j_add]);

          if((start_poly == ray_i) && (start_pt == ray_j)) {

            if(    (pt_ray_i_j->weight == PATH_DIJ_PT_NOT_VISITED)
                || ((cur_pt->weight + pf.weight[i>>2]) < pt_ray_i_j->weight)) {

              pt_ray_i_j->poly = start_poly;
              pt_ray_i_j->pt = start_pt;
              pt_ray_i_j->valid = PATH_DIJ_PT_MUST_VISIT;
              pt_ray_i_j->weight = cur_pt->weight + pf.weight[i>>2];

              cur_pt->valid = PATH_DIJ_PT_VISITED;
              finished = false;
            }
          }

        } // for(i)
      } // for(start_pt)
    } // for(start_poly)
  } // while(!finished)
}

// Affect the result value in the main path-finding container
// Returns -1 if there is no correct solution
// otherwise returns the number of checkpoints
int8_t path_get_result(path_poly_t* polys, uint8_t* rays) {

  // Start from the destination point (0,1)
  uint8_t poly = 0;
  uint8_t pt = 1;

  // Stepping variables
  uint8_t poly1;
  uint8_t pt1;
  uint8_t nb_checkpoints = 0;
  path_proc_pt_t* cur_pt;

  // Continue until we reach the starting point (0,0)
  while(! (poly == 0 && pt == 0)) {

    // Quick access to current poly/point
    cur_pt = &(polys[poly].pts[pt]);

    // The result has too many checkpoints
    if(nb_checkpoints >= PATH_MAX_CHECKPOINTS)
      return PATH_RESULT_ERROR;

    // Invalid path since it was not visited by the Dijkstra algorithm!
    if(cur_pt->valid == PATH_DIJ_PT_NOT_VISITED) {
      return PATH_RESULT_ERROR;
    }

    // Handle next polygon/point
    poly1 = cur_pt->poly;
    pt1 = cur_pt->pt;
    poly = poly1;
    pt = pt1;

    pf.u.res[nb_checkpoints].x = polys[poly].pts[pt].x * 10;
    pf.u.res[nb_checkpoints].y = polys[poly].pts[pt].y * 10;

    nb_checkpoints++;

  } // while

  // Save result
  pf.nb_checkpoint = nb_checkpoints;

  return nb_checkpoints;
}

// Find the shortest path between the source and destination point by taking
// into account the polygons as obstacles.
// Returns -1 if an error occured
// Otherwise returns the number of checkpoints used by the solution.
// Path points are contained in pf.u.res[] array (start point not present)
int8_t path_process(void) {

  uint16_t nb_rays;

  // First compute the visibility graph
  nb_rays = path_compute_rays(pf.polys, pf.cur_poly_idx, pf.u.rays);

  // Affect each ray with a weight
  path_compute_rays_weight(pf.polys, pf.u.rays, nb_rays, pf.weight);

  // Apply Dijkstra Algorithm on the visibility graph
  // from start (poly 0, point 0) to the end (poly 0, point 1)
  pf.nb_rays = nb_rays;
  path_compute_dijkstra(0, 0);

  // From here we can backtrack the result path from end to the start
  return path_get_result(pf.polys, pf.u.rays);
}


