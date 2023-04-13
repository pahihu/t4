// Persistence Of Vision raytracer version 1.0 sample file.

// By Dan Farmer
// Parabolic arches on the water.  Is this the St. Louis McDonalds? 

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

camera {
   location <60.0  0.0  -135.0>
   direction <0.0 0.0  2.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0.0 0.0 0.0>
}

// Light 
object {  
   light_source {<200.0  200.0  -150.0>
      colour red 1 green .5
   }
}

// Now draw the sky (From sunset.inc)
object {
   sphere { <0.0  0.0  0.0> 300.0 }
   texture {
      gradient <0.0  1.0  0.0>
      colour_map { [0.0 0.8  colour red 0.5 green 0.1 blue 0.7
         colour red 0.1 green 0.1 blue 0.9]
         [0.8 1.0 colour red 0.1 green 0.1 blue 0.9
         colour red 0.1 green 0.1 blue 0.9]
      }
      scale <300.0  300.0  300.0>
      ambient 1
   }
   colour red 0.7  green 0.7 blue 1.0
}

// Put in a few clouds 

object {
   sphere { <0.0  0.0  0.0> 259.0 }

   texture {
      bozo
      turbulence 0.7
      colour_map {
         [0.0 0.6  colour red 1.0 green 1.0 blue 1.0 alpha 1.0
         colour red 1.0 green 1.0 blue 1.0 alpha 1.0]
         [0.6 0.8 colour red 1.0 green 1.0 blue 1.0 alpha 1.0
         colour red 1.0 green 1.0 blue 1.0]
         [0.8 1.001 colour red 1.0 green 1.0 blue 1.0
         colour red 0.8 green 0.8 blue 0.8]
      }
      scale <100.0  20.0  100.0>
      ambient 1
   }
   colour red 0.7  green 0.7  blue 1.0
}



// Define the ocean surface 
object {
   plane { <0.0  1.0  0.0> -10.0 }

   texture {
      colour Blue
      waves 0.05
      reflection 0.8
      frequency 5000.0
      scale <3000.0 3000.0 3000.0>
   }
   colour Blue
}

/* Put a floor underneath to catch any errant waves from the ripples */
object {
   plane { <0.0  1.0  0.0> -11.0 }
   texture {
      colour Blue
      ambient 1.0
      diffuse 0.0
   }
   colour Blue
}

// Create the arches using CSG difference between two "squashed" paraboloids
composite {
   object {
      difference {
         quadric { Paraboloid_Y
            scale <20.0 20.0 5.0>
            rotate <180.0 0.0 0.0>
            translate <0.0 0.0 0.0>
         }
         quadric { Paraboloid_Y
            scale <15.0 20.0 15.0>
            rotate <180.0 0.0 0.0>
            translate <0.0 -2.0 0.0>
         }
      }
      texture {
         Metal
         color White
      }
      color White
   }

   /* Move the whole thing to where I should've
    designed it in the first place!!  */

   translate <0.0 30.0 -25.0>
}

composite {
   object {
      difference {
         quadric { Paraboloid_Y
            scale <20.0 20.0 5.0>
            rotate <180.0 0.0 0.0>
            translate <0.0 0.0 0.0>
         }
         quadric { Paraboloid_Y
            scale <15.0 20.0 15.0>
            rotate <180.0 0.0 0.0>
            translate <0.0 -2.0 0.0>
         }
      }
      texture {
         Metal
         color White
      }
      color White
   }

   /* Move the whole thing to where I should've
    designed it in the first place!!  */
   translate <0.0 30.0 50.0>

}
