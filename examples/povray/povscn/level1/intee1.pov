// Persistence of Vision Raytracer
// Intersection of two cylinders,  with holes cut from each one, in pink
// marblized stone.  Image by Dan Farmer.
// Demonstrates CSG intersection,  layered stone texture.
// Time: about 3 hours @640x480+a0.2 on 486/33

#include "colors.inc"           // Standard colors library
#include "shapes.inc"           // Commonly used object shapes
#include "textures.inc"         // LOTS of neat textures.  Lots of NEW textures.

camera {
   location <0.0  10  -26>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 0 0>
}


// Light source
object { light_source { <-10 20 -25> color White  }}
object { light_source { <0 0 0> color White  }}

#declare Color1A = color DustyRose
#declare Color1B = color DarkSlateGray

object {
   intersection {
      quadric { Cylinder_X }
      quadric { Cylinder_Y }
      quadric { Cylinder_X inverse scale <1 0.5 0.5> }  // small hole
      quadric { Cylinder_Y inverse scale <0.5 1 0.5> }  // ditto
   }

   // A great looking pinkish marble.  Except that it doesn't use marble
   // or agate... it's all done with bozo and granite!

   // Underlying surface is very subtly mottled with bozo in pinks and
   // grays.
   texture {
      ambient 0.25
      bozo
      turbulence 0.25
      color_map {
         [0.0 1.0 color red 0.9 green 0.75 blue 0.75
         color red 0.6 green 0.6 blue 0.6 ]
      }
      scale <.4 .4 .4>
   }

   // Second layer texture has some alpha values, yet a fair amount of color
   // Viening is kept quite thin.
   texture {
      Glossy
      granite
      color_map {
         [0.0 0.9 color Color1A alpha 1 color Color1A alpha 0.5 ]
         [0.9 1.0  color Color1B color Color1B ]
      }
      scale <2 2 2>
      phong 0.25
      phong_size 75
      brilliance 4
   }


   scale <10 10 10>
   rotate <0 45 0>
}

