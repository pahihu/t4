// Persistence Of Vision raytracer version 1.0 sample file.

// By Ville Saari
// Copyright (c) 1991 Ferry Island Pixelboys

/*
*
* POV-Ray scene description for chess board.
* 
* Created: 01-Feb-91
* Updated: 02-Mar-91
*
* This scene has 430 primitives in objects and 41 in bounding shapes and
* it takes over 40 hours to render by standard amiga.
*
* If you do some nice modifications or additions to this file, please send 
* me a copy. My Internet address is:
*
*         vsaari@niksula.hut.fi
*/

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

camera {
   location <59 20 -48>
   direction <0 0 1>
   up <0 1 0>
   right <1.33 0 0>
   look_at <0 0 1>
}

object {
   light_source { <800 600 -200>
      colour White
   }
}

#declare Pawn = union {
   sphere { <0 7 0> 1.5 }

   quadric { Ellipsoid 
      scale <1.2 0.3 1.2>
      translate <0 5.5 0>
   }

   intersection {
      plane { <0 1 0> 5.5 }
      quadric { Hyperboloid_Y
         translate <0 5 0>
         scale <0.5 1 0.5>
      }
      plane { <0 -1 0> -2.5 }
   }

   quadric { Ellipsoid 
      scale <2 0.5 2>
      translate <0 2.3 0>
   }

   intersection {
      sphere { <0 0 0> 2.5 }
      plane { <0 -1 0> 0 }
   }
}

#declare Rook = union {
   intersection {
      union {
         plane { < 1 0 0> -0.5 }
         plane { <-1 0 0> -0.5 }
         plane { < 0 1 0>  9   }
      }

      union {
         plane { <0 0  1> -0.5 }
         plane { <0 0 -1> -0.5 }
         plane { <0 1  0>  9   }
      }

      plane { <0 1 0> 10 }
      quadric { Cylinder_Y scale <2 1 2> }
      quadric { Cylinder_Y scale <1.2 1 1.2> inverse }
      plane { <0 -1 0> -8 }
   }

   intersection {
      plane { <0 1 0> 8 }
      quadric { Hyperboloid_Y
         scale <1 1.5 1>
         translate <0 5.401924 0>
      }
      plane { <0 -1 0> -3 }
   }

   quadric { Ellipsoid 
      scale <2.5 0.5 2.5>
      translate <0 2.8 0>
   }

   intersection {
      sphere { <0 0 0> 3 }
      plane { <0 -1 0> 0 }
   }
}

#declare Knight = union {
   intersection {
      quadric { Cylinder_Z
         scale <17.875 17.875 1>
         translate <-18.625 7 0>
         inverse
      }

      quadric { Cylinder_Z
         scale <17.875 17.875 1>
         translate <18.625 7 0>
         inverse
      }

      quadric { Cylinder_X
         scale <1 5.1 5.1>
         translate <0 11.2 -5>
         inverse
      }

      union {
         plane { <0 1 0> 0
            rotate <30 0 0>
            translate <0 9.15 0>
         }
         plane { <0 0 1> 0
            rotate <-20 0 0>
            translate <0 10 0>
         }
      }

      union {
         plane { <0 -1 0> 0
            rotate <30 0 0>
            translate <0 7.15 0>
         }
         plane { <0 1 0> 0
            rotate <60 0 0>
            translate <0 7.3 0>
         }
      }

      union {
         plane { <0 1 0> 0
            rotate <0 0 -45>
         }
         plane { <0 1 0> 0
            rotate <0 0 45>
         }
         translate <0 9 0>
      }

      quadric { Cylinder_Y scale <2 1 2> }
      sphere { <0 7 0> 4 }
   }

   quadric { Ellipsoid 
      scale <2.5 0.5 2.5>
      translate <0 2.8 0>
   }

   intersection {
      sphere { <0 0 0> 3 }
      plane { <0 -1 0> 0 }
   }
}

