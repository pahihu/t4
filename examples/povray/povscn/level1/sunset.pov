// Persistence Of Vision raytracer version 1.0 sample file.

// By David Buck

#include "colors.inc"

camera {
   location <0.0  10.0  200.0>
   direction <0.0  0.0  -1.0>
   up <0.0  1.0  0.0>
   right <1.333333 0.0 0.0>
   look_at <0.0  130.0  -300.0>
}

/* Define the ocean surface */
object {
   plane { <0.0  1.0  0.0> -10.0 }

   texture {
      colour red 1.0 green 0.3
      waves 0.05
      frequency 5000.0
      scale <3000.0 3000.0 3000.0>
      reflection 1.0
   }
   colour red 1.0 green 0.3
}

/* Put a floor underneath to catch any errant waves from the ripples */
object {
   plane { <0.0  1.0  0.0> -11.0 }
   texture {
      0.05
      colour red 1.0 green 0.3
      ambient 1.0
      diffuse 0.0
   }
   colour red 1.0 green 0.3
}

/* Now draw the sky */
object {
   sphere { <0.0  0.0  0.0> 400.0 }

   texture {
      gradient <0.0  1.0  0.0>
      colour_map { [0.0 0.8  colour red 1.0 green 0.3 blue 0.0
                           colour red 0.7 green 0.7 blue 1.0]
                 [0.8 1.0 colour red 0.7 green 0.7 blue 1.0
                          colour red 0.7 green 0.7 blue 1.0]
      }
      scale <400.0  400.0  400.0>
      ambient 0.7
      diffuse 0.0   /* we don't want clouds casting shadows on the sky */
   }
   colour red 0.7  green 0.7 blue 1.0
}

/* Put in a few clouds */
object {
   sphere { <0.0  0.0  0.0> 259.0 }

   texture {
      bozo
      turbulence 0.5
      colour_map {
          [0.0 0.6  colour red 1.0 green 1.0 blue 1.0 alpha 1.0
                    colour red 1.0 green 1.0 blue 1.0 alpha 1.0]
          [0.6 0.8 colour red 1.0 green 1.0 blue 1.0 alpha 1.0
                    colour red 1.0 green 1.0 blue 1.0]
          [0.8 1.001 colour red 1.0 green 1.0 blue 1.0
                     colour red 0.8 green 0.8 blue 0.8]
      }
      scale <100.0  20.0  100.0>
      rotate <0 20 0>
      ambient 0.7
      diffuse 0.0
   }
   colour red 0.7 green 0.7 blue 1.0
}

/* Now to cast some light on the subject */
object {
   union {
      sphere { <0 0 0 > 40
         texture {
            colour red 1.0 green 0.6 alpha 0.35
            ambient 1.0
            diffuse 0.0
        }
      }
      light_source { <0  0  0>  colour White }
   }
    translate <0 60 -300>
}
