// Persistence Of Vision raytracer version 1.0 sample file.

// By Dan Farmer
/* Script for a lawn with a swimming pool.
  Written by Dan Farmer.  Takes quite a while to render because of
  the many textures and waveforms used.  An interesting variation of this
  theme would be to change the view to look into the garden globe and see
  the rest of the scene as the reflection in the globe.

  As stated below, most of the garden globe structure was taken from the
  file "roman.pov".
*/

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

#declare Cube = intersection {
   plane { <0.0 0.0 1.0> 1.0 }
   plane { <0.0 0.0 -1.0> 1.0 }
   plane { <0.0 1.0 0.0> 1.0 }
   plane { <0.0 -1.0 0.0> 1.0 }
   plane { <1.0 0.0 0.0> 1.0 }
   plane { <-1.0 0.0 0.0> 1.0 }
   scale <1.0 1.0 1.0>
}

/* Most of the garden globe is borrowed from "roman.dat"*/
/********************************************************/
#declare Beam = quadric { Cylinder_Y
   scale <0.5 20.0 0.5>
   translate <2.0 0.0 0.0>
}

/* create a sample column for the base of the structure */
#declare BaseColumn = object {
   intersection {
      union {
         quadric { Beam }
         quadric { Beam rotate <0.0 -25.7  0.0> }
         quadric { Beam rotate <0.0 -51.4  0.0> }
         quadric { Beam rotate <0.0 -77.1  0.0> }
         quadric { Beam rotate <0.0 -102.8 0.0> }
         quadric { Beam rotate <0.0 -128.5 0.0> }
         quadric { Beam rotate <0.0 -154.2 0.0> }
         quadric { Beam rotate <0.0 -179.9 0.0> }
         quadric { Beam rotate <0.0 -205.6 0.0> }
         quadric { Beam rotate <0.0 -231.3 0.0> }
         quadric { Beam rotate <0.0 -257.0 0.0> }
         quadric { Beam rotate <0.0 -282.7 0.0> }
         quadric { Beam rotate <0.0 -308.4 0.0> }
         quadric { Beam rotate <0.0 -334.1 0.0> }
      }
      plane { <0.0 1.0 0.0> 20.0 }
      plane { <0.0 -1.0 0.0> 0.0 }
   }
   bounded_by {
      intersection {
         plane { <0.0 1.0 0.0> 20.0 }
         plane { <0.0 -1.0 0.0> 0.0 }
         quadric {
            Cylinder_Y
            scale <2.51 1.0 2.51>
         }
      }
   }
   texture {
      White_Marble
      scale <10.0 10.0 10.0>
      ambient 0.4
      diffuse 0.9
      reflection 0.1
   }
   colour red 0.8 green 0.0 blue 0.0
}

/* and a rectangular pad to serve as a footing for the column */
#declare BasePad = object {
   intersection { Cube
      scale <4.0 1.0 4.0>
   }
   bounded_by {
      quadric {
         Ellipsoid
         scale <10.0 3.0 10.0>
      }
   }
   texture {
      White_Marble
      scale <10.0 10.0 10.0>
      ambient 0.4
      diffuse 0.9
      reflection 0.1
   }
   colour red 0.6 green 0.6 blue 0.4
}

#declare Globe = object {
   sphere { <0.0 0.0 0.0> 5.0 }
   texture {
      Mirror
      color Blue
   }
   color Blue
}

#declare Garden_Globe = composite {
   object { BaseColumn translate <0.0 0.0 0.0>   }
   object { BasePad    translate <0.0 -1.0 0.0>  }
   object { BasePad    translate <0.0 21.0 0.0>  }
   object { Globe      translate<0.0 26.0 0.0> }
}
/********************* End of Garden Globe Epic *******************************/

/* Everybody's gotta have a point of view. */
camera {
   location <0.0  30.0  -120.0>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0.0 0.0 0.0>
}

/* No, fool... i said _bud_ light! */
object {  
   light_source { <100.0  200.0  -200.0>
      colour White
   }
}

/* "The sky was the color of her eyes" */
/*             ... and just as cloudy! */
object {
   sphere { <0.0 -10000.0 -200.0> 15000.0 }
   texture {
      0.05
      Bright_Blue_Sky
      scale <7500.0 1000.0 5000.0>
      ambient 0.7
      diffuse 0.0
   }
   colour red 0.5 green 0.5 blue 1.0
}

