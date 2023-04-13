// Persistence Of Vision raytracer version 1.0 sample file.

// Imagemap interpolation test by CdW!
// NOTE: Requires "test.gif"

#include "colors.inc"

#declare Bilinear = 2.0
#declare Norm_Dist = 4.0

camera {
   location <0.0  0.0  -25.0>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 0 0>
}

// Left hemisphere: Bilinear
object {
   sphere { <0.0  0.0  0.0>  1.0 }
   clipped_by { plane { <1 0 0> 0 } }
   texture {				    // x-y oriented bitmap image 
      color Red
      image_map { gif "test.gif"
         interpolate Bilinear
      }
      translate <-0.5 -0.5 0>
      ambient 0.2
      diffuse 0.7
   }
   scale <10 10 10>
   translate <-1 0 0>
}

// Right hemisphere: norm_dist
object {
   sphere { <0.0  0.0  0.0>  1.0 }
   clipped_by { plane { <-1 0 0> 0 } }
   texture {                    // x-y oriented bitmap image
      color Blue
      image_map { gif "test.gif"
         interpolate Norm_Dist
      }
      translate <-0.5 -0.5 0>
      ambient 0.2
      diffuse 0.7
   }
   scale <10 10 10>
   translate <1 0 0>
}


object {
   light_source { <100.0  120.0  -130.0>
      colour White
   }
}

