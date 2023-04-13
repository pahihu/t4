// Persistence Of Vision raytracer version 1.0 sample file.

// Sample qaurtic file 
// by Alexander Enzmann

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "shapesq.inc"

object {
   quartic { Piriform sturm }
   rotate <0 0 -90>
   translate <0 0.5 0>
   scale <2 10 2>
   texture {
      color SteelBlue
      phong 1.0
      phong_size 20
      ambient 0.2
      diffuse 0.8
   }
}

camera {
   location  <0.0  0.0 -12.0>
   up        <0.0  1.0  0.0>
   right     <1.33 0.0  0.0>
   look_at   <0.0  0.0  0.0>
}

object {
   light_source { <200 30 -500>
      colour White
   }
}