#declare Bishop = union {
   sphere { <0 10.8 0> 0.4 }

   intersection {
      union {
         plane { <0 0 -1> -0.25 }
         plane { <0 0  1> -0.25 }
         plane { <0 1  0>  0    }
         rotate <30 0 0>
         translate <0 8.5 0>
      }

      quadric { Ellipsoid 
         scale <1.4 2.1 1.4>
         translate <0 8.4 0>
      }

      plane { <0 -1 0> -7 }
   }

   quadric { Ellipsoid 
      scale <1.5 0.4 1.5>
      translate <0 7 0>
   }

   intersection {
      plane { <0 1 0> 7 }
      quadric { Hyperboloid_Y
         scale <0.6 1.4 0.6>
         translate <0 7 0>
      }
      plane { <0 -1 0> -3 }
   }

   quadric { Ellipsoid 
      scale <2.5 0.5 2.5>
      translate <0 2.8 0>
   }

   intersection {
      sphere { <0 0 0> 3 }
      plane { <0 -1 0> 0 }
   }
}

#declare QueenAndKing = union {
   sphere { <0 10.5 0> 1.5 }

   intersection {
      union {
         sphere { <1.75 12 0> 0.9 rotate <0 150 0> }
         sphere { <1.75 12 0> 0.9 rotate <0 120 0> }
         sphere { <1.75 12 0> 0.9 rotate <0  90 0> }
         sphere { <1.75 12 0> 0.9 rotate <0  60 0> }
         sphere { <1.75 12 0> 0.9 rotate <0  30 0> }
         sphere { <1.75 12 0> 0.9                   }
         sphere { <1.75 12 0> 0.9 rotate <0  -30 0> }
         sphere { <1.75 12 0> 0.9 rotate <0  -60 0> }
         sphere { <1.75 12 0> 0.9 rotate <0  -90 0> }
         sphere { <1.75 12 0> 0.9 rotate <0 -120 0> }
         sphere { <1.75 12 0> 0.9 rotate <0 -150 0> }
         sphere { <1.75 12 0> 0.9 rotate <0 180 0> }
         inverse
      }

      plane { <0 1 0> 11.5 }

      quadric { QCone_Y
         scale <1 3 1>
         translate <0 5 0>
      }

      plane { <0 -1 0> -8 }
   }

   quadric { Ellipsoid 
      scale <1.8 0.4 1.8>
      translate <0 8 0>
   }

   intersection {
      plane { <0 1 0> 8 }
      quadric { Hyperboloid_Y
         scale <0.7 1.6 0.7>
         translate <0 7 0>
      }
      plane { <0 -1 0> -3 }
   }

   quadric { Ellipsoid 
      scale <2.5 0.5 2.5>
      translate <0 2.8 0>
   }

   intersection {
      sphere { <0 0 0> 3 }
      plane { <0 -1 0> 0 }
   }
}

#declare Queen = union {
   sphere { <0 12.3 0> 0.4 }
   union { QueenAndKing }
}

#declare King = union {
   intersection {
      union {
         intersection {
            plane { <0  1 0>  13 }
            plane { <0 -1 0> -12.5 }
         }

         intersection {
            plane { < 1 0 0> 0.25 }
            plane { <-1 0 0> 0.25 }
         }
      }

      plane { < 0  0  1>   0.25 }
      plane { < 0  0 -1>   0.25 }
      plane { < 1  0  0>   0.75 }
      plane { <-1  0  0>   0.75 }
      plane { < 0  1  0>  13.5  }
      plane { < 0 -1  0> -11.5  }
   }

   union { QueenAndKing }
}

#declare WWood = texture {
   wood
   turbulence 0.1
   colour_map {
      [ 0.0 0.35 colour red 0.7  green 0.4
      colour red 0.7  green 0.4  ]
      [ 0.35 1.0 colour red 0.95 green 0.62
      colour red 0.95 green 0.62 ]
   }
   scale <0.6 1000.0 0.6>
   translate <200.0 0.0 100.0>
   specular 1
   roughness 0.02
}

#declare BWood = texture {
   wood
   turbulence 0.1
   colour_map {
      [ 0.0 0.55 colour red 0.45 green 0.25
      colour red 0.45 green 0.25 ]
      [ 0.55 1.0 colour red 0.30 green 0.16
      colour red 0.30 green 0.16 ]
   }
   scale <0.6 1000.0 0.6>
   translate <100.0 0.0 -200.0>
   specular 1
   roughness 0.02
}

