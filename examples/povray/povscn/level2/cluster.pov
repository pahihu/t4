// Persistence of Vision Raytracer
// Cluster of mirrored spheres 
// This file is essentially a copy of a similar file in MTV, Rayshade and Vivid
// This may look better at high resolutions 
// with max_trace_level set to 10 or 15

#include "colors.inc"

camera { // This is an unusual camera since it is a converted file
   location <2.31 1.43 1.65>
   up <0 0 1>          // The Z axis is up and
   right <-1.3333 0 0> // The negative X axis is right
   direction <0 -1.60746 0> // Field of view 45 degrees
   sky <0 0 1>
   look_at <0 0 -0.1>
}
object {sphere { <0 0 0> 10000 } texture { color SkyBlue ambient 1 diffuse .5} }
object { light_source { <4 3 2 > color Gray30 } }
object { light_source { <1 -4 4> color Gray30 } }
object { light_source {<-3 1 5> color Gray30 } }

object { plane { <0 0 1> -.5 } texture { color red 1.0 green 0.75 blue 0.33 diffuse 1 } } // ambient .15 .1 .045 
/*object {
  union {
    sphere { <.7 0 0> .2 texture {color red 1 diffuse .7} }
     sphere { <0 .7 0> .2  texture {color green 1 diffuse .7}}
      sphere { <0 0 .7> .2  texture {color blue 1 diffuse .7}}
      }
}*/
object { 
   sphere { <0 0 0> .5 }
   texture { 
      bumps .4
      //turbulence .5
      // octaves 5
      scale <.025 .025 .025>
      //scale <.05 .05 .05>
      ambient .03  //amb 0.035 0.0325 0.025
      diffuse 1
      color red 0.5 green 0.45 blue 0.35
      reflection .4     
      phong 1 
      phong_size 3
   }
}
object {
   union {
      sphere { <0.272166 0.272166 0.544331> 0.166667  }
      sphere {<0.643951 0.172546 0> 0.166667  }
      sphere {<0.172546 0.643951 0> 0.166667 }
      sphere {<-0.371785 0.0996195 0.544331> 0.166667  }
      sphere {<-0.471405 0.471405 0> 0.166667  }
      sphere {<-0.643951 -0.172546 0> 0.166667  }
      sphere {<0.0996195 -0.371785 0.544331> 0.166667  }
      sphere {<-0.172546 -0.643951 0> 0.166667  }
      sphere { <0.471405 -0.471405 0> 0.166667  }
   }
   texture { 
      ambient .035  //amb 0.035 0.0325 0.025
      color red 0.5 green 0.45 blue 0.35
      reflection .4
      diffuse 1
      phong 1 
      phong_size 3
   }
}
