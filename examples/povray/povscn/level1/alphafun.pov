// Persistence Of Vision raytracer version 1.0 sample file.
// Fun with alpha (and other neat tricks).

#include "shapes.inc"
#include "colors.inc"

camera {
   location <0.0  2.0  -4.5>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 0.4 0>
}

// Floor, with phoney gray "planks"
object {
   plane { <0.0 1.0 0.0> 0.0 }
   texture {
      gradient <1 0 0>
      color_map {
         [0.0 0.25 color Gray color Gray]
         [0.25 0.50 color DimGray color LightGray ]
         [0.50 0.75 color LightGray color Gray]
         [0.75 1.0 color Gray color Gray]
      }
      scale <0.45 1 1>
      ambient 0.1
      diffuse 0.7
   }
}


//  Note: Clear = color White alpha 1

// A blobby sphere
object {
   sphere { <0.0  1.0  -1.5>  1.0 }
   texture {
      bozo
      turbulence 0.5
      octaves 1
      scale <0.2 0.2 0.2>
      color_map {
         [0.0 0.5 color red 1 alpha 0.5 color red 1 alpha 1]
         [0.5 1.0 color Clear color Clear ]
      }
      ambient 0.15
      diffuse 0.7
   }
}

// A sliced green box
object { box { UnitBox
      scale <1 1 1>
      rotate <0 45 0>
      translate <-4 1 4>
   }
   texture {
      gradient <0 1 0>
      color_map {
         [0.0 0.5 color Green color Green ]
         [0.5 1.0 color Clear color Clear ]
      }
      scale <0.5 0.5 0.5>
   }
}

// A yellow, swirly finite cylinder
object { intersection { Disk_Y
      scale <1 1 1>
      translate <4 1 4>
   }
   texture {
      gradient <0 1 0 >
      turbulence 2
      octaves 1
      color_map {
         [0.0 0.5 color Yellow color Yellow ]
         [0.5 1.0 color Clear color Clear ]
      }
      scale <0.5 0.5 0.5>
   }
}

object {
   light_source { <10.0  12.0  -40.0>
      colour White
   }
}
