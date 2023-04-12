// Persistence Of Vision raytracer version 1.0 sample file.

// piece I: 
//          by Truman Brown 11/91
//          Close-up of a museum piece in the Woild Museum

#include "colors.inc"
#include "textures.inc"
#include "shapes.inc"

camera {
   location  <-36.0     29.0 -70.0>
   direction <  0.0      0.0   2.0  >
   up        <  0.2      0.8   0.0  >
   right     <  1.33333  0.0   0.0  >
   look_at   <  0.0     -8.0   0.0  >
}

// ----------------------------------------------------------------------------
// lights
// ----------------------------------------------------------------------------
#declare Light1 =  color red 0.1 green 0.1 blue 0.1
#declare Light2 =  color red 0.2 green 0.2 blue 0.2
#declare Light3 =  color red 0.3 green 0.3 blue 0.3
#declare Light4 =  color red 0.4 green 0.4 blue 0.4
#declare Light5 =  color red 0.5 green 0.5 blue 0.5

#declare light1 = object { light_source { <0.0 0.0 0.0>  color Light1 } }
#declare light2 = object { light_source { <0.0 0.0 0.0>  color Light2 } }
#declare light3 = object { light_source { <0.0 0.0 0.0>  color Light3 } }
#declare light4 = object { light_source { <0.0 0.0 0.0>  color Light4 } }
#declare light5 = object { light_source { <0.0 0.0 0.0>  color Light5 } }

composite {
   object { light1 translate <  35  30 -16>  }
   object { light3 translate < -25  40  20>  }
   object { light2 translate < -48  30 -20>  }
   object { light5 translate <  20 -10 -22>  }
}

// ----------------------------------------------------------------------------
// textures
// ----------------------------------------------------------------------------
#declare rod_texture1 =
texture {
   color Firebrick
   specular 0.9
   roughness 0.001
   ambient 0.0
   diffuse 0.5
   reflection 0.25
}

#declare loop_texture =
texture {
   color Black
   ambient 0.01
   diffuse 0.45
   specular 1.00
   roughness 0.001
   reflection 0.4
}

#declare innertube_texture =
texture {
   color SteelBlue
   ambient 0.05
   diffuse 0.1
   specular 1.00
   roughness 0.001
   reflection 0.90
}

#declare balls_texture =
texture {
   color White
   ambient 0.01
   diffuse 0.3
   specular 1.00
   roughness 0.001
   reflection 0.85
}

#declare tile1_texture =
texture {
   0.05
   wood
   turbulence 0.5
   color_map {
      [0.0 0.7  color red 0.93 green 0.710 blue 0.532
      color red 0.98 green 0.810 blue 0.600]
      [0.7 1.01 color red 0.60 green 0.333 blue 0.266
      color red 0.70 green 0.600 blue 0.23]
   }
   specular 1.0
   roughness 0.001
   scale <5 5 5>
   ambient 0.05
   diffuse 0.35
   reflection 0.15
   rotate <0 90 0>
}

#declare tile2_texture =
texture {
   0.05
   wood
   turbulence 0.6
   color_map {
      [0.0 0.8  color red 0.42857 green 0.23810 blue 0.04762
      color red 0.42857 green 0.23810 blue 0.04762]
      [0.8 1.01 color red 0.4     green 0.333   blue 0.066
      color red 0.2     green 0.033   blue 0.033]
   }
   specular 1.0
   roughness 0.001
   scale <7 7 7>
   ambient 0.01
   diffuse 0.75
   reflection 0.15
}

#declare frame_texture =
texture {
   Bronze_Texture
   specular 1.0
   roughness 0.001
   ambient 0.11
   diffuse 0.8
   reflection 0.85
}

// ----------------------------------------------------------------------------
// torus primatives
// ----------------------------------------------------------------------------
#declare torus1 =
quartic { <
   1.0 0.0   0.0  0.0   2.0      /* major radius: 5.0 */
   0.0 0.0   2.0  0.0 -50.5      /* minor radius: 0.5 */
   0.0 0.0   0.0  0.0   0.0      /* outer radius: 5.5 */
   0.0 0.0   0.0  0.0   0.0      /* inner radius: 4.5 */
   1.0 0.0   0.0  2.0   0.0
   49.5 0.0   0.0  0.0   0.0
   1.0 0.0 -50.50 0.0 612.5625 >
}

