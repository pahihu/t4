// Persistence Of Vision Raytracer version 1.0 sample file.

#include "colors.inc"
#include "mmshape.inc"
#include "textures.inc"
#include "thesnail.inc"
#include "hallway.inc"


camera {
   location <0 250 -300>
   direction <0 0 1>
   up <0 1 0>
   right <1.333 0 0>
   look_at <0 58 0>
}


object {
   sphere { <0 0 0> 5000 }
   texture { color MidnightBlue ambient 0.7 diffuse 0.0 }
}

object { light translate <600 400 -1000> }
object { light translate <-1000 2000 -200> }

/*------------------------ The Wet Spot ------------------------*/
#declare puddle = object {
   union {
      quadric { Ellipsoid scale <100 2 50> }
      quadric { Ellipsoid scale <150 2 70> translate <100 0 10> }
   }
   texture { Mirror }
}


/*---------------- BUILD STAGE --------------------------------------------*/

composite { the_floor scale <0.6 0.6 0.6> rotate <90 0 0> translate <50 0 120> }
//composite { thesnail scale <1.5 1.5 1.5> rotate <0 -12 0> translate <-130 20 -60> }
//composite { wall translate <-310 0 200> }
//composite { wall scale <1.4 0.8 1> translate <200 0 530> }
composite { body scale <1.7 1.7 1.7> rotate <0 -90 0> rotate <0 0 -5> translate <245 -23 -75> }
object { puddle translate <100 5 -95> }


/*-----------------TEST FLOOR -----------------------*/
/*
OBJECT 
PLANE <0 1 0> 0 END_PLANE
TEXTURE COLOR Gray END_TEXTURE
END_OBJECT
*/ 
