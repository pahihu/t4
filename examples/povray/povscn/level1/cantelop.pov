// Persistence of Vision Raytracer
// Cantelope segments.  Uses onion for the cantelope interior and skin.
// Demonstrates intersection of spheres and planes, onion texture,
// color maps.
// Time: about 20 minutes @640x480+a0.2 on 486/33.

#include "colors.inc"           // Standard colors library

#declare Melon =    texture {
   ambient 0.2
   onion
   color_map {
      [0.0   0.95 color Orange       color Orange ]
      [0.95  0.96 color Orange       color GreenYellow ]
      [0.96  0.98 color GreenYellow  color Khaki  ]
      [0.98  1.00 color NewTan       color DarkTan ]
   }
}

camera {
   location <-2  3  -3>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 0 0>
}


// Light source
#declare Grayscale = 0.25
#declare AmbientLight = color red Grayscale green Grayscale blue Grayscale

object { light_source { <-20 30 -100> color White  }}

object { light_source { <0 50 10> color AmbientLight }}


// Flat-topped sphere/plane intersection
#declare MelonHalf = object {
   intersection {
      sphere { <0 0 0> 1 }                // outer wall
      sphere { <0 0 0> 0.65 inverse }      // inner wall
      plane { <0 1 0> 0 }                 // top surface
   }
   texture { Melon }
   bounded_by {
      sphere { <0 0 0> 1.001 }
   }
}

// Quarter Wedge of above melon
#declare MelonWedge = object {
   intersection {
      sphere { <0 0 0> 1 }                // outer wall
      sphere { <0 0 0> 0.65 inverse }      // inner wall
      plane { <0  1 0> 0 rotate < 45 0 0>}                 // top surface
      plane { <0  1 0> 0 rotate <-45 0 0>}                 // top surface
   }
   texture { Melon }
   bounded_by {
      sphere { <0 0 0> 1.001 }
   }
}

object { MelonHalf }
object { MelonWedge rotate <0 30 0> translate <2 0 2> }
