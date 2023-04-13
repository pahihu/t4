// Persistence Of Vision raytracer version 1.0 sample file.

// By Drew Wells

// Spline.pov  - This file requires spline.inc

// Drew Wells 1990 cis 73767,1244 

// This is a huge word "Merry" written in cursive over a reflective  
// checkered plain. Normally i avoid using the cliched checkered plane {, 
// but here it looks good with the huge, holiday "Merry". 


#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#include "spline.inc"

camera {
   location <10.0  25.0  -550.0>
   direction <0.0 0.0  3.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <120.0 45.0 0.0>
}

/* Put down the beloved famous raytrace green/yellow checkered floor */
object {
   plane { <0.0 1.0 0.0> 17.0 }
   texture {
      checker
      colour red 1.0 blue 1.0 green 1.0
      colour red 0.1 green 0.6 blue 0.1
      scale < 20.0 20.0 20.0 >
      reflection 0.5
      ambient 0.1
      diffuse 0.8
   }
   colour red 1.0 green 1.0 blue 1.0
}

object {
   union { Merry }
   texture {
      color red 1.0
      ambient 0.1
      diffuse 0.9
   }
}

object {
   light_source { <200.0  120.99  -330.0>
      colour White
   }
}

