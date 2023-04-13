// Persistence Of Vision raytracer version 1.0 sample file.

/* Persistence of Vision Raytracer
   Scene file by Dan Farmer       Dec 1, 1991
   Demo of POV-Ray wood texture possibilities

   Note:  This image contains very fine detail in the texture.  To truly
          see the image as it's intended to look it should be rendered
          at *least* 360x480, with a minimum anti-aliasing threshold of
          +a0.05 (that's not 0.5).  It can look pretty good if you do,
          and pretty awful if you don't.  Really.
          Time to render at 640x480 +a0.025 on a 486/33: 9 hrs 30 min.
          (a lot less without the anti-aliasing!)
-----------------------------------------------------------------------------
*/
#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

// Symmetrical Disk_Z, scales equally both directions, unit length.
#declare Disk_Z = intersection {       /* Capped cylinder, Length in z axis */
   quadric { Cylinder_Z }
   plane { <0.0 0.0 1.0> -1.0 inverse }
   plane { <0.0 0.0 1.0>  1.0 }
}

#declare Tan = color red 0.86 green 0.58 blue 0.44
#declare Brown = colour red 0.647059 green 0.164706 blue 0.164706
#declare DarkBrown = color red 0.45 green 0.26 blue 0.24


camera {
   location <-20 15 -20>
   direction <0 0 1.7>
   up  <0 1 0>
   right <1.33333 0 0>
   look_at <-2 5 0>
}


object {   /* light */
   light_source { <-15 10 -13>
      colour red 0.8 green 0.8 blue 0.8
   }
}

object {   /* light */
   light_source { <-10 30 20>
      colour red 0.6 green 0.6 blue 0.6
   }
}

object {   /* background */
   sphere { <0 0 0> 1 }
   color Gray
   texture {
      ambient 1.0  diffuse 0
      gradient <0 1 0>
      color_map {
         [0.0 1.01 color red 0.50 green 0.50 blue 0.50
         color red 0.0 green 0.0 blue 0.0]
      }
      translate <0 1 0>
   }
   scale <255 255 255>
}


#declare WoodBlock = object {
   difference {

      // a block with a beveled front edge
      intersection {

         intersection { Cube scale <10 5 2> }

         // Take a 45 degree cut from left top edge
         plane { <-1 0 0> 0
            rotate <0 0 -45>
            translate <-10 0 0>
         }

      }

      // bore a hole through block
      quadric { Cylinder_Z scale <4 4 1> translate <5 0 0> }

   }

   color White

   // This texture { is two layers:  the lower "base-coat" is finely
   // "speckled" to simulate the fine pores in the wood.  The second,
   // "top-coat" is the darker woodgrain, fading into clear to let
   // the "base-coat" show through.
   texture {    // Bottom, shiny layer: fine porous woodgrain
      bozo
      color_map {
         [0.00 0.05 color Brown color Brown]
         [0.05 0.25 color Brown color Tan]
         [0.25 0.75 color Tan color Tan]
         [0.75 0.95 color Tan color Brown]
         [0.95 1.01 color Brown color Brown]
      }

      // Note: i've designed this layer to be scaled in the z axis and
      // then rotated 90 degrees on the y axiz to maintain consistancy with
      // the way wood texture { operates.
      scale <0.01 0.01 1>
      rotate <0 90 0>

      ambient 0.15 diffuse 0.7
      specular 1 roughness 0.005   // Make this layer shiny
      phong 1 phong_size 100
   }


   // Wood texture { is mapped in concentric rings aligned on the z axis.
   texture {      // Top texture {, dark grain
      wood
      turbulence 0.05   // Note the small amount of turbulence used.

      color_map {
         [0.00 0.05 color Clear         color Clear    ]
         [0.05 0.30 color Clear         color DarkBrown]
         [0.30 0.70 color DarkBrown     color DarkBrown]
         [0.70 0.95 color DarkBrown     color Clear    ]
         [0.95 1.01 color Clear         color Clear    ]
      }

      scale <0.1 0.1 1>         // Make 10 "rings" per unit
      translate <0 -0.5 0>      // (This is just a cosmetic adjustment)
      scale <3 3 3>             // Enlarge the whole thing.
      rotate <0 90 0>           // align the "log" on the x axis, not z
      rotate <0 -1 0>           // slightly askew of center
      ambient 0.15 diffuse 0.7  // Notice: no shine on this layer
   }

   bounded_by {
      intersection { Cube scale <10.1 5.1 2.5> }
   }

   translate <0 5 0>             // Put the base of the block at y=0
}


// a "POV Ray" medallion stamped into the side.  This is, of course
// optional.  To create povray2.gif" i used improces, a shareware paint
// and image-processing program.  Text was entered and converted to
// black & white.   The text was smoothed repeatedly using a combination
// of the averaging and custom filters to "spread" the text and soften
// the edges so the bump_map would have a nice, smooth gradient to build
// the edges with.  Simply comment out the "object Logo" line in the
// composite block below if you don't want to create the .gif.  Won't
// detract from the image at all.
//#declare Logo = object {
//   intersection { Disk_Z
//       scale <1.75 1.75 1>
//       translate <0 0 -1>
//   }
//   texture {
//       Copper_Texture
//       bump_map { <1 -1 0> gif "povray2.gif"  once  // 320x200 image
//           bumpsize -10
//           interpolate 4
//       }
//       translate <-0.5 -0.65 0>
//       scale <1.6 1.5 1>
//       scale <2 2 1>
//   }
//}

composite {
   object { WoodBlock }
   //    object { Logo translate <-7 2.5 0> }
}
