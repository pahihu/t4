// Persistence Of Vision raytracer version 1.0 sample file.

// By Aaron a. Collins

/* Scene Description of a Satellite Dish by Aaron A. Collins */
/* Made to test the quadric "Paraboloid" and "Cone" Shapes   */ 

#include "shapes.inc"
#include "colors.inc"

camera {
   location <0.0  0.0  -70.0>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
}

composite {

   object {			/* The basic dish */
      intersection {
         quadric { Paraboloid_Y
            scale <30.0 20.0 30.0>
         }
         quadric { Paraboloid_Y
            scale <29.0 19.0 29.0>
            inverse
         }
         plane { <0.0 1.0 0.0> 20.0 }
      }
      texture {
         color red 0.8 green 0.2 blue 0.2
         ambient 0.2
         diffuse 0.8
         phong 1.0
         phong_size 10
      }
      color red 0.8 green 0.2 blue 0.2
   }

   object {			/* The lna thingy at the focal point */
      union {
         intersection {
            quadric { QCone_Y
               scale <1.0 10.0 1.0>
               translate <0.0 31.0 0.0>
            }
            plane { <0.0 1.0 0.0> 31.0 }
            plane { <0.0 -1.0 0.0> 0.0 }
         }
         sphere { <0.0 30.0 0.0> 2.0 }
      }
      texture {
         color red 0.0 green 0.2 blue 0.8
         ambient 0.2
         diffuse 0.8
         phong 1.0
         phong_size 30
      }
      color red 0.0 green 0.2 blue 0.8
   }

   object {			/* The equatorial mount */
      quadric { Paraboloid_Y
         rotate <180.0 0.0 0.0>
         scale <30.0 60.0 30.0>
      }
      texture {
         color red 0.0 green 0.8 blue 0.2
         ambient 0.2
         diffuse 0.8
         phong 1.0
         phong_size 30
      }
      color red 0.0 green 0.8 blue 0.2
   }

   rotate <-30.0 -30.0 0.0>

}

object {
   light_source { <100.0  120.0  -130.0>
      colour White
   }
}

object {
   light_source { <-100.0  100.0  -130.0>
      colour White
   }
}
