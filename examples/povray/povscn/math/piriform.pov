// Persistence Of Vision raytracer version 1.0 sample file.

// By Alexander Enzmann

/* sample quartic scene file written by Alexander Enzmann */

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

/* Piriform - looks a bit like a Hershey's Kiss along the x axis...*/
object {
   quartic {
      < 4.0   0.0   0.0   -4.0  0.0   0.0   0.0   0.0   0.0  0.0
      0.0   0.0   0.0    0.0  0.0   0.0   0.0   0.0   0.0  0.0
      0.0   0.0   0.0    0.0  0.0   1.0   0.0   0.0   0.0  0.0
      0.0   0.0   1.0    0.0  0.0 >
   }
   bounded_by {
      sphere { <0 0 0> 2 }
   }
   texture {
      color red 0.7 green 0.0 blue 0.0
      phong 1.0
      phong_size 20
      ambient 0.2
      diffuse 0.8
   }
   translate <0 0.5 2>
}

/* Put down checkered floor */
object {
   plane { <0.0 1.0 0.0> -10.0 }
   texture {
      checker
      colour red 0.137255 green 0.137255 blue 0.556863
      colour red 0.184314 green 0.184314 blue 0.309804
      scale < 20.0 20.0 20.0 >
      ambient 0.8
      diffuse 0.2
   }
}

camera {
   location  <0.0  2.0 -2.0>
   up        <0.0  1.0  0.0>
   right     <1.33 0.0  0.0>
   look_at   <0.0  0.0  1.0>
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
