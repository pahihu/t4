// Persistence of Vision Raytracer

#include "colors.inc"           // Standard colors library
#include "textures.inc"         // LOTS of neat textures.  Lots of NEW textures.

camera {
   //   location <0.0  1.5  -3.0>
   location <0.0  1.5  -2.0>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 0 0>
}

// Light source (backlit)
object { light_source { <0 30 10> color White  }}


// A "sky sphere" to reflect in the glass
object { sphere { <0 0 0> 1000 } texture { color DimGray } }

// Crystal dish
object {
   intersection {
      sphere { <0 0 0> 1 }
      sphere { <0 0.25 0> 0.75 inverse}
      plane {<0 1 0> 0.75 }
   }
   texture { Glass3 reflection 0.25 }
   bounded_by { sphere { <0 0 0> 1.1 } }
}


object { plane { <0 1 0> -1 }
   texture {
      DMFDarkOak
      scale <0.75 0.75 1>
      translate <10 -0.45 10>
      rotate <5.0 30 0.25>
      ambient 0.05
      diffuse 0.5
      reflection 0.35
      specular 0.9
      roughness 0.005
   }

}
