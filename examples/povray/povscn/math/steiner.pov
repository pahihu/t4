// Persistence Of Vision raytracer version 1.0 sample file.

// Sample quartic { file 
// by Alexander Enzmann

#include "Shapes.inc"
#include "Colors.inc"
#include "Textures.inc"
#include "shapesq.inc"

// Get the declaration of the coordinate axes 
#include "axisbox.inc"

// Declare the orientation of the surface 
#declare Steiner_Orientation = <50 -20 0>

object {
   quartic { Steiner_Surface
      texture { color Red phong 1.0 }
   }
   bounded_by {
      sphere { <0 0 0> 1 }
   }
   scale <6 6 6>
   rotate Steiner_Orientation
}

// Show coordinate axes 
composite { Axes
   rotate Steiner_Orientation
}

// The viewer is eight units back along the z-axis. 
camera {
   location  <0.0  0.0 -8.0>
   right     <1.3333  0.0  0.0>
   up        <0.0  1.0  0.0>
   direction <0.0  0.0  1.0>
}

// Put in some light sources so that highlighting can give visual clues
//  as to the shape { of the surface. 
object {
   light_source { <200 100 -300>
      colour White
   }
}

object {
   light_source { <-200 100 -300>
      colour White
   }
}
