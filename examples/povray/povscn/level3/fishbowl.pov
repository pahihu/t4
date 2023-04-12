// Persistence Of Vision Raytracer version 1.0 sample file.

// By Anon ???
// Revision notes:
//   01/19/92 dmf
//      -   Modified the wood texture on the picture frame.
//          Note: Wood textures always look their best at 640x480 or above
//          If you find that wood looks like random blocks of color instead
//          of lines of grain, you're probably experiencing texture aliasing
//          and need to increase the resolution and/or anti-aliasing threshold.
//
//          The fishbowl itself doesn't seem to work the way I would expect,
//          but I don't want to make such extensive changes.

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

// a light tan wood with greenish rings
#declare New_Pine_Wood = texture {
   wood
   turbulence 0.025
   colour_map {
      [0.0 0.4  colour red 1.0 green 0.65875 blue 0.25
      colour red 1.0 green 0.65875 blue 0.25]
      [0.4 1.01 colour red 0.7 green 0.6 blue 0.266
      colour red 0.4 green 0.3 blue 0.033]
   }
}/* end_texture */

#declare Y_Wood = texture { New_Pine_Wood
   scale <0.15 0.15 1>
   rotate <86 0 0>
   ambient 0.15
   diffuse 0.8
}
#declare X_Wood = texture { New_Pine_Wood
   scale <0.2 0.2 1>
   rotate <0 86 0>
   ambient 0.15
   diffuse 0.8
}


camera {
   location <0.0 10.0 -90.0>
   direction <0.0 0.0  1.0>
   up  <0.0 1.0 0.0>
   right <1.0 0.0 0.0>
}

/*Top part of bowl*/
object {
   intersection {
      sphere { <0.0 0.0 0.0> 20.0 }
      sphere { <0.0 0.0 0.0> 19.99 inverse }
      plane { <0.0 1.0 0.0> 15.0 rotate <10.0 0.0 0.0> }
      plane { <0.0 1.0 0.0> 10.0 rotate <10.0 0.0 0.0> inverse }
   }
   translate <-8.0 0.0 0.0>
   texture {
      color LightGray alpha 0.7
      ambient 0.3
      diffuse 0.7
      refraction 1.0
      ior 1.5
   }
}

/*Bottom part of bowl*/
object {
   intersection {
      sphere { <0.0 0.0 0.0> 20.0 }
      sphere { <0.0 0.0 0.0> 19.99 inverse }
      plane { <0.0 1.0 0.0> 10.0 rotate <10.0 0.0 0.0> }
   }
   translate <-8.0 0.0 0.0>
   texture {
      // Fishbowl.dat
      color LightBlue alpha 0.9
      //       ambient 0.1
      //       diffuse 0.7
      //       refraction 0.9
      //       ior 1.33

      // Fishx.dat
      ambient 0.3
      diffuse 0.7
      refraction 0.5

   }
}

// Table Top or floor
object {
   plane { <0.0 1.0 0.0> -18.0 }
   color Blue
   texture {
      checker colour DarkSlateGrey colour IndianRed
      scale <10.0 10.0 10.0>
      ambient 0.3
      diffuse 0.7
   }
}

// Wall
object {
   plane { <0.0 0.0 1.0>   30.0  }/* end_plane ( facing wall ) )*/
   color Cyan
   texture {
      colour red 0.329804 blue 0.204314 green 0.204314
   }
}

object {
   light_source { <-100.0  100.0  -130.0>
      colour White
   }
}


// Picture
composite {
   object {
      intersection {
         plane { <1.0 0.0 0.0> 1.0 }
         plane { <1.0 0.0 0.0> -1.0 inverse }
         plane { <0.0 1.0 0.0> 1.0 }
         plane { <0.0 1.0 0.0> -1.0 inverse }
         plane { <0.0 0.0 1.0> 1.0 }
         plane { <0.0 0.0 1.0> -1.0 inverse }
      }
      translate <1.0 1.0 1.0>
      scale <20.0 15.0 1.0>
      color Magenta
      texture {
         color White
         ambient 0.05
         diffuse 0.9
         reflection 0.95
      }
   }


   // The picture frame 
   object {
      union {

         intersection {
            union {
               quadric { Cylinder_Y scale <1.0 1.0 1.0> translate <41.0 0.0 0.0> texture { Y_Wood }}
               quadric { Cylinder_Y scale <1.0 1.0 1.0> translate <-1.0 0.0 0.0> texture { Y_Wood }}
            }
            plane { <0.0 1.0 0.0> 31.0 }
            plane { <0.0 1.0 0.0> -1.0 inverse }
         }

         intersection {
            union {
               quadric { Cylinder_X scale <1.0 1.0 1.0> translate <0.0 31.0 0.0> texture { X_Wood }}
               quadric { Cylinder_X scale <1.0 1.0 1.0> translate <0.0 -1.0 0.0> texture { X_Wood }}
            }
            plane { <1.0 0.0 0.0> 41.0 }
            plane { <1.0 0.0 0.0> -1.0 inverse }
         }

         sphere { <-1.0 -1.0 0.0> 1.0 texture { Y_Wood }}
         sphere { <-1.0 31.0 0.0> 1.0 texture { X_Wood }}
         sphere { <41.0 -1.0 0.0> 1.0 texture { Y_Wood }}
         sphere { <41.0 31.0 0.0> 1.0 texture { X_Wood }}

      }
      color Gray
   }

   scale <1.5 1.5 1.5>
   translate <20.0 20.0 29.0>
}/* end_composite */
