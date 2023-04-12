// Persistence Of Vision raytracer version 1.0 sample file.

// Simpler Bezier patch example
// by Alexander Enzmann


#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

object {
   bicubic_patch { 1 8 8
      < 0.0  0.0  2.0>  < 1.0  0.0  0.0>  < 2.0  0.0  0.0>  < 3.0  0.0 -2.0>
      < 0.0  1.0  0.0>  < 1.0  1.0  0.0>  < 2.0  1.0  0.0>  < 3.0  1.0  0.0>
      < 0.0  2.0  0.0>  < 1.0  2.0  0.0>  < 2.0  2.0  0.0>  < 3.0  2.0  0.0>
      < 0.0  3.0  2.0>  < 1.0  3.0  0.0>  < 2.0  3.0  0.0>  < 3.0  3.0 -2.0>
      texture { checker color red 1.0 color blue 1.0 rotate <90 0 0>
         ambient 0.1 diffuse 0.9 phong 1 }
   }      /* end_bicubic */
   translate <-1.5 -1.5 0>
   scale <2 2 2>
   rotate <30 -60 0>
   bounded_by { sphere { <0 0 0> 6 } }
}

// Back wall 
object {
   plane { <0 0 1> 500 }
   texture {
      color red 0.4 green 0.4 blue 0.4
      // color White 
   }
}

camera {
   location  <0.0  0.0 -15.0>
   right     <1.3333  0.0  0.0>
   up        <0.0  1.0  0.0>
   direction <0.0  0.0  1.0>
}

// Light source 
object {
   light_source { <5 7 -5>
      colour White
   }
}
