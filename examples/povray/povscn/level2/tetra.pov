// Persistence Of Vision raytracer version 1.0 sample file.

// By ???

#include "shapes.inc"
#include "shapes2.inc"
#include "colors.inc"
#include "textures.inc"

camera {
   location <0.0 30.0 -200.0>
   direction <0.0 -0.15 1.0>
   up <0.0 1.0 0.0>
   right <1.333 0.0 0.0>
}

#declare Tetra = object {
   intersection { Tetrahedron }

   texture {
      0.01
      colour Gold
      ambient 0.1
      diffuse 0.7
      reflection 0.4
      brilliance 6.0
   }
   colour Gold
}

object { Tetra
   scale <10.0 10.0 10.0>
   rotate <0.0 -45.0 0.0>
   translate <-10.0 0.0 -105.0>
}

object { Tetra
   scale <10.0 10.0 10.0>
   rotate <0.0 -40.0 0.0>
   translate <75.0 0.0 50.0>
}

object { Tetra
   scale <10.0 10.0 10.0>
   rotate <0.0 30.0 0.0>
   translate <-60.0 0.0 -50.0>
}

object { Tetra
   scale <10.0 10.0 10.0>
   rotate <0.0 -75.0 0.0>
   translate <60.0 0.0 -65.0>
}

object {
   sphere { <-50.0 50.0 100.0> 100.0 }
   texture {
      0.01
      colour White
      ambient 0.05
      diffuse 0.1
      reflection 0.97
      brilliance 4.0
   }
   colour White
}

object {
   plane { <0.0 1.0 0.0> -50.0 }
   texture {
      0.01
      checker colour Blue colour Gray
      scale <40.0 40.0 40.0>
      ambient 0.3
      diffuse 0.7
      reflection 0.1
      brilliance 3.0
   }
   colour Blue
}

object {
   plane { <0.0 1.0 0.0> 500.0 }
   texture {
      0.05
      Bright_Blue_Sky
      scale <2000.0 500.0 1000.0>
      ambient 0.7
      diffuse 0.0
   }
   colour red 0.5 green 0.5 blue 0.8
}

object {
   light_source { <60.0 60.0 -200.0>
      colour White
   }
}

object {
   light_source { <-60.0 60.0 -200.0>
      colour White
   }
}
