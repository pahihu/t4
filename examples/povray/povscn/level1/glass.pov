// Persistence Of Vision raytracer version 1.0 sample file.

#include "colors.inc"
#include "textures.inc"

camera {
   location <0.0  20.0  -125.0>
   direction <0.0 0.0  2.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0.0 25.0 0.0> 
}

object {
   sphere { <0.0  25.0  0.0>  20.0 }
   texture { 
      color red 1.0 green 1.0 blue 1.0 alpha 1.0
      ambient 0.0
      diffuse 0.0
      specular 0.5
      roughness 0.001
      reflection 0.2
      refraction 1.0
      ior 1.5
   }
}

object { 
   sphere { <0.0 0.0 0.0> 200.0 }
   texture {
      Bright_Blue_Sky
      scale <30.0 10.0 10.0>
      ambient 1
      diffuse 0.8
   }
}

object {
   light_source { <100.0  140.0  -130.0>
      colour red 0.7 green 0.7 blue 0.7
   }
}
