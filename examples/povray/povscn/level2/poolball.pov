// Persistence Of Vision raytracer version 1.0 sample file.

// By Dan Farmer
// This is a complete rewrite of a scene file that I did a couple of years
// ago.  The original was a real mess... used intersections to do the
// stripes on the balls and had the table and balls set up on the X/Y plane,
// and rotated it on the x-axis to put it horizontal!  This time I did it
// right... used gradients for the stripes and designed things where I wanted
// them in the end.  Much faster with the gradients!
// Added a spotlight, too... new feature in POV-Ray 1.0.

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

// Special re-write of "Shiny" declaration.  Will override textures.inc.
#declare Shiny = texture {
   phong 1 phong_size 200
   reflection 0.25
   ambient 0.15
   diffuse 0.75
}
#include "poolball.inc"

/* ***************** set up the view & light sources **************************/

camera {
   location <0.0  6.0  -15.0>
   direction <0.0 0.0  1.5>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 -1 0>
}

/* Overhead spotlight */
object {
   light_source { <0.0  200.0  -3.0>
      color White
      spotlight
      point_at <0 1 3>
      tightness 800
      radius 8
      falloff 25
   }
}

/* ************************** set up the table ********************************/

object {     /* Felt table top */
   plane { <0 1 0 > -1 }
   texture { 0.15
      ambient 0.2
      diffuse 0.8
      color MediumForestGreen
   }
   color MediumForestGreen
}

/* Cue stick */
object { CueStick
   scale     < 0.15 0.15 1.0 >         /* Skinny and long */
   translate < 0.0  0.0 -7.5 >
}

/* Cue Ball */
object {
   sphere { Ball }
   translate < 0.0  0.0 -6.0 >
   texture {
      0.15
      Shiny
      color White
      ambient 0.25
   }
   color White
}

/* **************************** set up the balls ********************************/
// (The rotations here are just random values to rotate the stripes)

/* Row #1 */
object { _1_Ball
}

/* Row #2 */
object { _3_Ball
   translate <1.0  0.0  1.732 >}

object { _10_Ball  rotate < 0.0  0.0  -80.0 >
   translate <-1.0  0.0 1.732 >}


/* Row #3 */
object { _11_Ball  rotate < 45.0  -35.0  -12.0 >
   translate <2.0  0.0  3.464 >}

object { _8_Ball
   translate < 0.0  0.0  3.464 >}

object { _5_Ball
   translate <-2.0  0.0  3.464 >}


/* Row #4 */
object { _2_Ball
   translate <-3.0  0.0  5.196 >}

object { _7_Ball
   translate <-1.0  0.0  5.196 >}

object { _4_Ball
   translate < 1.0  0.0  5.196 >}

object { _14_Ball  rotate < 0.0  -15.0  -2.0 >
   translate < 3.0  0.0  5.196 >}


/* Row #5 */
object { _15_Ball
   translate <-4.0  0.0  6.928 >}

object { _13_Ball  rotate < -5.0  11.0  -1.0 >
   translate <-2.0  0.0  6.928 >}

object { _9_Ball  rotate < -80.0  -13.0  29.0 >
   translate < 0.0  0.0  6.928 >}

object { _12_Ball  rotate < 15.0  15.0  -2.0 >
   translate < 2.0 0.0  6.928 > }

object { _6_Ball
   translate < 4.0  0.0  6.928 >}
