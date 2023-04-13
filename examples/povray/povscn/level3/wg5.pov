// Persistence Of Vision raytracer version 1.0 sample file.

// Wine glass and chessboard
// Image by Dan Farmer
#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

camera {
   location <50.0  55.0  -75.0>
   direction <0.0 0.0  2.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <-10.0 18.0 0.0>
}

object {
   light_source { <10.0  50.0  50.0>
      colour White
   }
}
object {
   light_source { <-35.0  30.0  -150.0>
      colour White
   }
}

#declare Glass = texture {
   color red 1.0 green 1.0 blue 1.0 alpha 1.0
   ambient 0.0
   diffuse 0.0
   reflection 0.1
   refraction 1.0
   ior 1.33
   phong 0.3
   phong_size 90
}

#declare Wine = color red 1.0 alpha 0.85
#declare Liquid = texture {
   reflection 0.05
   refraction 1.0
   ior 1.2
}

#declare Bead = object {
   quadric { Ellipsoid scale <1.65 0.65 1.65> }
   color red 1.0 blue 1.0
   texture { Glass2 }
}

/*
   Torus generated using Truman's Torus Generator 1.0 (c) 1991 Truman Brown
   Torus specs:                                                                  
      Major radius: 0.975000
      Minor radius: 0.025000
      Outer radius: 1.000000
      Inner radius: 0.950000
*/

#declare Rim = object {
   quartic {
      <    1.000000 0.000000 0.000000 0.000000 2.000000
      0.000000 0.000000 2.000000 0.000000 -1.902500
      0.000000 0.000000 0.000000 0.000000 0.000000
      0.000000 0.000000 0.000000 0.000000 0.000000
      1.000000 0.000000 0.000000 2.000000 0.000000
      1.900000 0.000000 0.000000 0.000000 0.000000
      1.000000 0.000000 -1.902500 0.000000 0.902500 >
   }
   clipped_by { plane { <0.0 -1.0 0.0> 0.0 } }
   scale <15.0 10.0 15.0>
   translate <0.0 24.0 0.0>
   color red 1.0
   texture { Gold_Texture }
}

#declare Top = object {
   intersection {
      plane { <0.0 1.0 0.0> 1.0  }
      quadric { QCone_Y }
      quadric { QCone_Y scale <0.97 1.0 0.97> inverse }
   }
   clipped_by {
      plane { <0.0 1.0  0.0> 0.0  inverse }
   }
   scale <15.0 10.0 15.0>
   translate <0.0 14.0 0.0>
   color green 1.0
   texture { Glass }
}

#declare Stem = object {
   quadric { Cylinder_Y }
   clipped_by {
      plane { <0.0 1.0 0.0> -1.0 inverse }
      plane { <0.0 1.0 0.0>  1.0 }
   }
   scale <1.0 12.0 1.0>
   translate <0.0 6.0 0.0>
   color blue 1.0
   texture { Glass2 }
}


#declare Base = object {
   quadric { QCone_Y }
   clipped_by {
      plane { <0.0 1.0  0.0> 0.0  }
      plane { <0.0 1.0 0.0> -1.0 inverse }
   }
   translate <0.0 1.0 0.0>   /* This actually puts the base of the cone on y=0*/
   scale <12.0 3.25 12.0>
   translate <0.0 0.0 0.0> /*Does nothing: just for clarity */
   color red 1.0 green 1.0
   texture { Glass2 }
}

#declare Wine = object {
   intersection {
      plane { <0.0 1.0 0.0> 0.9
      }
      quadric { QCone_Y scale <0.95 1.0 0.95> }
   }
   clipped_by {
      plane { <0.0 1.0  0.0> 0.0  inverse
      }
   }
   scale <14.9 10.0 14.9>
   translate <0.0 14.0 0.0>
   color White
   texture { Liquid
      color Wine
   }
}

/* Ground plane */
object {
   plane { <0.0 1.0 0.0> -0.0 }
   color DimGray
   texture { 0.2 color RichBlue }
}

#declare Frame = object {
   union {
      intersection { Disk_X    /* Front rounded edge */
         scale <240.0 4.5 4.5>
         translate<-120.0 0.0 -120.0>
      }
      intersection { Disk_Z    /* Left rounded edge */
         scale <4.5 4.5 240.0>
         translate <-120.0 0.0 -120.0>
      }
      intersection { Disk_X    /* Rear rounded edge */
         scale <240.0 4.5 4.5>
         translate<-120.0 0.0 +120.0>
      }
      intersection { Disk_Z    /* Right rounded edge */
         scale <4.5 4.5 240.0>
         translate <+120.0 0.0 -120.0>
      }
   }
   color red 1.0
   texture { Silver_Texture }
}

#declare ChessBoard = object {
   intersection { Cube
      scale <120.0 4.0 120.0>
   }
   color Cyan
   texture { White_Marble scale <20.0 1.0 20.0> reflection 0.12 }
   texture { checker color Black color White alpha 1.0
      scale <30.0 4.001 30.0>
      reflection 0.12
   }
}


#declare WineGlass = composite {
   object { Rim   }
   object { Wine  }
   object { Top   }
   object { Bead  translate <0.0 14.5 0.0> }
   object { Bead  translate <0.0 10.0 0.0> }
   object { Bead  translate <0.0  7.0 0.0> }
   object { Bead  translate <0.0  3.0 0.0> }
   object { Stem  }
   object { Base  }
   bounded_by {
      intersection { Disk_Y
         scale <15.5 25.0 15.5>
      }
   }
   scale <1.0 1.50 1.0>
}

composite {
   object { Frame }
   object { ChessBoard }
   translate <0.0 4.0 145.0>
}

composite { WineGlass }

/*End of file */
