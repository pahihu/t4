// Persistence Of Vision raytracer version 1.0 sample file.
// By Rick Mallett of Carleton University,  Ottowa.
// First stage of the Tower of Pisa 
// Warning:  This picture can take a very long time to ray trace
// due to the large number of objects.  You have been warned :->  

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

camera {
   location <0.0 25.0 -150.0>
   direction <0.0 0.1 1.0>
   up <0.0 1.0 0.0>
   right <1.3333 0.0 0.0>
}

#declare Beam = quadric { Cylinder_Y
   scale <0.5 20.0 0.5>
   translate <2.0 0.0 0.0>
}

/* create a sample column for the base of the structure */

#declare BaseColumn = object {
   intersection {
      union {
         quadric { Beam }
         quadric { Beam rotate <0.0 -25.7  0.0> }
         quadric { Beam rotate <0.0 -51.4  0.0> }
         quadric { Beam rotate <0.0 -77.1  0.0> }
         quadric { Beam rotate <0.0 -102.8 0.0> }
         quadric { Beam rotate <0.0 -128.5 0.0> }
         quadric { Beam rotate <0.0 -154.2 0.0> }
         quadric { Beam rotate <0.0 -179.9 0.0> }
         quadric { Beam rotate <0.0 -205.6 0.0> }
         quadric { Beam rotate <0.0 -231.3 0.0> }
         quadric { Beam rotate <0.0 -257.0 0.0> }
         quadric { Beam rotate <0.0 -282.7 0.0> }
         quadric { Beam rotate <0.0 -308.4 0.0> }
         quadric { Beam rotate <0.0 -334.1 0.0> }
      }

      plane { <0.0 1.0 0.0> 40.0 }
      plane { <0.0 -1.0 0.0> 0.0 }
   }

   bounded_by {
      intersection {
         plane { <0.0 1.0 0.0> 40.0 }
         plane { <0.0 -1.0 0.0> 0.0 }
         quadric {
            Cylinder_Y
            scale <2.51 1.0 2.51>
         }
      }
   }

   texture {
      Red_Marble
      scale <10.0 10.0 10.0>
      ambient 0.2
      diffuse 0.7
      reflection 0.1
   }

   colour red 0.8 green 0.0 blue 0.0

}

/* and a rectangular pad to serve as a footing for the column */

#declare BasePad = object {
   box {
      UnitBox
      scale <4 1 4>
   }
   texture {
      Red_Marble
      scale <10.0 10.0 10.0>
      ambient 0.2
      diffuse 0.7
      reflection 0.1
   }

   colour red 0.6 green 0.6 blue 0.4

}

/* and define a basic arch to span the columns */

#declare BaseArch = object {
   intersection {
      quadric { Cylinder_X scale <1.0 12.5 12.5> }
      quadric { Cylinder_X scale <1.0 8.5 8.5> inverse }
      plane { <1.0 0.0 0.0> 2.0 }
      plane { <-1.0 0.0 0.0> 2.0 }
      plane { <0.0 -1.0 0.0> 0.0 }
   }

   bounded_by {
      quadric {
         Ellipsoid
         scale <5.0 13.0 13.0>
      }
   }

   texture {
      Red_Marble
      scale <10.0 10.0 10.0>
      ambient 0.2
      diffuse 0.7
      reflection 0.1
   }

   colour red 0.8 green 0.8 blue 0.8

}

/* and finally define the first floor floor */

#declare BaseFloor = object {
   intersection {
      quadric { Cylinder_Y scale <50.0 50.0 50.0> }
      quadric { Cylinder_Y scale <40.0 40.0 40.0> inverse }
      plane { <0.0 1.0 0.0> 2.0 }
      plane { <0.0 -1.0 0.0> 2.0 }
   }

   texture {
      Red_Marble
      scale <10.0 10.0 10.0>
      ambient 0.2
      diffuse 0.7
      reflection 0.1
   }

   colour red 0.8 green 0.8 blue 0.6

}

/* place a ring of 14 columns with footings around the base */

#declare FullColumn = composite {
   object { BaseColumn translate <45.0 0.0 0.0>   }
   object { BasePad    translate <45.0 -1.0 0.0>  }
   object { BasePad    translate <45.0 41.0 0.0>  }
   object { BaseArch   translate <45.0 42.0 2.0>
      rotate <0.0 -12.85 0.0>     }
}

#declare Level1 = composite {
   composite { FullColumn }
   composite { FullColumn rotate <0.0 -25.7  0.0> }
   composite { FullColumn rotate <0.0 -51.4  0.0> }
   composite { FullColumn rotate <0.0 -77.1  0.0> }
   composite { FullColumn rotate <0.0 -102.8 0.0> }
   composite { FullColumn rotate <0.0 -128.5 0.0> }
   composite { FullColumn rotate <0.0 -154.2 0.0> }
   composite { FullColumn rotate <0.0 -179.9 0.0> }
   composite { FullColumn rotate <0.0 -205.6 0.0> }
   composite { FullColumn rotate <0.0 -231.3 0.0> }
   composite { FullColumn rotate <0.0 -257.0 0.0> }
   composite { FullColumn rotate <0.0 -282.7 0.0> }
   composite { FullColumn rotate <0.0 -308.4 0.0> }
   composite { FullColumn rotate <0.0 -334.1 0.0> }
   composite { FullColumn rotate <0.0 -334.1 0.0> }
   object { BaseFloor translate <0.0 56.5 0.0>   }

   bounded_by {
      intersection { 
         quadric { Cylinder_Y scale <55.0 1.0 55.0> }
         plane { <0.0  -1.0  0.0> 0.0 }
         plane { <0.0  1.0   0.0> 60.0 }
      }
   }
}

composite { Level1 }

/* Add the sky to the picture */
object {
   sphere { <0.0 0.0 0.0> 300.0 }

   texture {
      bozo
      turbulence 0.5
      colour_map {
         [0.0 0.6  colour red 0.5 green 0.5 blue 1.0
         colour red 0.5 green 0.5 blue 1.0]
         [0.6 0.8 colour red 0.5 green 0.5 blue 1.0
         colour red 1.0 green 1.0 blue 1.0]
         [0.8 1.001 colour red 1.0 green 1.0 blue 1.0
         colour red 0.8 green 0.8 blue 0.8]
      }
      scale <100.0  20.0  100.0>
      ambient 0.8
      diffuse 0.0
   }

   colour red 0.5 green 0.5 blue 1.0

}

/* Define the desert floor */
object {
   plane { <0.0 1.0 0.0> -2.0 }

   texture {
      0.05  /* This value dithers the colours */
      colour red 1.0 green 0.66 blue 0.2
      ripples 0.5
      frequency 2000.0
      scale <50000.0 50000.0 50000.0>
      ambient 0.3
      diffuse 0.7
   }

   colour red 1.0 green 0.66 blue 0.2

}

/* Add a light source */
object {
   light_source { <60.0  50.0  -110.0>
      colour White
   }
}
