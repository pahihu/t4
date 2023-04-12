// Persistence Of Vision raytracer version 1.0 sample file.

// By Alexander Enzmann
/* sample quartic scene file written by Alexander Enzmann */

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

#declare Rectangle =
intersection {
   plane { < 1  0  0> 3 texture { color Clear } }
   plane { <-1  0  0> 3 texture { color Clear } }
   plane { < 0  1  0> 3 texture { color Clear } }
   plane { < 0 -1  0> 3 texture { color Clear } }
   plane { < 0  0  1> 1 texture { color Clear } }
   plane { < 0  0 -1> 1 texture { color Clear } }
}

/* Quartic parabola of sorts */
object {
   intersection {
      quartic {
         < 0.1   0.0   0.0  0.0   0.0   0.0   0.0   0.0   0.0  -1.0
         0.0   0.0   0.0  0.0   0.0   0.0   0.0   0.0   0.0   0.0
         0.0   0.0   0.0  0.0   0.0   0.0   0.0   0.0   0.0  -1.0
         0.0   0.0  -1.0  0.0   0.9 >
         texture {
            color Red
            phong 1.0
            phong_size 20
            ambient 0.2
            diffuse 0.8
         }
      }
      intersection { Rectangle }
   }
   bounded_by {
      intersection { Rectangle }
   }
   /* translate <0 0 3> */
   rotate <-30 0 0>
}

camera {
   location  <0.0  0.0 -10.0>
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
