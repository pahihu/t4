// Persistence Of Vision raytracer version 1.0 sample file.

// By Alexander Enzmann
/* sample quartic scene file written by Alexander Enzmann */

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

#declare Quartic_Saddle =
quartic {
   < 0.0   0.0   0.0   0.0  0.0   0.0   0.0   4.0   0.0  0.0
   0.0   0.0   0.0   0.0  0.0   0.0   0.0   0.0   0.0  0.0
   0.0   0.0   0.0   0.0  0.0   0.0   0.0   0.0   0.0 -1.0
   0.0   0.0   0.0   0.0  0.0 >
}

#declare Unit_Cube =
intersection {
   plane { < 1  0  0> 1 texture { color Clear } }
   plane { <-1  0  0> 1 texture { color Clear } }
   plane { < 0  1  0> 1 texture { color Clear } }
   plane { < 0 -1  0> 1 texture { color Clear } }
   plane { < 0  0  1> 1 texture { color Clear } }
   plane { < 0  0 -1> 1 texture { color Clear } }
}

/* Crossed Trough */
object {
   intersection {
      quartic { Quartic_Saddle
         texture {
            color Red
            specular 0.8
            roughness 0.005
            ambient 0.3
            diffuse 0.6
         }
      }
      intersection { Unit_Cube }
   }
   bounded_by {
      intersection { Unit_Cube scale <1.5 1.5 1.5> }
   }
   scale <2 2 2> 
   rotate <0 -10 0>
   rotate <-60 0 0>
   translate <0 0 4>
}

camera {
   location  <0.0  0.0 -4.0>
   right     <1.33 0.0  0.0>
   up        <0.0  1.0  0.0>
   direction <0.0  0.0  1.0>
}

object {
   light_source { <200 30 -300>
      colour White
   }
}
