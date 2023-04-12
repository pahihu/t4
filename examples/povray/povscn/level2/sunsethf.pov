// Persistence of Vision Raytracer
// Height field sample
// by Douglas Muir
// Note: Original used 640 x 480 height field.
// This version is scaled down for distribution.
// Requires "fract003.gif" plasma gif for the height field.


#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

camera {
   location <0.0  80.0  -300.0>
   direction <0.0  0.0  1.0>
   up <0.0  1.0  0.0>
   right <1.333333 0.0 0.0>
   look_at <0.0  30.0  200.0>
}

object {
   height_field { gif "fract003.gif" 
      water_level 101.0
   }

   scale <320 256 200>

   texture {
      image_map { <1.0 0.0 -1.0> gif "fract003.gif"  }
      scale <320.0 1.0 200.0>
      translate <0.0 0.0 -1.0>
   }

   scale <2.0 0.5 2.0>
   translate <-160.0 -63.5 -100.0>
   rotate <0.0 10.0 0.0>
   translate <-80.0 0.0 -30.0>

   colour White
}

// Define the ocean surface 
object {
   plane { <0.0  1.0  0.0> -10.0  }

   texture {
      colour Aquamarine
      waves 0.06
      frequency 5000.0
      ambient 0.1
      diffuse 0.1
      reflection 0.8
      scale <1000.0 1000.0 1000.0>
   }
   colour red 1.0 green 0.3
}

// Put a floor underneath to catch any errant waves from the ripples 
object {
   plane { <0.0  1.0  0.0> -11.0  }
   texture {
      0.05
      colour red 1.0 green 0.6
      ambient 0.8
      diffuse 0.0
   }
   colour red 1.0 green 0.6
}

// Now draw the sky 
object {
   sphere { <0.0  0.0  0.0> 3500.0  }
   texture {
      onion
      colour_map { [0.0 0.6  colour red 1.0 green 0.6 blue 0.0
         colour red 0.3 green 0.6 blue 0.6]
         [0.6 1.0 colour red 0.3 green 0.6 blue 0.6
         colour red 0.1 green 0.4 blue 0.6]
      }
      scale <6000.0  1700.0  4000.0>
      translate <-1200.0 220.0 2500.0>
      ambient 0.8
      diffuse 0.0   /* we don't want clouds casting shadows on the sky */
   }
   colour red 0.7  green 0.7 blue 1.0
}

// Put in a few clouds 
object {
   plane { <0.0 1.0 0.0> 300.0  }

   texture {
      bozo
      turbulence 0.5
      colour_map {
         [0.0 0.6  colour red 1.0 green 1.0 blue 1.0 alpha 1.0
         colour red 1.0 green 1.0 blue 1.0 alpha 1.0]
         [0.6 0.8 colour red 1.0 green 1.0 blue 1.0 alpha 1.0
         colour red 1.0 green 0.8 blue 0.1]
         [0.8 1.001 colour red 1.0 green 0.8 blue 0.1
         colour red 0.8 green 0.4 blue 0.2]
      }
      scale <1000.0  200.0  800.0>
      ambient 0.7
      diffuse 0.0
   }
   translate <-450.0 0.0 0.0>
   rotate <6.0 0.0 0.0>
   colour red 0.7 green 0.7 blue 1.0
}

// Now to cast some light on the subject 
object {
   light_source { <-150.0  250.0  -400.0>
      colour MediumGoldenrod
   }       
}

// Now to cast some more light on the subject 
object {
   union {
      sphere { <0.0  0.0  0.0> 190.0
         texture {
            colour red 1.0 green 0.6 alpha 0.35
            ambient 1.0
            diffuse 0.0
         }
      }
      light_source { <0 0 0> colour red 1.0 green 0.7 } 
   }       
   translate <-1300.0  380.0  2500.0>
}
