// Persistence Of Vision raytracer version 1.0 sample file.

// By Ville Saari
// Copyright (c) 1990 Ferry Island Pixelboys
/*
*
* POV-Ray scene description for Earth-like planet
*
* Created 29-Dec-90
*
* If you do some nice modifications or additions to this file, please send 
* me a copy. My Internet address is:
*
*         vsaari@niksula.hut.fi
*
* fractint-generated starfield backdrop added and conversion to latest ver
* by Aaron a. Collins
*/

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

camera {
   location <0.0 0.0 -80.0>
   direction <0.0 0.0 1.0>
   up <0.0 1.0 0.0>
   right <1.33 0.0 0.0>
}

object {
   sphere { <0.0 0.0 0.0> 30.0 }
   texture {
      0.08
      bozo
      turbulence 0.5
      colour_map {
         [0.0 0.7  colour red 0.0 green 0.3 blue 0.8
         colour red 0.0 green 0.3 blue 0.8]
         [0.7 1.0  colour red 0.0 green 0.7 blue 0.0
         colour red 0.6 green 0.7 blue 0.0]
         [1.0 1.01 colour red 0.6 green 0.7 blue 0.0
         colour red 0.7 green 0.3 blue 0.3]
      }
      scale <10.0 10.0 10.0>
      translate <100.0 0 0>
      ambient 0.0
      diffuse 1.0
   }
}

object {
   sphere { <0.0 0.0 0.0> 30.2 }
   texture {
      bozo
      turbulence 1.0
      colour_map {
         [0.0 0.4 colour red 1.0 green 1.0 blue 1.0 alpha 1.0
         colour red 1.0 green 1.0 blue 1.0 alpha 1.0]
         [0.4 0.9 colour red 1.0 green 1.0 blue 1.0 alpha 1.0
         colour red 1.0 green 1.0 blue 1.0]
         [0.9 1.0 colour red 1.0 green 1.0 blue 1.0
         colour red 1.0 green 1.0 blue 1.0]
      }
      scale <12.0 3.0 12.0>
      rotate <30.0 0.0 -45.0>
      ambient 0.0
      diffuse 1.0
   }
}

object {
   plane { <0.0 0.0 1.0> 80.0 }
   texture {
      //      image_map { <1.0 -1.0 0.0> gif "stars.gif" /*640x400*/
      //                }
      //        scale <100.0 100.0 100.0>
      ambient 1.0
      diffuse 0.0
   }
}

object {
   light_source { <-200.0  200.0  -200.0>
      colour White
   }
}
