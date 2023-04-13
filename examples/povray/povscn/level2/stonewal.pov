// Persistence Of Vision raytracer version 1.0 sample file.

// By Dan Farmer
//    Minneapolis, mn
/*
      a pastoral scene with a granite stone fence.  This was never really
      "finished", but it works as it is.  Plenty of material to play around
      with here.  The basic building blocks are here... just use your
      imagination.
*/ 
#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

camera {
   location <50.0  40.0  -350.0>
   look_at <-100.0 0.0 0.0>
   direction <0.0 0.0 2.0>
   right < 1.3333 0.0 0.0 >
}

/* Add light source */
object {
   light_source { <100.0  200.0  -630.0>
      color White
   }
}

object {
   light_source { <50.0  20000.0 50.0>
      color White
   }
}

/* Add the sky to the picture */
object {
   sphere { <0.0 0.0 0.0> 1000000.0 }
   texture {
      color SkyBlue
      diffuse 0
      ambient 1
   }
   color SkyBlue
}

/* Grassy meadow */
object {
   plane { <0.0 1.0 0.0> -2.0 }
   texture {
      0.05  /* This value dithers the colors */
      color red 0.2 green 1.0 blue 0.4
      waves 1.0
      frequency 2000.0
      scale <50000.0 50000.0 50000.0>
      ambient 0.1
      diffuse 0.3
   }
   color red 0.2 green 1.0 blue 0.4
}

#declare Wall_Segment = object {
   intersection { Cube }
   scale <8.0 50.0 100.0>
   texture {
      0.05
      granite
      ambient 0.2
      diffuse 0.8
      brilliance 7.0
      reflection 0.1
      phong 1.0
      phong_size 60
   }
   color White
}

#declare Granite_Ball = object {
   sphere { <0.0 0.0 10.0> 10.0 }
   texture {
      0.05
      granite
      ambient 0.2
      diffuse 0.8
      brilliance 7.0
      reflection 0.1
      phong 1.0
      phong_size 60
   }
   color White
}

#declare Column = object {
   intersection { Cube }
   scale <12.0 60.0 12.0>
   texture {
      0.05
      granite
      ambient 0.2
      diffuse 0.8
      brilliance 7.0
      reflection 0.1
      phong 1.0
      phong_size 60
   }
   color White
}

/* note: Granite column is 12 wide x 12 deep x 60 high
        Granite ball has a radius of 20 */
#declare Pillar = composite {
   object { Column       translate <0.0  0.0 0.0> }
   object { Granite_Ball translate <0.0 70.0 0.0> }
}

/* Note: Wall segments are 100 units long, Pillars are 12 units "long" */
composite {
   Pillar
   translate <0.0 0.0 -106.0 >
}
object {
   Wall_Segment
   translate <-2.0 0.0 -6.0 >
}
composite {
   Pillar
   translate <0.0 0.0 0.0 >
}
object {
   Wall_Segment
   translate <-2.0 0.0 112.0 >
}
composite {
   Pillar
   translate <0.0 0.0 112.0 >
}
object {
   Wall_Segment
   rotate <0.0 -90.0 0.0>
   translate <76.0 0.0 114.0 >
}
composite {
   Pillar
   translate <176.0 0.0 112.0 >
}
