// Persistence Of Vision raytracer version 1.0 sample file.

// By Alexander Enzmann
/* sample quartic scene file written by Alexander Enzmann */

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

/* Cubic curve - can make a nice teardrop by cleverly adding a clipping plane */
object {
   intersection {
      quartic {
         < 0.0   0.0   0.0   -0.5  0.0   0.0   0.0   0.0   0.0 -0.5
         0.0   0.0   0.0    0.0  0.0   0.0   0.0   0.0   0.0  0.0
         0.0   0.0   0.0    0.0  0.0   1.0   0.0   0.0   0.0  0.0
         0.0   0.0   1.0    0.0  0.0 >
         texture {
            color Red
            phong 1.0
            diffuse 0.8
            ambient 0.2
         }
      }
      sphere { <0 0 0> 2
         texture {
            color Clear
         }
      }
   }
   bounded_by {
      sphere { <0 0 0> 2.2 }
   }
   rotate <0 20 0>
}

camera {
   location  <0.0  0.0 -5.0>
   direction <0.0  0.0  1.0>
   up        <0.0  1.0  0.0>
   right     <1.33 0.0  0.0>
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
