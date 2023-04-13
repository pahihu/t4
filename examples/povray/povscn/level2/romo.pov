// POV-Ray scene
// Fun reflections of a fertility symbol.
// A strange, but pleasant image.

#include "colors.inc"
#include "textures.inc"
#include "shapes.inc"

camera {
  location <0 5 -16>
  direction <0 0 1.9>
  right <1.333 0 0>
  look_at <0 4 8>
}
#declare Radius = <.4 .4 .4>
#declare Width = <0 0 2>
object {
  union {
    quadric {Cylinder_Y scale Radius translate Width }
    quadric {Cylinder_Y scale Radius translate Width rotate <0 30 0>}
    quadric {Cylinder_Y scale Radius translate Width rotate <0 60 0>}
    quadric {Cylinder_Y scale Radius translate Width rotate <0 90 0>}
    
    quadric {Cylinder_Y scale Radius translate Width rotate <0 -30 0>}
    quadric {Cylinder_Y scale Radius translate Width rotate <0 -60 0>}
    quadric {Cylinder_Y scale Radius translate Width rotate <0 -90 0>}
    }
  scale<4 4 4>
  texture { Copper_Texture reflection 0.5 }
  translate <0 4 8>
}    
object { // Fertility symbol?
  union {
    sphere { <0 8 8> 2 }
    sphere { <2 7 8> 1 }
    sphere { <-2 7 8> 1 }
    sphere { <0 4 8> 4 }
    sphere { <3 2 8> 2 }
    sphere { <-3 2 8> 2 }
    sphere { <4.5 1 8> 1 }
    sphere { <-4.5 1 8> 1 }
  }
  scale <.7 .7 .7>
  translate <0 0 -3>
  texture { color Red
            phong 1}
  }
  
object { // Floor
  plane { <0 1 0> 0 }
  texture { Brass_Valley scale <2 2 2> }
 }

//object {
// sphere { <0 0 0> 100 }
// texture { Blood_Sky }
//}

object { 
  light_source { <3 16 -8> color red 1 green 1 blue 1 }
}