#declare torus2=
quartic { <
   1.0 0.0  0.0  0.0  2.0        /* major radius: 0.8 */
   0.0 0.0  2.0  0.0 -1.36       /* minor radius: 0.2 */
   0.0 0.0  0.0  0.0  0.0        /* outer radius: 1.0 */
   0.0 0.0  0.0  0.0  0.0        /* inner radius: 0.6 */
   1.0 0.0  0.0  2.0  0.0
   1.2 0.0  0.0  0.0  0.0
   1.0 0.0 -1.36 0.0  0.36 >
}

#declare torus3 =
quartic { <
   1.0 0.0    0.0 0.0     2.0     /* major radius: 12.0 */
   0.0 0.0    2.0 0.0  -296.0     /* minor radius:  2.0 */
   0.0 0.0    0.0 0.0     0.0     /* outer radius: 14.0 */
   0.0 0.0    0.0 0.0     0.0     /* inner radius: 10.0 */
   1.0 0.0    0.0 2.0     0.0
   280.0 0.0    0.0 0.0     0.0
   1.0 0.0 -296.0 0.0 19600.0 >
}

#declare torus4 =
quartic { <
   1.0 0.0   0.0 0.0    2.0       /* major radius: 7.0 */
   0.0 0.0   2.0 0.0  -98.5       /* minor radius: 0.5 */
   0.0 0.0   0.0 0.0    0.0       /* outer radius: 7.5 */
   0.0 0.0   0.0 0.0    0.0       /* inner radius: 6.5 */
   1.0 0.0   0.0 2.0    0.0
   97.5 0.0   0.0 0.0    0.0
   1.0 0.0 -98.5 0.0 2376.5625 >
}

// ----------------------------------------------------------------------------
// piece definitions
// ----------------------------------------------------------------------------
#declare halfring =
object {
   quartic { torus1 rotate <90 0 0>  }
   clipped_by {
      plane { <0 -1 0> 0  }
   }
   texture {
      rod_texture1
   }
   bounded_by {
      sphere { <0 0 0> 5.51  }
   }
   translate <-12 0 0>
}

#declare halfring_array =
composite {
   object { halfring rotate <0  -10 0>  }
   object { halfring rotate <0  -40 0>  }
   object { halfring rotate <0 -100 0>  }
   object { halfring rotate <0 -130 0>  }
   object { halfring rotate <0 -190 0>  }
   object { halfring rotate <0 -220 0>  }
   object { halfring rotate <0 -280 0>  }
   object { halfring rotate <0 -310 0>  }
   bounded_by {
      sphere { <0 0 0> 17.51  }
   }
}

#declare quarterring =
object {
   quartic { torus1 rotate <90 0 0>  }
   clipped_by {
      plane { <0 1 0> 0  }
      plane { <1 0 0> 0  }
   }
   texture {
      rod_texture1
   }
   bounded_by {
      sphere { <-2.75 -2.75 0> 3.9  }
   }
   translate <-12 0 0>
}

#declare quarterring_array =
composite {
   object { quarterring rotate <0  -10 0>  }
   object { quarterring rotate <0  -40 0>  }
   object { quarterring rotate <0 -100 0>  }
   object { quarterring rotate <0 -130 0>  }
   object { quarterring rotate <0 -190 0>  }
   object { quarterring rotate <0 -220 0>  }
   object { quarterring rotate <0 -280 0>  }
   object { quarterring rotate <0 -310 0>  }
   bounded_by {
      sphere { <0 0 0> 17.51  }
   }
}

#declare ringball =
object {
   sphere { <0 -5.0 0> 1.0  }
   texture {
      rod_texture1
   }
   translate <-12 0 0>
}

#declare ringball_array =
composite {
   object { ringball rotate <0  -10 0>  }
   object { ringball rotate <0  -40 0>  }
   object { ringball rotate <0 -100 0>  }
   object { ringball rotate <0 -130 0>  }
   object { ringball rotate <0 -190 0>  }
   object { ringball rotate <0 -220 0>  }
   object { ringball rotate <0 -280 0>  }
   object { ringball rotate <0 -310 0>  }
   bounded_by {
      sphere { <0 0 0> 13.5  }
   }
}

#declare pole =
object {
   intersection {
      Disk_Y
      rotate <180 0 0>
      scale <0.5 20 0.5>
      translate <5 0 0>
   }
   texture {
      rod_texture1
   }
   translate <-12 0 0>
}

#declare pole_array =
composite {
   object { pole rotate <0  -10 0>  }
   object { pole rotate <0  -40 0>  }
   object { pole rotate <0 -100 0>  }
   object { pole rotate <0 -130 0>  }
   object { pole rotate <0 -190 0>  }
   object { pole rotate <0 -220 0>  }
   object { pole rotate <0 -280 0>  }
   object { pole rotate <0 -310 0>  }
   bounded_by {
      sphere { <0 -10 0> 13.5  }
   }
}

