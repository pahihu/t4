// Persistence Of Vision raytracer version 1.0 sample file.

// Persistance of Vision Raytracer
// By Alexander Enzmann

/* sample quartic scene file written by Alexander Enzmann */

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

/*
  Folium - a plane with an oogah horn installed (?) or maybe a sassy
           olive sticking out it's pimento!
*/

object {
   intersection {
      quartic {
         < 0.0   0.0   0.0    0.0  0.0   0.0   0.0   0.0   0.0  2.0
         0.0   0.0  -3.0    0.0  0.0   0.0   0.0  -3.0   0.0  0.0
         0.0   0.0   0.0    0.0  0.0   1.0   0.0   0.0   0.0  0.0
         0.0   0.0   1.0    0.0  0.0 >
         texture {
            color Red
            phong 1.0
            phong_size 10
            ambient 0.2
            diffuse 0.8
         }
      }
      sphere { <0 0 0> 10
         texture {
            color Clear
         }
      }
   }
   bounded_by {
      sphere { <0 0 0> 11 }
   }
   rotate <0 50 10>
   translate <0 0 20>
}

camera {
   location  <0.0  0.0 -10.0>
   direction <0.0  0.0  1.0>
   up        <0.0  1.0  0.0>
   right     <1.33 0.0  0.0>
}

object {
   light_source { <-200 300 -300>
      colour White
   }
}
