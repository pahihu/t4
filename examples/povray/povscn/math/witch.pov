// Persistence Of Vision raytracer version 1.0 sample file.

// By Alexander Enzmann
/* sample quartic scene file written by Alexander Enzmann */

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

/* Witch of Agnesi */
object {
   intersection {
      quartic {
         < 0.0   0.0   0.0   0.0   0.0   0.0   1.0   0.0   0.0   0.0
         0.0   0.0   0.0   0.0   0.0   0.0   0.0   0.0   0.0   0.0
         0.0   0.0   0.0   0.0   0.0   0.0   0.0   1.0   0.0   0.04
         0.0   0.0   0.0   0.0   0.04 >
         texture {
            color Red
            specular 1.0
            roughness 0.05
            ambient 0.2
            diffuse 0.8
         }
      }
      sphere { <0 0 0> 1
         texture {
            color Clear
         }
      }
   }
   bounded_by {
      sphere { <0 0 0> 1.5 }
   }
   rotate <30 0 180>
}

camera {
   location  <0.0  0.0 -3.0>
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
