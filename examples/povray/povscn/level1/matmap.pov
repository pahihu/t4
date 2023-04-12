// Persistence Of Vision raytracer version 1.0 sample file.

// a very simple file to demonstrate material_map  - CdW 
// 7/29/91 
// NOTE: Uses POVMAP.GIF

#include "colors.inc"
#include "textures.inc"

max_trace_level  2

camera {
   location <15.0  90.0  -135.0>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 15 0>
}

object { sphere { <0.0  0.0  0.0>  1.0 }

   texture { /* Declare it like a normal image map */
      material_map { <1.0 -1.0 0.0> gif "povmap.gif"


         /* Now a list of textures to map with instead of colors */

         // Wood
         texture {       // #1
            DMFLightOak
         }

         // Bumpy Granite
         texture {       // #2
            granite
            dents 1
            scale <.35 .35 .35>
            specular 0.75
         }


         // Multi-hued gradient
         texture {       // #3
            gradient < 0.0 1.0 0.0 >
            colour_map {
               [0.00 0.33  colour red 1.0 green 0.0 blue 0.0
               colour red 0.0 green 0.0 blue 1.0]
               [0.33 0.66  colour red 0.0 green 0.0 blue 1.0
               colour red 0.0 green 1.0 blue 0.0]
               [0.66 1.001 colour red 0.0 green 1.0 blue 0.0
               colour red 1.0 green 0.0 blue 0.0]
            }
            metallic
            specular 0.75
            scale <8.0 8.0 8.0>
         }

         // Mirror
         texture {       // #4
            Mirror
            metallic
            brilliance 8
         }

      }             /* --- end of material list --- */
      // transform the texture
      translate <-0.5 -0.5 0>     // Center the image
   }
   // Transform the texture with the sphere
   scale <50 50 50>
   rotate <15 -15 0>
   translate <0 50 0>
}

// Small sphere on the left
object {
   sphere {<0 0 0> 25 }
   texture { Chrome_Metal reflection 0.85 }
   translate <-80 25 0>
   rotate <0 -55 0>
}


object {
   plane { <0.0 1.0 0.0> 0.0 }
   texture {
      color GreenCopper
      ambient 0.15
      diffuse 0.7
   }
}

object {
   light_source { <100.0  140.0  -130.0>
      colour White
   }
}