#declare WPawn = object {
   union { Pawn }

   bounded_by {
      sphere { <0 4 0> 4.72 }
   }

   texture { WWood }
   colour red 0.95 green 0.62
}

#declare BPawn = object {
   union { Pawn }

   bounded_by {
      sphere { <0 4 0> 4.72 }
   }

   texture { BWood }
   colour red 0.4 green 0.2
}

#declare WRook = object {
   union { Rook }

   bounded_by {
      sphere { <0 5 0> 5.831 }
   }

   texture { WWood }
   colour red 0.95 green 0.62
}

#declare BRook = object {
   union { Rook }

   bounded_by {
      sphere { <0 5 0> 5.831 }
   }

   texture { BWood }
   colour red 0.4 green 0.2
}

#declare WKnight = object {
   union { Knight }

   bounded_by {
      sphere { <0 5 0> 5.831 }
   }

   texture { WWood }
   colour red 0.95 green 0.62
}

#declare BKnight = object {
   union { Knight }
   rotate <0 180 0>

   bounded_by {
      sphere { <0 5 0> 5.831 }
   }

   texture { BWood }
   colour red 0.4 green 0.2
}

#declare WBishop = object {
   union { Bishop }

   bounded_by {
      sphere { <0 5.5 0> 6.265 }
   }

   texture { WWood }
   colour red 0.95 green 0.62
}

#declare BBishop = object {
   union { Bishop }
   rotate <0 180 0>

   bounded_by {
      sphere { <0 5.5 0> 6.265 }
   }

   texture { BWood }
   colour red 0.4 green 0.2
}

#declare WQueen = object {
   union { Queen }

   bounded_by {
      intersection {
         sphere { <0 6 0> 6.71 }
         quadric { Cylinder_Y scale <3 1 3> }
      }
   }

   texture { WWood }
   colour red 0.95 green 0.62
}

#declare BQueen = object {
   union { Queen }

   bounded_by {
      intersection {
         sphere { <0 6 0> 6.71 }
         quadric { Cylinder_Y scale <3 1 3> }
      }
   }

   texture { BWood }
   colour red 0.4 green 0.2
}

#declare WKing = object {
   union { King }

   bounded_by {
      intersection {
         sphere { <0 6.5 0> 7.16 }
         quadric { Cylinder_Y scale <3 1 3> }
      }
   }

   texture { WWood }
   colour red 0.95 green 0.62
}

#declare BKing = object {
   union { King }

   bounded_by {
      intersection {
         sphere { <0 6.5 0> 7.16 }
         quadric { Cylinder_Y scale <3 1 3> }
      }
   }

   texture { BWood }
   colour red 0.4 green 0.2
}

object { /* Sky */
   sphere { <0 -39000 0> 40000 inverse }

   texture {
      bozo
      turbulence 0.6
      colour_map {
         [0 0.5 colour red 0.4 green 0.5 blue 1
         colour red 0.4 green 0.5 blue 1.0]
         [0.5 0.7 colour red 0.4 green 0.5 blue 1
         colour red 1 green 1 blue 1.0]
         [0.7 1 colour red 1 green 1 blue 1
         colour red 0.7 green 0.7 blue 0.7]
      }
      scale <500 500 500>
      ambient 1
      diffuse 0
   }

   colour red 0.4 green 0.5 blue 1
}

object { /* Ground */
   plane { <0 1 0> -80 }

   texture {
      0.05
      colour green 1
      ambient 0.5
      diffuse 0.5
   }
   colour green 1
}

#declare Frame = intersection {
   plane { < 0  1  0> -0.0001 }
   plane { < 0 -1  0>  3 }
   plane { < 0  0 -1> 35 }
   plane { <-1  0  1>  0 }
   plane { < 1  0  1>  0 }
}