#declare blueloop =
object {
   quartic { torus2 rotate <0 0 90>  }
   texture {
      loop_texture
   }
   bounded_by {
      sphere { <0 0 0> 1.1  }
   }
}

#declare blueloop_set =
composite {
   object { blueloop translate <0 5 0> rotate <0 0 0>  }
   object { blueloop translate <0 5 0> rotate <0 0 90>  }
   object { blueloop translate <0 5 0> rotate <0 0 -90>  }
   bounded_by {
      sphere { <0 0 0> 6.51  }
   }
   translate <-12 0 0>
}

#declare blueloop_array =
composite {
   composite { blueloop_set rotate <0  -10 0>  }
   composite { blueloop_set rotate <0  -40 0>  }
   composite { blueloop_set rotate <0 -100 0>  }
   composite { blueloop_set rotate <0 -130 0>  }
   composite { blueloop_set rotate <0 -190 0>  }
   composite { blueloop_set rotate <0 -220 0>  }
   composite { blueloop_set rotate <0 -280 0>  }
   composite { blueloop_set rotate <0 -310 0>  }
   bounded_by {
      sphere { <0 0 0> 18.51  }
   }
}

#declare basering =
object {
   quartic { torus4  }
   texture {
      rod_texture1
   }
   bounded_by {
      sphere { <0 0 0> 7.6  }
   }
   translate <0 -20.5 0>
}

#declare innertube =
object {
   quartic { torus3  }
   texture {
      innertube_texture
   }
   bounded_by {
      sphere { <0 0 0> 14.1  }
   }
}

#declare structurebase =
composite {
   composite { halfring_array     }
   composite { quarterring_array  }
   composite { ringball_array     }
   composite { blueloop_array     }
   composite { pole_array         }
   object { basering   }
   object { innertube  }
   bounded_by {
      sphere { <0 0 0> 22  }
   }
}

#declare balls =
object {
   sphere { <0 0 0> 4.0  }
   texture {
      balls_texture
   }
}

#declare block1 =
object {
   intersection {
      intersection {
         Cube
         scale <10 1 10>
      }
      quadric {
         Cylinder_Y
         scale <6 1 6>
         inverse
      }
   }
   texture {
      tile1_texture
   }
   bounded_by {
      sphere { <0 0 0> 15  }
   }
}

#declare block2 =
object {
   intersection {
      intersection {
         Cube
         scale <10 1 10>
      }
      quadric {
         Cylinder_Y
         scale <6 1 6>
         inverse
      }
   }
   texture {
      tile2_texture
   }
   bounded_by {
      sphere { <0 0 0> 15  }
   }
}

#declare frame =
object {
   intersection {
      Cube
      scale <21 1.5 21>
   }
   clipped_by {
      intersection {
         Cube
         scale <20 2 20>
         inverse
      }
   }
   texture { frame_texture  }
}

#declare bigtile =
composite {
   object { block1 translate < 10 -1  10>  }
   object { block1 translate <-10 -1 -10>  }
   object { block2 translate <-10 -1  10>  }
   object { block2 translate < 10 -1 -10>  }
   object { frame  }
   bounded_by {
      intersection {
         plane { < 1.0  0.0  0.0> 21.1  }
         plane { <-1.0  0.0  0.0> 21.1  }
         plane { < 0.0  1.0  0.0> 0.6  }
         plane { < 0.0 -1.0  0.0> 2.6  }
         plane { < 0.0  0.0  1.0> 21.1  }
         plane { < 0.0  0.0 -1.0> 21.1  }
      }
   }
}

#declare floor =
composite {
   composite { bigtile  }
}

// color debugger
// object { quadric { Cylinder_Y scale <3 1 3>  } texture { rod_texture1  }  }
// object { quadric { Cylinder_X scale <1 3 3>  } texture { rod_texture1  }  }

//-----------------------------------------------------------------------------
// Put it all together and render it
//-----------------------------------------------------------------------------
composite { structurebase  }
composite {
   object { balls translate <0  0 0>  }
   object { balls translate <0 -8 0>  }
   object { balls translate <0 -16 0>  }
   bounded_by {
      quadric {
         Cylinder_Y
         scale <4.1 1 4.1>
      }
   }
}
composite { floor rotate <0 45 0> translate < 0 -20 0>  }
