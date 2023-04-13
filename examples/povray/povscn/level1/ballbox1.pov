// Persistence of Vision Raytracer
// Image by Dan Farmer
// Demonstrates glass textures, CGS with box primitives, one of Mike Miller's
// fabulous marble textures, modified with an "octaves" change, and doesn't
// make a half-bad image, either.  Interesting lighting effect, too.

#include "colors.inc"           // Standard colors library
#include "shapes.inc"           // Commonly used object shapes
#include "textures.inc"         // LOTS of neat textures.  Lots of NEW textures.
#include "stones.inc"           // SPECIAL!  Mike Miller's stone library.

camera {
   location <+0.75  3.5  -3.5>
   direction <0.0 0.0  0.5>       //  "wide-angle" view
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 0 -1>
}

// Light sources, two to the front, right, on from the left, rear.
object { light_source { <-30 11 +20>  color White  } }
object { light_source { <31 12 -20>  color White  } }
object { light_source { <32 11 -20>  color LightGray  } }

composite {

   // A green glass ball inside of a box-shaped frame
   object {  sphere {< 0 0 0> 1.75 }
      texture {
         Glass                          // use the predefined "glass"
         color green 0.90 alpha 0.85    // transmit most of it's light
         phong 1 phong_size 300         // Very tight highlights
         reflection 0.15                // Needs a little reflection added
      }
   }


   // A box-shaped frame surrounding a green glass ball
   object {
      intersection {
         box {UnitBox scale <1.5 1.5 1.5> }    // The outside dimensions

         // And some square "holes" in all sides.  I think I could use
         // difference (no inverse in the union) instead, but I must've
         // lost my head for a moment.  Note that each of these boxes
         // that are going to be subtracted has one vector scaled just
         // slightly larger than the outside box.  The other two vectors
         // determine the size of the hole.
         union {   // Clip some sqr holes in the box to make a 3D box frame
            box {UnitBox scale <1.51 1.25 1.25> }   // "clip" x
            box {UnitBox scale <1.25 1.51 1.25> }   // "clip" y
            box {UnitBox scale <1.25 1.25 1.51> }   // "clip" z
            inverse
         }
      }
      texture {   // Totally un-spectacular.  _You_ can do better than this!
         ambient 0.2
         diffuse 0.7
         reflection 0.15
         brilliance 8.0
         specular 1
         roughness 0.01
         color red 0.75 green 0.75 blue 0.85
      }
   }

   bounded_by {     // Same as radius of glass sphere, not the box!
      box {UnitBox scale <1.75 1.75 1.75> }
   }
   rotate <0 45 0>
}

object {  plane {< 0 1 0> -1.5 }
   texture {
      Stone1
      octaves 3
      rotate <0 0 90>
      reflection 0.15
   }
}
// end of file
