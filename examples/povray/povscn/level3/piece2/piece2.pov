// Persistence Of Vision Raytracer version 1.0 sample file.

// piece 2: 
//          by Truman Brown 11/91

//          Close-up of a museum piece in the Woild Museum

#include "colors.inc"
#include "textures.inc"
#include "shapes.inc"

#include "light.inc"
#include "ttexture.inc"
#include "tori2.inc"
#include "spural1.inc"
#include "spural2.inc"

composite {
   object { light3 translate < -43  14  -80>  }
   object { light3 translate < 134 223  -20>  }
   object { light4 translate <  28  88 -259>  }
}

camera {
   location  < 15.0     23.0 -55.0>
   direction <  0.0      0.0   2.0  >
   up        <  0.0      1.0   0.0  >
   right     <  1.33333  0.0   0.0  >
   look_at   <  0.0     -0.5   0.0  >
}

#declare orb =
composite {
   object { torus7 translate < 0.0  9.510565 0.0 > texture { oak  }  }
   object { torus5 translate < 0.0  8.090170 0.0 > texture { oak  }  }
   object { torus3 translate < 0.0  5.877853 0.0 > texture { oak  }  }
   object { torus1 translate < 0.0  3.090170 0.0 > texture { oak  }  }
   object { torus  translate < 0.0  0.000000 0.0 > texture { oak  }  }
   object { torus1 translate < 0.0 -3.090170 0.0 > texture { oak  }  }
   object { torus3 translate < 0.0 -5.877853 0.0 > texture { oak  }  }
   object { torus5 translate < 0.0 -8.090170 0.0 > texture { oak  }  }
   object { torus7 translate < 0.0 -9.510565 0.0 > texture { oak  }  }
   object { sphere { <0 0 0> 4.3  } texture { color White ambient 0.0 diffuse 0.1 reflection 0.98 specular 1.0 roughness 0.0  }  }

   object { torus9 rotate <90  60 0> texture { brace_texture  }  }
   object { torus9 rotate <90 120 0> texture { brace_texture  }  }
   object { torus9 rotate <90 180 0> texture { brace_texture  }  }
   bounded_by {
      sphere { <0 0 0> 11.0  }
   }
   rotate <0 0 25>
}

#declare plate =
object {
   intersection {
      quadric { Cylinder_Y scale <15 1 15>  }
      plane { <0 1 0> 0  }
      plane { <0 -1 0> 1  }
   }
   texture {
      gilt_texture
   }
}

#declare plate_border =
object {
   intersection {
      quadric { Cylinder_Y scale <17 1 17>  }
      quadric { Cylinder_Y scale <15 1 15> inverse  }
      plane { <0 1 0> 0  }
      plane { <0 -1 0> 1  }
   }
   texture { oak  }
}

composite { orb translate <0.0 3.0 0.0>  }
composite { spural1 rotate <0 -28 0> translate < 8.9 -12.5 -0.5>  }
composite { spural2 rotate <0 212 0> translate <-8.9 -12.5 -0.5>  }

object { plate translate <0 -12.7 0>  }
object { plate_border translate <0 -12.7 0>  }
