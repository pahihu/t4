// Persistence of Vision Raytracer Version 1.0
//  "Pawns", a study in wood by Douglas Otwell
//  Three pawns on a chessboard
//   
//  Thanks to Dan Farmer and Mike Miller for showing me how.
//  The color maps were designed using Lutz Kretzschmar's cmapper v1.1.

#include "colors.inc"
#include "shapes.inc"
//#include "shapes.old"
#include "textures.inc"

//
//   Yellow pine, close grained
//
#declare Yellow_Pine = texture {
   wood
   turbulence 0.02
   colour_map {
      [0.000 0.222   colour red  0.808  green  0.671  blue  0.251  alpha  0.000
      colour red  0.808  green  0.671  blue  0.251  alpha  0.000]
      [0.222 0.342   colour red  0.808  green  0.671  blue  0.251  alpha  0.000
      colour red  0.600  green  0.349  blue  0.043  alpha  0.000]
      [0.342 0.393   colour red  0.600  green  0.349  blue  0.043  alpha  0.000
      colour red  0.808  green  0.671  blue  0.251  alpha  0.000]
      [0.393 0.709   colour red  0.808  green  0.671  blue  0.251  alpha  0.000
      colour red  0.808  green  0.671  blue  0.251  alpha  0.000]
      [0.709 0.821   colour red  0.808  green  0.671  blue  0.251  alpha  0.000
      colour red  0.533  green  0.298  blue  0.027  alpha  0.000]
      [0.821 1.001   colour red  0.533  green  0.298  blue  0.027  alpha  0.000
      colour red  0.808  green  0.671  blue  0.251  alpha  0.000]
   }
   scale <0.1 0.1 0.1>
   translate <10 0 0>

}

texture {
   wood
   turbulence 0.01
   colour_map {
      [0.000 0.120   colour red  1.000  green  1.000  blue  1.000  alpha  1.000
      colour red  0.702  green  0.412  blue  0.118  alpha  0.608]
      [0.120 0.231   colour red  0.702  green  0.412  blue  0.118  alpha  0.608
      colour red  0.702  green  0.467  blue  0.118  alpha  0.608]
      [0.231 0.496   colour red  0.702  green  0.467  blue  0.118  alpha  0.608
      colour red  1.000  green  1.000  blue  1.000  alpha  1.000]
      [0.496 0.701   colour red  1.000  green  1.000  blue  1.000  alpha  1.000
      colour red  1.000  green  1.000  blue  1.000  alpha  1.000]
      [0.701 0.829   colour red  1.000  green  1.000  blue  1.000  alpha  1.000
      colour red  0.702  green  0.467  blue  0.118  alpha  0.608]
      [0.829 1.001   colour red  0.702  green  0.467  blue  0.118  alpha  0.608
      colour red  1.000  green  1.000  blue  1.000  alpha  1.000]
   }
   scale <0.5 0.5 0.5>
   translate <10 0 0>
}

//
//   Rosewood
//
#declare Rosewood = texture {
   bozo
   turbulence 0.05
   colour_map {
      [0.000 0.256   colour red  0.204  green  0.110  blue  0.078  alpha  0.000
      colour red  0.231  green  0.125  blue  0.090  alpha  0.000]
      [0.256 0.393   colour red  0.231  green  0.125  blue  0.090  alpha  0.000
      colour red  0.247  green  0.133  blue  0.090  alpha  0.000]
      [0.393 0.581   colour red  0.247  green  0.133  blue  0.090  alpha  0.000
      colour red  0.204  green  0.110  blue  0.075  alpha  0.000]
      [0.581 0.726   colour red  0.204  green  0.110  blue  0.075  alpha  0.000
      colour red  0.259  green  0.122  blue  0.102  alpha  0.000]
      [0.726 0.983   colour red  0.259  green  0.122  blue  0.102  alpha  0.000
      colour red  0.231  green  0.125  blue  0.086  alpha  0.000]
      [0.983 1.001   colour red  0.231  green  0.125  blue  0.086  alpha  0.000
      colour red  0.204  green  0.110  blue  0.078  alpha  0.000]
   }
   scale <0.2 0.2 4>
   rotate <0 90 0>
}

