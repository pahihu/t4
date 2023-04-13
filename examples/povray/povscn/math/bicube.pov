// Persistence Of Vision raytracer version 1.0 sample file.

// Sample quartic { file
// by Alexander Enzmann


#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

// a cubic { shape {, like a cube with smoothed edges in appearance 
object {
   quartic {
      < 1.0   0.0   0.0    0.0     0.0   0.0   0.0   0.0   0.0    0.0
      0.0   0.0   0.0    0.0     0.0   0.0   0.0   0.0   0.0    0.0
      1.0   0.0   0.0    0.0     0.0   0.0   0.0   0.0   0.0    0.0
      1.0   0.0   0.0    0.0  -1000.0 >
      rotate <20.0 40.0 30.0>
   }
   texture {
      color Red
      phong 1.0
      phong_size 10
      ambient 0.2
      diffuse 0.8
   }
   rotate <-45 0 0>
   translate <0 0 20>
   color Red
}

// Put down checkered floor 
/*
object
   plane <0.0  1.0  0.0> -20.0 end_plane
   texture
      checker colour NavyBlue colour MidnightBlue
      scale < 20.0 20.0 20.0 >
      ambient 0.8
      diffuse 0.2
   end_texture
end_object
*/
camera {
   location  <0.0  2.0 -10.0>
   look_at   <0.0  0.0   0.0>
   up        <0.0  1.0   0.0>
   right     <1.33 0.0   0.0>
}

object {
   light_source { <50 100 0>
      colour White
   }
}

object {
   light_source { <-200 30 -300>
      colour White
   }
}
