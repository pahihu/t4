// Persistence Of Vision Raytracer version 1.0 sample file.


//----------- fish out of water ----------- 3/10/92 miller

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"
#include "fish.inc"
#include "world12.inc"
#include "stem1.inc"

camera {
   location <-5 0 -105> direction <0 0 1.5>
   up <0 1 0> right <1.333 0 0>
   look_at <-5 -5 0>
}

//-------light
object { light_source { <200 100 -100> color White } }



//-------light
object { light_source {  <1000 500 400> color White } }


composite { fish rotate <0 -30 0>  }
object { world12  }

composite { stem1 scale <3 3 3> rotate <0 0 0> translate <80 -25 150>  }
composite { stem1 scale <1.5 1.5 1.5> rotate <0 80 0> translate <70 -25 90>  }




/*
FOG
20000
COLOR DimGray
END_FOG
*/


/*-------------- SWAMP WATER ----------------------------*/
object { 
   intersection { Cube scale <10000 1 500> translate <0 -25 0>  }
   texture {
      color red 0.0 green 0.07 blue 0.0
      reflection 0.45
      ambient 0.15
      //color MidnightBlue
      ripples 0.7 frequency 0.08
      translate <0 0 0>
      diffuse 0.6 phong 1.0 phong_size 80
   }
}






