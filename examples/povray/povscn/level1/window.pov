// Persistence Of Vision raytracer version 1.0 sample file.

// By Aaron A. Collins

/* Window Highlighting Scene by Aaron A. Collins */
/* This file is hereby released to the public domain. */

#include "colors.inc"

/* Someone to take in the breathtaking view... */

camera {
   location <0.0  20.0  -100.0>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
}

/* Put down the beloved famous raytrace green/yellow checkered floor */

object {
   plane { <0.0 1.0 0.0> -10.0 }
   texture {
      checker colour Yellow colour Green
      scale < 20.0 20.0 20.0 >
      ambient 0.1
      diffuse 0.9
   }
   colour Yellow
}

/*
 Now a CBlue Plastic sphere floating in space over the ground - note that no 
 Phong or specular reflection is given.  Any would conflict with the window
 "highlights" by showing that they are not exactly in the mirror direction!
*/

object {
   sphere { <0.0 25.0 0.0> 40.0 }
   texture {
      colour Blue
      reflection 0.8
      ambient 0.3
      diffuse 0.7
   }
   colour Blue
}

/*
 a wall with a window frame to block the light source and cast the shadows
*/

object {
   union {
      triangle { <-1000.0 -1000.0 0.0> <1000.0 4.0 0.0> <1000.0 -1000.0 0.0> }
      triangle { <-1000.0 -1000.0 0.0> <1000.0 4.0 0.0> <-1000.0 4.0 0.0> }
      triangle { <4.0 4.0 0.0> <-1000.0 21.0 0.0> <-1000.0 4.0 0.0> }
      triangle { <4.0 4.0 0.0> <-1000.0 21.0 0.0> <4.0 21.0 0.0> }
      triangle { <-1000.0 21.0 0.0> <1000.0 1000.0 0.0> <-1000.0 1000.0 0.0> }
      triangle { <-1000.0 21.0 0.0> <1000.0 1000.0 0.0> <1000.0 21.0 0.0> }
      triangle { <1000.0 4.0 0.0> <17.0 21.0 0.0> <1000.0 21.0 0.0> }
      triangle { <1000.0 4.0 0.0> <17.0 21.0 0.0> <17.0 4.0 0.0> }
      triangle { <4.0 12.0 0.0> <17.0 13.0 0.0> <4.0 13.0 0.0> }
      triangle { <4.0 12.0 0.0> <17.0 13.0 0.0> <17.0 12.0 0.0> }
      triangle { <10.0 21.0 0.0> <11.0 4.0 0.0> <11.0 21.0 0.0> }
      triangle { <10.0 21.0 0.0> <11.0 4.0 0.0> <10.0 4.0 0.0> }
   }
   translate <39.0 89.0 -120.0>
   texture {
      colour Black
      ambient 1.0
      diffuse 0.0
   }
}

/*
  Now, the 4 actual "panes" to be reflected back onto the sphere for psuedo-
  "highlights".  They are not exactly co-incident with where the actual light
  source is, because they would block the light.  They are very near by where
  the openings are in the black wall above, close enough to give the proper
  illusion.  This is massive cheating, but then, this isn't reality, you see.
*/

object {
   union {
      triangle { <4.0 21.0 0.0> <10.0 13.0 0.0> <10.0 21.0 0.0> }
      triangle { <4.0 21.0 0.0> <10.0 13.0 0.0> <4.0 13.0 0.0> }

      triangle { <11.0 21.0 0.0> <17.0 13.0 0.0> <11.0 13.0 0.0> }
      triangle { <11.0 21.0 0.0> <17.0 13.0 0.0> <17.0 21.0 0.0> }

      triangle { <4.0 12.0 0.0> <10.0 4.0 0.0> <4.0 4.0 0.0> }
      triangle { <4.0 12.0 0.0> <10.0 4.0 0.0> <10.0 12.0 0.0> }

      triangle { <11.0 12.0 0.0> <17.0 4.0 0.0> <11.0 4.0 0.0> }
      triangle { <11.0 12.0 0.0> <17.0 4.0 0.0> <17.0 12.0 0.0> }
   }
   scale <15.0 15.0 15.0>
   translate <20.0 90.0 -100.0>
   texture {
      colour White
      ambient 1.0
      diffuse 0.0
   }
}


/* a Light above the sphere, behind the camera and window frame for shadows */

object {
   light_source { <50.0 111.0 -130.0>
      colour White
   }
}