/* Plant a little grass, well mowed. */
object {
   plane { <0.0 1.0 0.0> 0.0 }
   texture {
      0.05  /* This value dithers the colours */
      color Green
      ripples 0.5
      frequency 2000.0
      scale <50000.0 50000.0 50000.0>
   }
   color Green
}

/* Now, we pour the concrete and set the tiles...*/
object {
   difference {
      intersection { Cube
         scale <45.0 10.0 60.0>
      }
      intersection { Cube /* Inside box */
         scale <35.0 11.0 50.0>
      }
   }
   translate <0.0 -8.0 0.0 >
   texture {
      checker colour DarkTurquoise colour White
      scale <2.0 2.0 2.0>
   }
   color DarkTurquoise
}

/* Better put some water in the pool! */
object {
   intersection { Cube
      scale <35.0 10.0 50.0>
      translate<0.0 -9.0 0.0>
   }
   texture {
      0.05
      color LightBlue               /*alpha 0.5 ?*/
      ripples 0.5
      frequency 100.0
      scale <5.0 5.0 10.0>
      translate <20.0 4.0 -15.0>    /* Where the ball floats?? */
      reflection 0.5
      brilliance 3.0
      /*??     refraction 1.0		{ Doesn't really help the scene any... }
        ior 1.2 ?? */
   }
   color LightBlue
}

/* Let's add a diving board */
object {
   union {
      intersection { Cube                        /* The board itself */
         scale <6.0 1.0 30.0 >
         translate <0.0 2.0 -30.0>
      }
      intersection { Cube                        /* a block under the board */
         scale <6.0 2.0 2.0 >
         translate <0.0 1.0 -30.0>
      }
   }
   texture {
      Cherry_Wood
      0.05
      scale <0.02 0.02 0.02>
   }
   color Brown
}

/* Float a red and white striped ball in the pool */
object {
   sphere { <0.0 0.0 0.0> 5.0 }
   translate <20.0 4.0 -15.0>    /* Sorta right front center of pool */
   texture {
      gradient < 1.0 1.0 0.0 >
      colour_map {
         [0.00 0.25  colour White colour White ]
         [0.25 0.75  colour red 1.0  colour red 1.0]
         [0.75 1.001 colour White colour White ]
      }
      scale <7.0 7.0 7.0>
      rotate <-30.0 30.0 0.0>
      ambient 0.3
      diffuse 0.7
      phong 0.5
      phong_size 10.0
   }
   color Red
}

/* Place the garden globe on left side of pool */
composite { Garden_Globe translate <-60.0 0.0 0.0> }

/* The hedge behind the pool */
object {
   intersection { Cube }
   scale <200.0 30.0 30.0>
   translate <-100.0 0.0 180.0>
   texture {
      0.05  /* This value dithers the colours */
      color YellowGreen
      turbulence 0.5
      spotted 0.5
      colour_map {
         [0.00 0.25  colour YellowGreen colour Green ]
         [0.25 0.75  colour Green colour DarkGreen]
         [0.75 1.001 colour DarkGreen colour YellowGreen ]
      }
      wrinkles 1.0
      frequency 2000.0
      scale <10.0 10.0 20.0>
   }
   color YellowGreen
}

/* The hedge on the right side of pool */
object {
   intersection { Cube }
   scale <30.0 30.0 100.0>
   translate <100.0 0.0 -85.0>
   texture {
      0.05  /* This value dithers the colours */
      color YellowGreen
      turbulence 0.5
      spotted 0.5
      colour_map {
         [0.00 0.25  colour YellowGreen colour Green ]
         [0.25 0.75  colour Green colour DarkGreen]
         [0.75 1.001 colour DarkGreen colour YellowGreen ]
      }
      wrinkles 1.0
      frequency 2000.0
      scale <10.0 10.0 20.0>
   }
   color YellowGreen
}

/* a low, squat shrub of some generic species */
object {
   quadric { Ellipsoid }
   scale <30.0 20.0 25.0>
   translate <-70.0 0.0 110.0>
   texture {
      0.05  /* This value dithers the colours */
      color YellowGreen
      turbulence 0.5
      spotted 1.0
      colour_map {
         [0.00 0.25  colour Khaki colour Green ]
         [0.25 0.50  colour Green colour DarkGreen]
         [0.50 0.75  colour DarkGreen colour MediumForestGreen]
         [0.75 1.001 colour MediumForestGreen colour YellowGreen ]
      }
      wrinkles 1.0
      frequency 2000.0
      scale <5.0 5.0 5.0>
   }
   color YellowGreen
}
