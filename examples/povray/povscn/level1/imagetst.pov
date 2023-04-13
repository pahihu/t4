// Persistence Of Vision raytracer version 1.0 sample file.
//
// NOTE: Requires "TEST.GIF"
//
// This is a good image to practice placing an mapping image onto objects.
// A specially designed image is mapped onto a unit sphere and a unit box.
// Experiment with scaling, translation, map types, etc.  (But be sure that
// you keep a backup so can find your way home if you get things too turned
// around.  It can happen.
//
// If you are not using an extended mode version of POV-Ray,  you may not be
// able to access enough memory to run this entire file.  In this case,
// the best you can do is to comment out all but one or two of the three
// imagemapped objects.

#include "colors.inc"           // Standard colors library
#include "shapes.inc"           // Commonly used object shapes
#include "textures.inc"         // LOTS of neat textures.  Lots of NEW textures.

#declare POVPlastic = texture {
   ambient 0.3   diffuse 0.7
   phong 0.5     phong_size 60                /*  Satiny sheen */
}


camera {
   location <0.0  1.75  -6>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 -0.5 0>
}

object {
   light_source { <100.0  200.0  -60.0>
      colour White
   }
}

object {
   light_source { <-50.0  60.0  -30.0>
      colour Gray
   }
}

object {
   light_source { <-15.0  -10.0  -30.0>
      colour Gray
   }
}


// Sphere on the right
object {
   sphere { <0  0  0>  1 }                    /* A "unit" sphere */
   texture { Glossy color White }             /* Optional sub-texture */
   texture {
      POVPlastic
      image_map { <1 -1 0>  gif "test.gif"   /* x-y oriented planar map */
         once                                   /* Don't tile the image */
         interpolate 4                          /* highest quality interpolation */
      }
      translate <-0.5 -0.5 0>                /* Center the image */
      scale <2 2 2>                          /* Scale to fit diameter */
   }
   translate <1.2 1 0>                        /* Move imagemap with object */
}

// Sphere on the left
object {
   sphere { <0  0  0>  1 }
   texture { Glossy color White }
   texture {
      POVPlastic
      image_map { 1  gif "test.gif"          /* Spherical map type */
         /*"once" is implicit here */
         interpolate 4
      }
      rotate <0 -90 0>                       /* put "seam" in back */
      scale <2 2 2>                          /* scale to fit diameter */
   }
   translate <-1.2 1 0>                       /* Move image with object */
}



// Box
object {
   box { UnitBox scale <2 1 2>}
   texture { Metal color DimGray }

   texture {
      image_map { <1 -1 0> gif "test.gif"    /* x-y oriented bitmap image */
         interpolate 2                          /* Faster interpolation than 4 */
         alpha 0 1.0
      }
      translate <-0.5 -0.5 0>                /* Center image */
      scale <2 2 1>                          /* Scale to fit */
      ambient 0.25
      diffuse 0.75
   }

   translate <0 -1 0>                         /* Move image with object */
}
