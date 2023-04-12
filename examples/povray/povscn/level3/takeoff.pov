// Persistence Of Vision raytracer version 1.0 sample file.

// Sample quartic file
// by David Mason (of DTA fame!)

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "shapesq.inc"

#declare Steel = texture {
   color SteelBlue
   phong 1.0
   phong_size 20
   ambient 0.2
   diffuse 0.8
}

#declare Fin = object {
   difference {
      intersection {
         quadric { Ellipsoid scale <2 4 0.5> }
         plane { <0.0 1.0 0.0> -0 inverse }
         plane { <1.0 0.0 0.0> -0 }
      }
      quadric { Ellipsoid scale <2 2 2> }
   }
   translate <-1.2 0 0>
   texture { Steel }
}

#declare Fins = composite {
   object { Fin rotate <0 0 0>   }
   object { Fin rotate <0 90 0>  }
   object { Fin rotate <0 180 0> }
   object { Fin rotate <0 270 0> }
   bounded_by {
      intersection {
         quadric { Cylinder_Y scale <3.5 1 3.5> }
         plane { <0.0 1.0 0.0> 4 }
         plane { <0.0 1.0 0.0> 0 inverse }
      }
   }
}

composite { Fins rotate <0 25 0> translate <0 6 0> }


object {
   intersection {
      union {
         intersection {
            quartic { Piriform
               translate <-0.75 0 0>
               rotate <0 0 -90>
               scale <2 6 2>
               texture { Steel }
            }
            quadric { QCone_Y
               scale <1.8 5.0 1.8>
               rotate <180 0 0>
               translate <0 5 0>
               color Clear
            }
            plane { <0.0 1.0 0.0> 5 color Clear }
            plane { <0.0 1.0 0.0> 0 inverse color Clear }
         }
         intersection {
            quartic { Piriform
               translate <-0.75 0 0>
               rotate <0 0 90>
               scale <2 18 2>
               texture { Steel }
            }
            quadric { QCone_Y
               scale <1.8 15.0 1.8>
               translate <0 -15 0>
               color Clear
            }
            plane { <0.0 1.0 0.0> 0 color Clear }
            plane { <0.0 1.0 0.0> -8 inverse color Clear }
         }
         intersection {
            quartic { Piriform
               translate <-0.40 0 0>
               rotate <0 0 90>
               scale <2 6 2>
               texture { Steel }
            }
            plane { <0.0 1.0 0.0> 0 inverse }
            translate <0 -11 0>
         }
      }
      quadric { Cylinder_Y scale <1.45 1.0 1.45> color Clear }
   }
   bounded_by {
      intersection {
         quadric { Cylinder_Y scale <1.45 1.0 1.45> }
         plane { <0.0 1.0 0.0> 5 }
         plane { <0.0 1.0 0.0> -11 inverse }
      }
   }
   translate <0 18 0>
}

object {
   union {
      intersection {
         quartic { Piriform
            texture { Luminous color Yellow alpha 0.7 }
         }
         quadric { Cylinder_Y scale <0.8 1.0 0.8> }
      }
      light_source { <0 0 0> color Yellow }
   }

   translate <-0.40 0 0>
   rotate <0 0 90>
   scale <0.8 5 0.8>

   bounded_by {
      intersection {
         quadric { Cylinder_Y scale <0.8 1.0 0.8> }
         plane { <0.0 1.0 0.0> 6 }
         plane { <0.0 1.0 0.0> -8 inverse }
      }
   }
   translate <0 4 0>
}

camera {
   location  <3.0  30 -20.0>
   up        <0.0  1.0  0.1>
   right     <1.33 0.0  0.0>
   look_at   <0.0  12 0>
}

object {
   light_source { <100 100 -100>
      colour White
   }
}

object {
   plane { <0.0 1.0 0.0> 0 }
   texture {
      checker
      colour Gold
      colour Aquamarine
      scale < 5 5.1 5 >
      reflection 0.5
      ambient 0.1
      diffuse 0.8
   }
}

object {
   sphere { <0.0  0.0  0.0> 300.0 }
   texture {
      gradient <0.0  1.0  0.0>
      colour_map { [0.0 0.8  colour red 1.0 green 0.3 blue 0.0
         colour red 0.7 green 0.7 blue 1.0]
         [0.8 1.0 colour red 0.7 green 0.7 blue 1.0
         colour red 0.7 green 0.7 blue 1.0]
      }
      scale <300.0  300.0  300.0>
      ambient 0.7
      diffuse 0.0
   }
   colour red 0.7  green 0.7 blue 1.0
}

object {
   sphere { <0.0  0.0  0.0> 259.0 }
   texture {
      bozo
      turbulence 0.5
      colour_map {
         [0.0 0.6  colour red 1.0 green 1.0 blue 1.0 alpha 1.0
         colour red 1.0 green 1.0 blue 1.0 alpha 1.0]
         [0.6 0.8 colour red 1.0 green 1.0 blue 1.0 alpha 1.0
         colour red 1.0 green 1.0 blue 1.0]
         [0.8 1.001 colour red 1.0 green 1.0 blue 1.0
         colour red 0.8 green 0.8 blue 0.8]
      }
      scale <100.0  20.0  100.0>
      ambient 0.7
      diffuse 0.0
   }
   colour red 0.7 green 0.7 blue 1.0
}
