// Persistence Of Vision raytracer version 1.0 sample file.

// A single wooden torus
// Illustrates what is possible withPOV-Raywood textures.
// File by Dan Farmer Jan 1992

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

/*
   Torus specs:                                                                  
      Major radius: 7.000000       Minor radius: 3.000000
      Outer radius: 10.000000      Inner radius: 4.000000
*/
#declare Torus =
quartic {
   <    1.000000 0.000000 0.000000 0.000000 2.000000
   0.000000 0.000000 2.000000 0.000000 -116.000000
   0.000000 0.000000 0.000000 0.000000 0.000000
   0.000000 0.000000 0.000000 0.000000 0.000000
   1.000000 0.000000 0.000000 2.000000 0.000000
   80.000000 0.000000 0.000000 0.000000 0.000000
   1.000000 0.000000 -116.000000 0.000000 1600.000000 >
}


// Wooden torus
object {
   quartic { Torus }

   // Bottom layer texture.  Uses a "stretched" bozo for fine porous grain
   texture {
      bozo
      color_map {
         [0.0 0.4 color BakersChoc  color BakersChoc ]
         [0.4 1.01 color Tan color Tan]
      }
      scale <4 0.05 0.05>
   }

   // Overlaying ring grain texture
   texture {
      phong 1
      phong_size 100
      brilliance 3
      ambient 0.2
      diffuse 0.8
      wood turbulence 0.025

      color_map {
         [0.0 0.15 color SemiSweetChoc color CoolCopper ]
         [0.15 0.40 color CoolCopper color Clear ]
         [0.40 0.80 color Clear  color CoolCopper ]
         [0.80 1.01 color CoolCopper color SemiSweetChoc ]
      }

      scale <3.5 1 1>
      translate <0 -50 0>
      rotate <0 0 1.5>
   }

   translate <0 0 0>
}


// Main light source
object {
   light_source { <-50.0 100 -80.0>
      colour White
   }
}

// Dim side light to fill shadows
object {
   light_source { <250.0  25.0 -100.0>
      colour DimGray
   }
}


camera {
   location <0.0  20.0  -15.0>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 0 0>
}
