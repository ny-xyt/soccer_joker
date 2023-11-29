#ifndef _FIELD_H
#define _FIELD_H

#include "../worldmodel/WorldObject.h"
#include "../math/Geometry.h"
#include <stdio.h>


// setup constants for field size, landmark locations, etc
// New Field

const double FIELD_Y = 20;
const double FIELD_X = 30;

const double HALF_FIELD_Y = FIELD_Y / 2.0;
const double HALF_FIELD_X = FIELD_X / 2.0;

const double GOAL_Y = 2.1;    // width
const double GOAL_X = .6;     // depth
const double GOAL_Z = .8;      // height
const double HALF_GOAL_Y = GOAL_Y / 2.0;

const double PENALTY_Y = 6.0;  // 禁区Y
const double PENALTY_X = 1.8;//禁区X

const double FIELD_CENTER_X = 0;
const double FIELD_CENTER_Y = 0;

const double CORNER_Y = 5.5;

const double FORBIDDEN_ZONE_X = 1.8;  // 禁区的X长度
const double FORBIDDEN_ZONE_Y = 3.9;  // 禁区除去球门的Y长度

// Some rectangles

const SIM::Rectangle FIELD =
        SIM::Rectangle(SIM::Point2D(-FIELD_X / 2, FIELD_Y / 2),
                       SIM::Point2D(FIELD_X / 2, -FIELD_Y / 2));
// We don't want to limit our particles to inside the boundary
// So this area is two times larger
const SIM::Rectangle GRASS =
        SIM::Rectangle(SIM::Point2D(-HALF_FIELD_X - 5, HALF_FIELD_Y + 5),
                    SIM::Point2D(  HALF_FIELD_X+5 , -HALF_FIELD_Y -5 ) );


#endif
