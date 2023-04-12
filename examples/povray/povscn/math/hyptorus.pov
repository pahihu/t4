// Persistence Of Vision raytracer version 1.0 sample file.

// By Alexander Enzmann

/* sample quartic scene file written by Alexander Enzmann */

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"


/* Hyperbolic Torus having major radius sqrt(40), minor radius sqrt(12) */
object {
   quartic {
      < 1.0   0.0   0.0    0.0     2.0   0.0   0.0  -2.0   0.0 -104.0
      0.0   0.0   0.0    0.0     0.0   0.0   0.0   0.0   0.0    0.0
      1.0   0.0   0.0   -2.0     0.0  56.0   0.0   0.0   0.0    0.0
      1.0   0.0  104.0   0.0   784.0 >
   }
   bounded_by {
      sphere { <0 0 0> 20 }
   }
   texture {
      color Red
      specular 1.0
      roughness 0.01
      ambient 0.2
      diffuse 0.8
   }
   rotate <90 0 0>
   rotate <0 30 0>
   translate <0 0 20>
}

/* Put down a floor */
object {
   plane { <0.0  1.0  0.0> -20.0 }
   texture {
      Blue_Agate
      scale < 20.0 20.0 20.0 >
      ambient 0.5
      diffuse 0.5
   }
}

camera {
   location  <0.0  0.0 -20.0>
   direction <0.0  0.0   1.0>
   up        <0.0  1.0   0.0>
   right     <1.33 0.0   0.0>
}

object {
   light_source { <200 30 -300>
      colour White
   }
}

object {
   light_source { <-200 30 -300>
      colour White
   }
}
