// Persistence Of Vision Raytracer version 1.0 sample file.

// Drums, the new improved version  Feb. 1992
// Copyright Dan Farmer 1992
// Time: a little over 3 hours no anti-aliasing, 640,480 on 33/486
//       7 hrs, 22 min +a0.05

/*  Fractint 17 parameter file to reconstruct REDNEWT.GIF:
Clip the Red Newton block below and save to a file named REDNEWT.PAR.
Run FRACTINT, press @ to get to the par file screen, press F6 to select
REDNEWT.PAR. Make it as large as you think your memory will afford... I used
1024x768 myself.


Red Newton         { ; Used as backdrop in DRUMS2.GIF
  reset type=complexnewton passes=1
  corners=-4.316552/-6.725229/9.54158/9.251383/-5.04438/10.512019
  params=8/0/4/8 maxiter=32767 inside=0 periodicity=0
  colors=000700<13>000000000000000<124>y00z00y00<108>800
  }

*/

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"
#include "drums.inc"

max_trace_level 20

/*----------------------------- Viewer and lights -----------------------------*/
camera {
   location <0.0  3.0  -90.0>
   direction <0.0 0.0  2.0>
   //   direction <0.0 0.0  3.0>  (Close up view, nice, too.)
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0.0 8.0 0.0>
}


#declare Spacing = 15
#declare Brightness = 0.5
// Light source #1
composite {
   object {
      light_source { <-Spacing  0.0  0.0>
         color red Brightness
      }
   }
   object {
      light_source { <0.0  Spacing  0.0>
         color green Brightness
      }
   }
   object {
      light_source { <Spacing  0.0  0.0>
         color blue Brightness
      }
   }
   translate <0 0 -100 >
   rotate <80 0 0>
}
// Light source #2
composite {
   object {
      light_source { <-Spacing  0.0  0.0>
         color red Brightness
      }
   }
   object {
      light_source { <0.0  Spacing  0.0>
         color green Brightness
      }
   }
   object {
      light_source { <Spacing  0.0  0.0>
         color blue Brightness
      }
   }
   translate <0 0 -100 >
   rotate <30 30 0>
}
// Light source #3
composite {
   object {
      light_source { <-Spacing  0.0  0.0>
         color red Brightness
      }
   }
   object {
      light_source { <0.0  Spacing  0.0>
         color green Brightness
      }
   }
   object {
      light_source { <Spacing  0.0  0.0>
         color blue Brightness
      }
   }
   translate <0 0 -100 >
   rotate <30 -30 0>
}


object {
   sphere { <0 0 0> 1000 }
   texture {
      image_map { 0
         gif
         "rednewt.gif"
         interpolate 4
      }
      ambient 1
      diffuse 0.5
      translate <-0.5 -0.5 0.0>
      scale <800 800 800>
   }
}

// Floor
object {
   plane { <0.0 1.0 0.0> -10.0  }
   color White
   texture {
      color Black
      ambient 0.2
      diffuse 0.8
      reflection 0.25
   }
}


// Left drumset

composite { HalfSet
   rotate <0.0 +20.0 0.0>
   translate <-13.0 0.0 0.0>
}


// Right drumset
composite { HalfSet
   rotate <0.0 -20.0 0.0>
   translate <+13.0 0.0 0.0>
}