texture {
   wood
   turbulence 0.1
   colour_map {
      [0.000 0.139   colour red  0.545  green  0.349  blue  0.247  alpha  1.000
      colour red  0.000  green  0.000  blue  0.000  alpha  0.004]
      [0.139 0.148   colour red  0.000  green  0.000  blue  0.000  alpha  0.004
      colour red  0.000  green  0.000  blue  0.000  alpha  0.004]
      [0.148 0.287   colour red  0.000  green  0.000  blue  0.000  alpha  0.004
      colour red  0.545  green  0.349  blue  0.247  alpha  1.000]
      [0.287 0.443   colour red  0.545  green  0.349  blue  0.247  alpha  1.000
      colour red  0.545  green  0.349  blue  0.247  alpha  1.000]
      [0.443 0.626   colour red  0.545  green  0.349  blue  0.247  alpha  1.000
      colour red  0.000  green  0.000  blue  0.000  alpha  0.004]
      [0.626 0.635   colour red  0.000  green  0.000  blue  0.000  alpha  0.004
      colour red  0.000  green  0.000  blue  0.000  alpha  0.004]
      [0.635 0.843   colour red  0.000  green  0.000  blue  0.000  alpha  0.004
      colour red  0.545  green  0.349  blue  0.247  alpha  1.000]
      [0.843 1.001   colour red  0.545  green  0.349  blue  0.247  alpha  1.000
      colour red  0.545  green  0.349  blue  0.247  alpha  1.000]
   }
   scale <0.2 0.2 3.6>
   rotate <0 90 0>
}

//
//   Sandalwood
//
#declare Sandalwood = texture {
   bozo
   turbulence 0.2
   colour_map {
      [0.000 0.171   colour red  0.725  green  0.659  blue  0.455  alpha  0.000
      colour red  0.682  green  0.549  blue  0.420  alpha  0.000]
      [0.171 0.274   colour red  0.682  green  0.549  blue  0.420  alpha  0.000
      colour red  0.557  green  0.451  blue  0.322  alpha  0.000]
      [0.274 0.393   colour red  0.557  green  0.451  blue  0.322  alpha  0.000
      colour red  0.725  green  0.659  blue  0.455  alpha  0.000]
      [0.393 0.564   colour red  0.725  green  0.659  blue  0.455  alpha  0.000
      colour red  0.682  green  0.549  blue  0.420  alpha  0.000]
      [0.564 0.701   colour red  0.682  green  0.549  blue  0.420  alpha  0.000
      colour red  0.482  green  0.392  blue  0.278  alpha  0.000]
      [0.701 1.001   colour red  0.482  green  0.392  blue  0.278  alpha  0.000
      colour red  0.725  green  0.659  blue  0.455  alpha  0.000]
   }
   scale <0.2 0.2 1>
   scale <2 2 2>
   rotate <0 90 0>
}

texture {
   bozo
   turbulence 0.8
   colour_map {
      [0.000 0.087   colour red  0.682  green  0.604  blue  0.380  alpha  1.000
      colour red  0.761  green  0.694  blue  0.600  alpha  0.020]
      [0.087 0.226   colour red  0.761  green  0.694  blue  0.600  alpha  0.020
      colour red  0.635  green  0.553  blue  0.325  alpha  1.000]
      [0.226 0.348   colour red  0.635  green  0.553  blue  0.325  alpha  1.000
      colour red  0.761  green  0.694  blue  0.600  alpha  0.020]
      [0.348 0.496   colour red  0.761  green  0.694  blue  0.600  alpha  0.020
      colour red  0.682  green  0.604  blue  0.380  alpha  1.000]
      [0.496 0.565   colour red  0.682  green  0.604  blue  0.380  alpha  1.000
      colour red  0.761  green  0.694  blue  0.600  alpha  0.020]
      [0.565 0.661   colour red  0.761  green  0.694  blue  0.600  alpha  0.020
      colour red  0.682  green  0.604  blue  0.380  alpha  1.000]
      [0.661 0.835   colour red  0.682  green  0.604  blue  0.380  alpha  1.000
      colour red  0.761  green  0.694  blue  0.600  alpha  0.020]
      [0.835 1.001   colour red  0.761  green  0.694  blue  0.600  alpha  0.020
      colour red  0.682  green  0.604  blue  0.380  alpha  1.000]
   }
   scale <0.2 0.2 2.0>
   scale <2 2 2>
   rotate <0 90 0>
}

//
//   Lights, Camera ...
//
camera {
   location  <-8.0  4.0  -14.0>
   direction <0.0 0.0  2.0>
   up        <0.0  1.0  0.0>
   right     <1.33333 0.0 0.0>
   look_at   <-2.0 0.0 -4.0>
}

object { light_source { <100.0  400.0  -600.0> color White } }

//   a back-light to create a highlight on the board
object { light_source { <12.0  4.0  12.0> color White } }

//  Because tiles { doesn't work with layered textures, we'll build
//  our chessboard out of one big pine block and 32 little rosewood ones
#declare Fours = composite {
   object {
      box { UnitBox }
      texture {
         Rosewood
         phong 0.3
         ambient 0.5
         diffuse 0.7
         reflection 0.3
      }
      translate <-1 -1 1>
   }
   object {
      box { UnitBox }
      texture {
         Rosewood
         phong 0.3 
         ambient 0.5
         diffuse 0.7
         reflection 0.3
      }
      translate <1 -1 -1>
   }

   bounded_by { box { UnitBox scale <2 1 2> }
   }
}

