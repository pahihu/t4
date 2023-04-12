// Persistence Of Vision raytracer version 1.0 sample file.

// By Tom Price
// Modification to two-layer wood texture by Dan Farmer 01/92
// Image should be run at 640x480 or greater to take full advantage of
// the wood texture, preferably with a rather tight anti-aliasing threshold.
// Also commented out the random dithering values and bounded the coin
// object.

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

// a dark wood with a greenish hue to it
#declare New_Dark_Wood1 = texture {
   wood
   turbulence 0.02
   colour_map {
      [0.0 0.8  colour red  0.42857 green 0.23810 blue 0.04762
      colour red  0.42857 green 0.23810 blue 0.04762]
      [0.8 1.01 colour red 0.4 green 0.333 blue 0.066
      colour red 0.2 green 0.033 blue 0.033]
   }
}

// Overlaying woodgrain
#declare New_Dark_Wood2 = texture {
   wood
   turbulence 0.022
   colour_map {
      [0.0 0.5  colour Clear  colour Clear]
      [0.5 1.01 colour red 0.4 green 0.333 blue 0.066 alpha 0.5
      colour red 0.2 green 0.033 blue 0.033 alpha 0.25]
   }
}

camera {
   location <0.0 75.0 -100.0>
   direction <0.0 -0.5 1.0>
   up <0.0 1.0 0.0>
   right <1.333 0.0 0.0>
}

object {
   quadric { Plane_XZ translate <0.0 -10.0 0.0> }

   texture {

      New_Dark_Wood1
      scale <.25 .25 1>
      rotate <0 90 1>

      ambient 0.8
      diffuse 0.2
      reflection 0.4
      brilliance 3.0
   }

   texture {
      New_Dark_Wood2
      scale <.25 .25 1>
      rotate <0 90 1.5>
   }
   color Brown
}

object {
   quadric { Plane_XZ translate <0.0 200.0 0.0> }
   texture {
      color blue 0.5 red 0.2 green 0.2
      ambient 0.5
      diffuse 0.5
   }
}

object {
   light_source { <60.0 100.0 -110.0>
      color White
   }
}

object {
   light_source { <-60.0 100.0 -110.0>
      color LightGray
   }
}


#declare Coin = object {
   intersection {
      quadric { Cylinder_Y scale <20.0 1.0 20.0> }
      quadric { Plane_XZ translate <0.0 1.0 0.0> }
      quadric { Plane_XZ translate <0.0 -1.0 0.0> inverse }
   }
   bounded_by {
      sphere { <0 0 0> 21}
   }

   texture { 
      ambient 0.2  diffuse 0.6
      color red 1.0 green 0.89 blue 0.55
      reflection 0.6
      brilliance 4.0
      specular 0.5
      metallic
   }

   color red 1.0 green 0.89 blue 0.55
}

object { Coin
   rotate <-15.0 0.0 -2.0>
   translate <-27.0 -2.0 -3.0>
}

object { Coin
   rotate <-15.0 0.0 0.0>
   translate <-28.0 3.0 2.0>
}
object { Coin
   rotate <-15.0 0.0 0.0>
   translate <-30.0 10.0 0.0>
}
object { Coin
   rotate <-15.0 0.0 0.0>
   translate <-29.0 20.0 -2.0>
}
object { Coin
   rotate <-15.0 0.0 -10.0>
   translate <-31.0 30.0 3.0>
}
object { Coin
   rotate <-15.0 0.0 0.0>
   translate <-26.0 40.0 5.0>
}
object { Coin
   rotate <-25.0 0.0 15.0>
   translate <-23.0 50.0 8.0>
}

// A reflective sphere
object {
   quadric { Ellipsoid scale <35.0 35.0 35.0> }
   translate <40.0 25.0 40.0>
   texture {
      color White
      ambient 0.1
      diffuse 0.3
      reflection 0.95
      brilliance 5.0
   }
   color White
}
