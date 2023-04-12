// Persistence Of Vision raytracer version 1.0 sample file.

// By  Ville Saari
// Copyright (c) 1991 Ferry Island Pixelboys
/* NOTE: Requires "eight_.iff"
*
* POV-Ray scene description for two billiard balls
* 
* Created: 07-Jan-91
* Updated: 09-Jan-91
*          19-Jan-92 dmf (see below)
*
* If you do some nice modifications or additions to this file, please send 
* me a copy. My Internet address is:
*
*         vsaari@niksula.hut.fi
*
*  Modified somewhat by dmf forPOV-Ray1.0:
*  decreased most ambient values a bit
*  increased phong_size on balls
*  fixed the image_map ... interpolation didn't work.
*  modified the wood on the questick a bit
*/

camera {
   location <-15.0 8.0 -10.0>
   direction <0.0 0.0 1.0>
   up <0.0 1.0 0.0>
   right <1.333 0.0 0.0>
   sky <0.0 1.0 0.0>
   look_at <0.0 5.0 5.0>
}

// tabletop
object {
   plane { <0.0 1.0 0.0> 0.0 }
   texture {
      0.08
      ambient 0.3
      diffuse 0.7
      colour green 0.7
   }
   colour green 0.7
}

// eightball
object {
   sphere { <0.0 5.0 0.0> 5.0 }

   texture {
      colour red 0.0  green 0.0  blue 0.0
      ambient 0.15
      diffuse 0.8
      brilliance 1.0 
      phong 1.0      
      phong_size 90
      reflection 0.15
   }

   texture {
      image_map { <1.0 -1.0 0.0> iff "eight_.iff" once
         interpolate 2.0
         use_colour
      }
      translate <-0.5 -0.5 -0.5>
      scale <4.0 4.0 4.0>
      rotate <-30.0 20.0 -45.0>
      translate <0.0 5.0 0.0>
   }
}

object {
   sphere { <0.0 5.0 10.0> 5.0 }
   colour red 1.0 green 1.0 blue 1.0
   texture {
      colour red 1.0 green 1.0 blue 1.0
      ambient 0.2
      diffuse 0.8
      brilliance 1.0 
      phong 1.0      
      phong_size 80
      reflection 0.15
   }
}

// stick
composite {
   object {
      intersection {
         quadric {
            <900.0 900.0 -1.0>
            <0.0 0.0 0.0>
            <0.0 0.0 0.0>
            0.0
            translate <0.0 0.0 -30.0>
         }
         plane { <0.0 0.0 -1.0> 0.0 }
      }
      texture {
         wood
         turbulence 0.02             // dmf
         translate <50.0 -50.0 0.0>
         //         scale <0.3 0.3 1000>      // vs
         scale <0.1 0.1 1000>        // dmf
         //         rotate <-10.0 0.0 45.0>   // vs
         rotate <-10.0 0.0 15.0>     // dmf
         ambient 0.15
         diffuse 0.8
         phong 1.0      
         phong_size 20
         reflection 0.15
      }
   }

   // Blue tip on stick
   object {
      intersection {
         sphere { <0.0 0.0 -0.2> 1.1 }
         plane { <0.0 0.0 1.0> 0.0 }
         plane { <0.0 0.0 -1.0> 0.4 }
      }

      texture {
         colour red 0.2 green 0.5 blue 1.0
         ambient 0.15
         diffuse 0.8
      }

   }

   rotate <-10.0 0.0 45.0>
   translate <-4.5 6.0 14.5>
}

object {
   light_source { <-30.0 30.0 -15.0>
      colour red 1.0 green 1.0 blue 1.0
   }
}