#declare Eights = composite {
   composite {   
      Fours
      translate <-2 0 2>
   }
   composite {
      Fours
      translate <2 0 2>
   }
   composite {
      Fours
      translate <-2 0 -2>
   }
   composite {
      Fours
      translate <2 0 -2>
   }

   bounded_by { box { UnitBox scale <4 1 4> }
   }
}

//   Add another wood texture { around the edges
#declare rail = object {
   intersection {
      box { UnitBox
         scale <10 0.25 1>
      }               
      plane { <-1 0 0> 0 
         rotate <0 45 0>
         translate <-9 0 0>
      }
      plane { <1 0 0> 0 
         rotate <0 -45 0>
         translate <9 0 0>
      }
   }
   translate <0 -0.25 -9>

   texture {
      Sandalwood
      phong 0.4
   }
}

//
//   Chessboard
//
#declare Chessboard = composite {
   composite {   
      Eights
      translate <-4 0 4>
   }
   composite {
      Eights
      translate <4 0 4>
   }
   composite {
      Eights
      translate <-4 0 -4>
   }
   composite {
      Eights
      translate <4 0 -4>
   }

   object {
      box { UnitBox
         scale <8 0.25 8>
         translate <0 -0.25 0>
      }               
      texture {
         Yellow_Pine
         phong 0.5
         reflection 0.3
         scale <2 2 2>
      }
   }

   object { rail rotate <0 0 0 > }
   object { rail rotate <0 90 0> }
   object { rail rotate <0 180 0> }
   object { rail rotate <0 270 0> }

   bounded_by {
      box { UnitBox
         scale <10 1 10>
      }             
   }
}

//
//   Pawn
//
#declare pawn = object {
   union {
      difference {
         intersection { Disk_Y
            scale <8 12.7468 8>
         }
         quartic {
            < 1.0   0.0   0.0    0.0     2.0   0.0   0.0   2.0   0.0 -738.0
            0.0   0.0   0.0    0.0     0.0   0.0   0.0   0.0   0.0    0.0
            1.0   0.0   0.0    2.0     0.0  162.0   0.0   0.0   0.0    0.0
            1.0   0.0 -738.0   0.0   6561.0 >
            sturm
         }
      }

      quartic {
         < 1.0   0.0   0.0    0.0     2.0   0.0   0.0   2.0   0.0 -132.5
         0.0   0.0   0.0    0.0     0.0   0.0   0.0   0.0   0.0    0.0
         1.0   0.0   0.0    2.0     0.0  123.5   0.0   0.0   0.0    0.0
         1.0   0.0 -132.5   0.0   3813.0625 >
         sturm
         translate <0 -11.2468 0>
      }
      quartic {
         < 1.0   0.0   0.0    0.0     2.0   0.0   0.0   2.0   0.0 -132.5
         0.0   0.0   0.0    0.0     0.0   0.0   0.0   0.0   0.0    0.0
         1.0   0.0   0.0    2.0     0.0  123.5   0.0   0.0   0.0    0.0
         1.0   0.0 -132.5   0.0   3813.0625 >
         sturm
         translate <0 11.2468 0>
      }

      //   Base
      intersection {
         intersection { Disk_Y
            scale <12 3 12>
            translate <0 -15.7468 0>
         }
         quadric {
            QCone_Y
            translate <0 -2 0>
         }
      }

      //   Ball on top
      sphere { <0 17.7468 0> 7 }
   }

   bounded_by {
      intersection { Disk_Y
         scale <14 26 14>
      }
   }

   translate <0 18.7468 0>
   scale <0.06 0.06 0.06>
}


//   Now let's put the pieces together

composite { Chessboard }

//   Pawn 1
object { 
   pawn 
   texture {
      Yellow_Pine
      phong 0.8
   }

   rotate <0 60 0>
   translate <-5 0 -7>
}

//   Pawn 2
object { 
   pawn 
   texture {
      Yellow_Pine
      phong 0.8
   }

   rotate <0 30 0>
   translate <1 0 -1>
}

//   Pawn 3
object { 
   pawn 
   texture {
      Rosewood
      phong 1.0
      ambient 0.5
      diffuse 0.7
   }

   rotate <0 30 0>
   translate <0.72 -0.24 0>
   rotate <0 0 96.2052>
   translate <1 0 -5>
}

//   a background glow to add interest
object { 
   sphere { <0 0 0 > 1000 inverse }
   texture {
      gradient <0 1 0>
      color_map {
         [0.0 0.25   color Goldenrod color Goldenrod]
         [0.25 0.46   color Goldenrod color Black]
         [0.46 1.001 color Black color Black]
      }
      scale <2000 2000 2000>
      translate <0 -1000 0>
   }
}
