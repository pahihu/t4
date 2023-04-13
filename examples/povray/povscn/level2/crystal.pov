// Persistence Of Vision raytracer version 1.0 sample file.

//---------------------------------------------------------------------------
// This scene file was designed to emulate the digitized photographic image
// of a crystal sphere { on a checkerboard that David Buck took, and to
// verify or refute the correctness of the current refractive functions
// in POV-Ray.  The original image is available on CompuServe
// (go comart), Library 17 by the name of crysta.gif.
// Experiment with the index-of-refraction value for the crystal ball.
//---------------------------------------------------------------------------

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

max_trace_level 5

camera {
   //    location <-0.75 12.5 -28>
   location <-0.85 12.5 -28>
   direction <0  0  4.125>
   up  <0 1 0>
   right <1.33333 0 0>
   look_at <0.25 0.15  0>
}

object { light_source { <-5 50 -5> colour red 0.85 green 0.85 blue 0.85 }}
object { light_source { <-500 500 -500> colour DimGray }}
// light (under checkerboard, for background
object { light_source { <10 -50 5> colour White } }

// The background.  Designed to give the shaded quality of the photo
object {
   quadric { Ellipsoid 
      scale <10000 500 500>
      rotate <0 60 0>
   }
   colour Blue
   texture { 0.025
      ambient 0.2
      diffuse 0.75
      color Gray
   }
}

composite {
   object {
      intersection { Cube
         scale <5 0.001 7>
      }
      colour Gray
      texture { 0.015
         checker color Black color White
         ambient 0.35
         diffuse 0.65
         translate <1 0 7>
      }
   }


   object {
      sphere { <-0.25  2.15 -4.25> 2.15  }
      colour White
      texture {
         color red 1.0 green 1.0 blue 1.0  alpha 0.75
         ambient 0.2
         diffuse 0.0
         reflection 0.12
         refraction 1.0
         ior 1.45
         specular 1.0 roughness 0.001
      }
   }
   rotate <0 0 -6 >   /* Simulate the slight camera tilt in the photo */
}
