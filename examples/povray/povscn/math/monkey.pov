// Persistence Of Vision raytracer version 1.0 sample file.

// By Alexander Enzmann

/* sample quartic scene file written by Alexander Enzmann */

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#declare Basic_Saddle =
quartic {
   < 0.0   0.0   0.0   1.0  0.0   0.0   0.0   0.0   0.0  0.0
   0.0   0.0  -3.0   0.0  0.0   0.0   0.0   0.0   0.0  0.0
   0.0   0.0   0.0   0.0  0.0   0.0   0.0   0.0   0.0  0.0
   0.0   0.0   0.0  -1.0  0.0 >
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

/* Monkey Saddle */
object {
   intersection {
      quartic { Basic_Saddle
         texture {
            color Red
            specular 1.0
            roughness 0.05
            ambient 0.2
            diffuse 0.8
         }
      }
      intersection { Unit_Cube scale <2 2 2> }
   }
   bounded_by {
      intersection { Unit_Cube scale <2.5 2.5 2.5> }
   }
   rotate <0 20 0>
   rotate <-30 0 0>
}

camera {
   location  <0.0  0.0 -10.0>
   right     <1.33 0.0   0.0>
   up        <0.0  1.0   0.0>
   direction <0.0  0.0   1.0>
}

object {
   light_source { <200 30 -300>
      colour White
   }
}
