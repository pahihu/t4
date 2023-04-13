// Persistence Of Vision raytracer version 1.0 sample file.

// By Alexander Enzmann

/* sample quartic scene file written by Alexander Enzmann */

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

composite {
   /* Lemniscate of Gerono */
   object {
      quartic {
         < 1.0   0.0   0.0    0.0  0.0   0.0   0.0   0.0   0.0 -1.0
         0.0   0.0   0.0    0.0  0.0   0.0   0.0   0.0   0.0  0.0
         0.0   0.0   0.0    0.0  0.0   1.0   0.0   0.0   0.0  0.0
         0.0   0.0   1.0    0.0  0.0 >
      }
      bounded_by {
         sphere { <0 0 0> 2.01 }
      }
      texture {
         color Red
         phong 1.0
         phong_size 10
         ambient 0.2
         diffuse 0.8
      }
      rotate <0 0 45>
   }

   /* Lemniscate of Gerono */
   object {
      quartic {
         < 1.0   0.0   0.0    0.0  0.0   0.0   0.0   0.0   0.0 -1.0
         0.0   0.0   0.0    0.0  0.0   0.0   0.0   0.0   0.0  0.0
         0.0   0.0   0.0    0.0  0.0   1.0   0.0   0.0   0.0  0.0
         0.0   0.0   1.0    0.0  0.0 >
      }
      bounded_by {
         sphere { <0 0 0> 2.01 }
      }
      texture {
         color Blue
         phong 1.0
         phong_size 10
         ambient 0.2
         diffuse 0.8
      }
      rotate <0 0 -45>
   }

   scale <4 4 4>
   rotate <30 0 20>
   translate <0 0 5>

}

camera {
   location  <0.0  0.0 -10.0>
   right     <1.0  0.0  0.0>
   up        <0.0  1.0  0.0>
   direction <0.0  0.0  1.0>
}

object {
   light_source { <200 30 -30>
      colour White
   }
}

object {
   light_source { <-200 30 -300>
      colour White
   }
}

/* Put down floor */
object {
   plane { <0.0 1.0 0.0> -30.0 }
   texture {
      White_Marble
      scale <30 30 30>
      ambient 0.3
      diffuse 0.7
   }
   rotate <5 0 0>
}
