// Persistence Of Vision raytracer version 1.0 sample file.

// By Alexander Enzmann
/* sample quartic scene file written by Alexander Enzmann */

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

/* Quartic Cylinder - a Space Needle? */
object {
   quartic {
      < 0.0   0.0   0.0   0.0   1.0   0.0   0.0   0.0   0.0   0.01
      0.0   0.0   0.0   0.0   0.0   0.0   0.0   0.0   0.0   0.0
      0.0   0.0   0.0   1.0   0.0   0.0   0.0   0.0   0.0   0.0
      0.0   0.0   0.01  0.0  -0.01 >
   }
   bounded_by {
      sphere { <0 0 0> 2 }
   }
   texture {
      color Red
      phong 1.0
      phong_size 10
      ambient 0.2
      diffuse 0.8
   }
   rotate <-30 20 0>
   translate <0 0 3>
}

/* Put down checkered floor */
object {
   quadric {
      Plane_XZ
      translate <0.0  -20.0  0.0>
   }
   texture {
      checker colour NavyBlue colour MidnightBlue
      scale < 20.0 20.0 20.0 >
      ambient 0.8
      diffuse 0.2
   }
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
