// Persistence Of Vision raytracer version 1.0 sample file.

// POV-Ray sample image mapper scene file 
// by Alexander Enzmann
// NOTE: Requires "test.gif"


#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#declare Bi = 2.0

#declare Texture2 = /* Cylindrical image map */
texture { image_map { 2 gif "test.gif" once interpolate Bi } }

#declare Texture5 = /* Torus image map */
texture { image_map { 5 <6.325 1 0> gif "test.gif" interpolate Bi } }

camera {
   location  <0.0  0.0 -90.0>
   direction <0.0  0.0  1.0>
   up        <0.0  0.5  0.0>
   right     <0.6666 0.0  0.0>
}

object {
   light_source { <0 30 -200>
      colour White
   }
}


object {
   quadric { Cylinder_Y }
   clipped_by { box {<-2 0 -2> <2 1 2> } }
   texture {
      Texture2
   }
   translate <0 -0.5 0>
   scale <1 2 1>
   scale <7 7 7>
   rotate <40 -60 0>
   translate <-15 15 20>
   color Red
}


object {
   quadric { Hyperboloid_Y
      translate <0 1 0> scale <1 0.5 1>
      texture { Texture2 }
      scale <1 2 1> translate <0 -1 0>
   }
   clipped_by { box {<-2 0 -2> <2 1 2> } }
   scale <5 7 5>
   rotate <-40 -90 0>
   translate <-15 -15 20>
   color Red
}


/* Torus having major radius sqrt(40) = 6.3246, minor radius sqrt(12) = 3.4641 */
object {
   quartic {
      < 1.0   0.0   0.0    0.0     2.0   0.0   0.0   2.0   0.0 -104.0
      0.0   0.0   0.0    0.0     0.0   0.0   0.0   0.0   0.0    0.0
      1.0   0.0   0.0    2.0     0.0  56.0   0.0   0.0   0.0    0.0
      1.0   0.0 -104.0   0.0   784.0 >
   }
   bounded_by {
      sphere { <0 0 0> 10 }
   }
   texture { Glossy color White }
   texture { Texture5 }
   rotate <0 -90 0>
   rotate <-20 0 0>
   translate <15 15 20>
   color Red
}

object {
   quadric { Paraboloid_Y }
   texture {
      Texture2
   }
   clipped_by { box {<-2 0 -2> <2 1 2> } }
   translate <0 -0.5 0>
   scale <1 2 1>
   scale <8 8 8>
   rotate <-40 0 0>
   translate <15 -15 20>
   color Red
}
