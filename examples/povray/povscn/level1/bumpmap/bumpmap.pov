// Persistence Of Vision raytracer version 1.0 sample file.

// Bumpmap test file by Drew Wells
// NOTE: Calls for "bumpmap_.gif", but any GIF will work.

#include "colors.inc"

camera {
   location <0.0  20.0  -120.0>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0.0 20.0 0.0>
}

object {
   sphere { <0.0  0.0  0.0>  50.0 }

   texture {	
      color Blue 
      bump_map { <1.0 -1.0 0.0> gif "bumpmap_.gif"
         bump_size 5.0
         interpolate 2.0
      }
      scale < 100.0 100.0 100.0 >	 /* scaled and translated so only */
      translate < 50.0 -50.0 0.0 >	 /* 1 instance of the map appears. */
      ambient 0.2
      diffuse 0.7
      specular 0.6
   }
   translate <0.0 25.0 0.0>
}

object {
   plane { <0.0 1.0 0.0> -10.0 }

   texture {
      color Gold
      ambient 0.1
      diffuse 0.5
   }
}

object {
   light_source { <100.0  120.0  -130.0>
      colour White
   }
}

