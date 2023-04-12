// Persistence Of Vision raytracer version 1.0 sample file.

// By Dan Farmer

// Demonstrates one use of the powerful alpha parameter for colors.

#include "colors.inc"

camera {
   location <-1.5  30.0  -150.0>
   direction <0.0 0.0 2.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0.0  25.0  35.0>
}

object {    /* Basic Light source */
   light_source { <100.0  100.0  -200.0>
      colour White
   }
}

/* Now draw the sky */
object {
   sphere { <0.0  0.0  0.0> 200000.0 }
   texture {
      bozo
      turbulence 0.35
      colour_map {
         [0.0 0.5   colour red 0.5 green 0.6 blue 1.0
         colour red 0.6 green 0.5 blue 1.0]
         [0.5 0.6   colour red 0.5 green 0.6 blue 1.0
         colour red 1.0 green 1.0 blue 1.0]
         [0.6 1.001 colour red 1.0 green 1.0 blue 1.0
         colour red 0.5 green 0.5 blue 0.5]
      }
      scale <100000.0 10000.0 100000.0>
      ambient 1.0
      diffuse 0.0
   }
   colour SkyBlue
}

object {
   plane { <0.0 1.0 0.0 > 0.0  }
   texture { colour NeonBlue reflection 0.15 }
   color White
}

/*******************************************************************************/
/*
  This next object uses the alpha parameter to make a sphere with
  a "cutout" checker pattern.  

  Don't limit this idea to checker patterns.  Try it with gradient and
  bozo, for example. Or maybe marble with alpha 1.0 for all but the
  "veins".
  Try a series of "nested" concentric spheres, all with the transparent
  checker pattern as its surface, perhaps in different colors.
*/

object {
   sphere { <0.0 25.0 0.0 > 25.0 }
   texture {
      checker colour YellowGreen
      colour Clear
      scale <4.0 50.0 2.0>
      rotate <90 0.0 -90.0>
      brilliance 8
      phong 1
      phong_size 100
   }
   color White

   bounded_by {
      sphere { <0.0 25.0 0.0> 25.2 }
   }
}
