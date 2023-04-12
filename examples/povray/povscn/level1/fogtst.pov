// Persistence Of Vision raytracer version 1.0 sample file.

#include "colors.inc"
#include "textures.inc"

camera {
   location <0.0  30.0  -100.0>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
}

fog {
   colour red 0.2 green 0.2 blue 0.22
   150.0
}

/* Put down a new twist in the beloved famous raytrace g/y checked floor */
object {
   plane { <0.0 1.0 0.0> 0.0 }
   texture {
      checker colour White colour Black
      ambient 0.2
      diffuse 0.8
      scale < 20.0 20.0 20000.0 >
      translate <0 0 -10000>
   }
   colour Yellow
}
// Forground sphere
object {
   sphere { <0.0  40.0  40.0>  40.0 }

   texture {
      Phong_Glossy
      ambient 0.2
      diffuse 0.7
      colour Red
   }
   colour Red
}

// Near distant sphere
object {
   sphere { <100.0  50.0  160.0>  30.0 }

   texture {
      Glossy
      colour Coral
      ambient 0.15
      diffuse 0.75
   }
   colour Red
}

// Far distant sphere
object {
   sphere { <-100.0  150.0  250.0>  20.0 }
   texture {
      Glossy
      colour Magenta
      ambient 0.15
      diffuse 0.75
   }
   colour Magenta
}


object {
   light_source { <100.0  120.0  40.0>
      colour White
   }
}
