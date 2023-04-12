// Persistence Of Vision raytracer version 1.0 sample file.

// Sample quartic { file
// by Alexander Enzmann

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "shapesq.inc"

/*
   Approximation to the helix z = arctan(y/x).

   The helix can be approximated with an algebraic equation (kept to the
   range of a quartic) with the following steps:

      tan(z) = y/x   =>  sin(z)/cos(z) = y/x   =>

   (1) x sin(z) - y cos(z) = 0

   Using the taylor expansions for sin, cos about z = 0,

      sin(z) = z - z^3/3! + z^5/5! - ...
      cos(z) = 1 - z^2/2! + z^6/6! - ...

   Throwing out the high order terms, the expression (1) can be written as:

      x (z - z^3/6) - y (1 + z^2/2) = 0, or

  (2) -1/6 x z^3 + x z + 1/2 y z^2 - y = 0

  This helix (2) turns 90 degrees in the range 0 <= z <= sqrt(2)/2.  By using
  scale <2 2 2>, the helix defined below turns 90 degrees in the range
  0 <= z <= sqrt(2) = 1.4042.
*/

#declare Red_Helix =
object { Helix
   texture {
      /* gradient <0 1 0>
        color_map
           [0 1.001 color red 1 green 0 blue 0 color red 0 green 1 blue 0]
        end_color_map */
      color red 1 green 0 blue 0
      /* scale <1 1.4142 1> */
      phong 1.0
   }
   bounded_by {
      sphere { <0 0 0> 2.5 }
      quadric { Cylinder_Z scale <2.0 2.0 2.0> }
   }
}

#declare Green_Helix =
object { Helix
   texture {
      /*gradient <0 1 0>
       color_map
          [0 1.001 color red 0 green 1 blue 0 color red 1 green 0 blue 0]
       end_color_map */
      color red 0 green 1 blue 0
      /* scale <1 1.4142 1> */
      phong 1.0
   }
   bounded_by {
      sphere { <0 0 0> 2.5 }
      quadric { Cylinder_Z scale <2.0 2.0 2.0> }
   }
}

// Glue a bunch of pieces together to make one long helix. 

object { Green_Helix
   translate <0 0 -4.2426>
   rotate <0 0 160>
   rotate <-90 0 0>
   translate <0 -2 5>
}

object { Red_Helix
   translate <0 0 -2.8284>
   rotate <0 0 70>
   rotate <-90 0 0>
   translate <0 -2 5>
}

object { Green_Helix
   translate <0 0 -1.4142>
   rotate <0 0 160>
   rotate <-90 0 0>
   translate <0 -2 5>
}

object { Red_Helix
   rotate <0 0 70>
   rotate <-90 0 0>
   translate <0 -2 5>
}

object { Green_Helix
   translate <0 0 1.4142>
   rotate <0 0 160>
   rotate <-90 0 0>
   translate <0 -2 5>
}

object { Red_Helix
   translate <0 0 2.8284>
   rotate <0 0 70>
   rotate <-90 0 0>
   translate <0 -2 5>
}

object { Green_Helix
   translate <0 0 4.2426>
   rotate <0 0 160>
   rotate <-90 0 0>
   translate <0 -2 5>
}

object { Red_Helix
   translate <0 0 5.6569>
   rotate <0 0 70>
   rotate <-90 0 0>
   translate <0 -2 5>
}

object { Green_Helix
   translate <0 0 7.0711>
   rotate <0 0 160>
   rotate <-90 0 0>
   translate <0 -2 5>
}


camera {
   location <0.0  0.0 -10.0>
   direction <0.0 0.0 1.0>
   up        <0.0 1.0 0.0>
   right     <1.333 0.0 0.0>
}

// Toss in a couple of light sources. 
object {
   light_source { <200 100 -300>
      colour red 1.0 green 1.0 blue 1.0
   }
}

object {
   light_source { <-200 100 -300>
      colour red 1.0 green 1.0 blue 1.0
   }
}
