// Persistence Of Vision raytracer version 1.0 sample file.

// Bezier patch example
// by Alexander Enzmann


#include "shapes.inc"
#include "colors.inc"

object {
   bicubic_patch { 1 5 5
      // bicubic_patch { 3 0.05 5 5 
      < 0.0  0.0  2.0>  < 1.0  0.0  0.0>  < 2.0  0.0  0.0>  < 3.0  0.0 -2.0>
      < 0.0  1.0  0.0>  < 1.0  1.0  0.0>  < 2.0  1.0  0.0>  < 3.0  1.0  0.0>
      < 0.0  2.0  0.0>  < 1.0  2.0  0.0>  < 2.0  2.0  0.0>  < 3.0  2.0  0.0>
      < 0.0  3.0  2.0>  < 1.0  3.0  0.0>  < 2.0  3.0  0.0>  < 3.0  3.0 -2.0>
      texture {
         checker
         color red 1.0 color blue 1.0
         rotate <90 0 0>
         phong 1
      }
   }      /* end_bicubic */
   color red 1.0
   translate <-1.5 -1.5 0>
   scale <2 2 2>
   rotate <30 -70 0>
}

// The viewer is eight units back along the z-axis. 
camera {
   location  <0.0  0.0 -15.0>
   right     <1.3333  0.0  0.0>
   up        <0.0  1.0  0.0>
   direction <0.0  0.0  1.0>
}

// Light source 
object {
   light_source { <100 100 0>
      colour White
   }
}

