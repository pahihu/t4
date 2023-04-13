// Persistence of Vision Raytracer
// Height field sample
// by Alexander Enzmann

#include "colors.inc"
#include "textures.inc"

camera {
   location <0.0  150.0  -250.0>
   direction <0 0 2.0>
   up        <0 1 0>
   right     <1.33 0 0>
   look_at   <0 0 50>
   }

// Define the sombrero shape based on a Targa height field
object {
   height_field { tga "hfsomb.tga" }
   texture {
      White_Marble 
      ambient 0.15
      diffuse 0.7
      phong 1.0
      }
   translate <-.5 -.5 -.5> // Center it on the origin
   scale <160 100 160> // Scale it to old size
   rotate <-15 0 0>
   }

/*object {
   plane { <0.0  1.0  0.0> -60.0 }
   texture {
      colour red 1
      ambient 0.2
      diffuse 0.1
      }
   }*/

// Shed some lights
object {
   light_source {
      <-450 250 -40>
      color red 0.7 green 0.7 blue 0.7
      }
   }

object {
   light_source {
      <0 250 0>
      color red 0.7 green 0.7 blue 0.7
      }
   }
