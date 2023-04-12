// Persistence Of Vision raytracer version 1.0 sample file.

// By David Buck

// This is the "classic" sunset scene by David k. Buck.  I'm resurrecting it
// because I always thought it looked very realistic...  - Aaron a. Collins

#include "colors.inc"

camera {
   location <0.0  100.0  200.0>
   direction <0.0  0.0  -1.0>
   up <0.0  1.0  0.0>
   right <1.333333 0.0 0.0>
}

/* Define the ocean surface */
object {
   plane { <0.0 1.0 0.0> -10.0 }

   texture {
      waves 0.06
      frequency 5000.0
      scale <1000.0 1000.0 1000.0>
      reflection 1.0
   }
   colour red 1.0 green 0.3
}

/* Put a floor underneath to catch any errant waves from the ripples */
object {
   plane { <0.0 1.0 0.0> -11.0 }

   texture {
      0.05
      colour red 1.0 green 0.3
      ambient 1.0
      diffuse 0.0
   }
   colour red 1.0 green 0.3
}

/* Now draw the sky, a distant rear wall */
object {
   plane { <0.0 0.0 1.0> -200.0 }

   texture {
      0.05
      colour red 1.0 green 0.3
      ambient 0.3
      diffuse 0.7
   }
   colour red 1.0 green 0.3
}

/* Now to cast some light on the subject */
object {
   union {
      sphere { <0 0 0> 20.0
         texture {
            colour red 1.0 green 0.6 alpha 0.35
            ambient 1.0
            diffuse 0.0
         }
      }
      light_source { <0 0 0> colour red 1.0 green 1  blue 1}
   }
   translate <0.0  30.0  -160.0>
}