composite {
   composite {
      object {
         union {
            intersection { Frame }
            intersection { Frame rotate <0 180 0> }
         }

         texture {
            wood
            turbulence 0.3
            scale <0.8 1000 0.8>
            rotate <0 0 -88>
            translate <200 40 -20>
            specular 1
            roughness 0.02
         }

         colour red 0.5 green 0.25
      }

      object {
         union {
            intersection { Frame rotate <0 -90 0> }
            intersection { Frame rotate <0  90 0> }
         }

         texture {
            wood
            turbulence 0.3
            scale <0.8 1000 0.8>
            rotate <-91 0 0>
            translate <100 30 0>
            specular 1
            roughness 0.02
         }

         colour red 0.5 green 0.25
      }

      object { /* Board */
         intersection {
            plane { < 1  0  0> 32 }
            plane { <-1  0  0> 32 }
            plane { < 0  1  0>  0 }
            plane { < 0 -1  0>  1 }
            plane { < 0  0  1> 32 }
            plane { < 0  0 -1> 32 }
         }

         texture {
            tiles {
               texture {
                  marble
                  turbulence 1.0
                  colour_map {
                     [0.0 0.7 colour White
                     colour White]
                     [0.7 0.9 colour White
                     colour red 0.8 green 0.8 blue 0.8]
                     [0.9 1.0 colour red 0.8 green 0.8 blue 0.8
                     colour red 0.5 green 0.5 blue 0.5]
                  }
                  scale <0.6 1 0.6>
                  rotate <0 -30 0>
               }
               tile2
               texture {
                  granite
                  scale <0.3 1 0.3>
                  colour_map {
                     [0 1 colour Black
                     colour red 0.5 green 0.5 blue 0.5]
                  }
               }
            }                        /* end_checker_texture */
            scale <8 1 8>
            specular 1
            roughness 0.02
            reflection 0.25
         }

         colour red 0.63 green 0.52 blue 0.45
      }

      object { /* Table */
         union {
            intersection {
               plane {  <0  1 0> -3 }
               plane {  <0 -1 0>  8 }
               sphere { <0 -5.5 0> 55 }
            }

            intersection {
               plane { <0 1 0> -8 }
               quadric { Hyperboloid_Y
                  scale <10 20 10>
                  translate <0 -20 0>
               }
            }
         }

         texture { granite
            scale <6 6 6>
            specular 1
            roughness 0.02
            reflection 0.3
         }

         colour red 0.5 green 0.5 blue 0.5
      }

      bounded_by {
         plane { <0 1 0> 0 }
      }
   }

   composite {
      object { WPawn translate <-28 0 -20> }
      object { WPawn translate <-20 0 -20> }
      object { WPawn translate <-12 0 -20> }
      object { WPawn translate < -4 0 -20> }
      object { WPawn translate <  4 0 -20> }
      object { WPawn translate < 12 0 -20> }
      object { WPawn translate < 20 0 -20> }
      object { WPawn translate < 28 0 -20> }

      object { WRook   translate <-28 0 -28> }
      object { WKnight translate <-20 0 -28> }
      object { WBishop translate <-12 0 -28> }
      object { WQueen  translate < -4 0 -28> }
      object { WKing   translate <  4 0 -28> }
      object { WBishop translate < 12 0 -28> }
      object { WKnight translate < 20 0 -28> }
      object { WRook   translate < 28 0 -28> }

      bounded_by {
         quadric { Cylinder_X
            scale <1 9.56 9.56>
            translate <0 6.5 -24>
         }
      }
   }

   composite {
      object { BPawn translate <-28 0 20> }
      object { BPawn translate <-20 0 20> }
      object { BPawn translate <-12 0 20> }
      object { BPawn translate < -4 0 20> }
      object { BPawn translate <  4 0 20> }
      object { BPawn translate < 12 0 20> }
      object { BPawn translate < 20 0 20> }
      object { BPawn translate < 28 0 20> }

      object { BRook   translate <-28 0 28> }
      object { BKnight translate <-20 0 28> }
      object { BBishop translate <-12 0 28> }
      object { BQueen  translate < -4 0 28> }
      object { BKing   translate <  4 0 28> }
      object { BBishop translate < 12 0 28> }
      object { BKnight translate < 20 0 28> }
      object { BRook   translate < 28 0 28> }

      bounded_by {
         quadric { Cylinder_X
            scale <1 9.56 9.56>
            translate <0 6.5 24>
         }
      }
   }

   bounded_by {
      intersection {
         plane { <0 1 0> 13.5 }
         sphere { <0 -30 0> 63 }
      }
   }
}
