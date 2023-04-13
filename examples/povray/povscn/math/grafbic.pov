// Persistence Of Vision raytracer version 1.0 sample file.

// Sample Quartic { file
// by Alexander Enzmann

// Bicorn 

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "shapesq.inc"

// Get the declaration of the coordinate axes 
#include "axisbox.inc"

// Declare the orientation of the surface 
#declare Orient = <-20 -60 0>

// Bicorn 
composite {
   object {
      quartic { Bicorn
         texture { color Red phong 1.0 }
         sturm
      }
      scale <3 3 3>
      bounded_by {
         sphere { <0 0 0> 3 }
      }
      clipped_by {
         plane { < 1 0 0> 0.5 }
         plane { <-1 0 0> 0.5 }
      }
   }
   object {
      quartic { Bicorn
         texture { color Blue phong 1.0 }
         sturm
      }
      scale <2.99 2.99 2.99>
      translate <0 0.01 0>
      bounded_by {
         sphere { <0 0 0> 2.99 }
      }
      clipped_by {
         plane { < 1 0 0> 0.49 }
         plane { <-1 0 0> 0.49 }
      }
   }
   translate <0 -1.5 0 >
   rotate Orient
}

// Show coordinate axes 
composite { Axes
   rotate Orient
}

// The viewer is eight units back along the z-axis. 
camera {
   location  <0.0  0.0 -8.0>
   right     <1.3333  0.0  0.0>
   up        <0.0  1.0  0.0>
   direction <0.0  0.0  1.0>
}

// Put in some light sources so that highlighting can give visual clues
//  as to the shape of the surface.
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
