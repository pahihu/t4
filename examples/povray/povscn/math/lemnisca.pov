// Persistence Of Vision raytracer version 1.0 sample file.

// By Alexander Enzmann

/* sample quartic scene file written by Alexander Enzmann */

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

/* Lemniscate of Gerono */
object {
   quartic {
      < 1.0   0.0   0.0    0.0  0.0   0.0   0.0   0.0   0.0 -1.0
      0.0   0.0   0.0    0.0  0.0   0.0   0.0   0.0   0.0  0.0
      0.0   0.0   0.0    0.0  0.0   1.0   0.0   0.0   0.0  0.0
      0.0   0.0   1.0    0.0  0.0 >
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
   rotate <0 -45 0>
   translate <0 0 2>
}

/* Put down checkered floor */
object {
   plane { <0.0 1.0 0.0> -20.0 }
   texture {
      checker colour NavyBlue colour MidnightBlue
      scale < 20.0 20.0 20.0 >
      ambient 0.8
      diffuse 0.2
   }
}

camera {
   location  <0.0  1.0 -2.0>
   up 	     <0.0  1.0  0.0>
   right     <1.33 0.0  0.0>
   look_at   <0.0  0.0  5.0>
}

object {
   light_source { <200 30 -300>
      color White
   }
}

object {
   light_source { <-200 30 -300>
      color White
   }
}
