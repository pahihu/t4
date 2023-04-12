// Persistence Of Vision raytracer version 1.0 sample file.

// POV-Ray sample image mapper scene file 
// by Alexander Enzmann
// NOTE: Requires "test.gif"



#include "shapes.inc"
#include "colors.inc"

#declare Bi = 2

#declare Texture0 = /* Planar image map */
texture { image_map { 0 gif "test.gif" once interpolate Bi } }

#declare Texture1 = /* Spherical image map */
texture { image_map { 1 gif "test.gif" interpolate Bi } }


object {
   quadric { Ellipsoid }
   texture { Texture1 }
   scale <10 10 10> 
   rotate <0 -90 0>
   translate <-10 0 0>
   color Red
}

object {
   plane { <0 0 1> 0 }
   clipped_by {
      box { <0 0 -1> <1 1 1> }
   }
   texture { Texture0 }
   translate <-0.5 -0.5 0>
   scale <2 2 2>
   scale <10 10 10>
   rotate <20 30 0>
   translate <10 0 0>
   color Red
}

camera {
   location  <0.0  0.0 -90.0>
   direction <0.0  0.0  1>
   up        <0.0  0.5  0.0>
   right     <0.6666 0.0  0.0>
   look_at <0 0 0>
}

object {
   light_source { <0 300 -200>
      colour White
   }
}
