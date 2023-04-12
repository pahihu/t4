// Persistence Of Vision Raytracer version 1.0 sample file.

// Scene file by Dan Farmer    November, 1991
//
// note: To properly process on a pc, the resultant .gif file should
// be modified with piclab: brighten 20 contrast 45, gamma 0.6, transform
//
// Many of the declarations here are not the same as in the #include files!
//=========================================================================
#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

#declare BlueViolet = color red 0.38 green 0.39 blue 0.58 // tce designed color
#declare DarkPurple     = color red 0.53 green 0.12 blue 0.47
#declare CoolCopper     = color red 0.85 green 0.53 blue 0.10
#declare LightWood      = color red 0.91 green 0.76 blue 0.65
#declare MediumWood     = color red 0.65 green 0.50 blue 0.39
#declare DarkWood       = color red 0.52 green 0.37 blue 0.26
#declare SemiSweetChoc  = color red 0.42 green 0.26 blue 0.15
#declare BakersChoc     = color red 0.36 green 0.20 blue 0.09
#declare VeryDarkBrown  = color red 0.35 green 0.00 blue 0.14

// Shiny creates a small, tight highlight on the object {'s surface
#declare Shiny = texture {
   specular 1.0  roughness 0.001
}

// You need to specify a color when Metal is used
#declare Metal = texture {
   metallic
   ambient 0.085   diffuse 0.6
   brilliance 6.0
   reflection 0.25
   phong 0.75  phong_size 20
}

#declare Copper_Texture = texture {
   Metal
   colour CoolCopper
}

#declare Brass_Texture = texture {
   Metal
   colour Brass
}

// a unit sphere { is one with radius of 1.0.
// This shape { is equivilent in size to a unit sphere {
// and will scale evenly in all directions.
#declare Disk_X = intersection {       /* Capped cylinder, Length in x axis */
   quadric { Cylinder_X }
   plane { <1.0 0.0 0.0> -1.0 inverse }
   plane { <1.0 0.0 0.0> 1.0 }
}

#declare Disk_Y = intersection {       /* Capped cylinder, Length in y axis */
   quadric { Cylinder_Y }
   plane { <0.0 1.0 0.0> -1.0 inverse }
   plane { <0.0 1.0 0.0> 1.0 }
}

#declare Disk_Z = intersection {       /* Capped cylinder, Length in z axis */
   quadric { Cylinder_Z }
   plane { <0.0 0.0 1.0> -1.0 inverse }
   plane { <0.0 0.0 1.0> 1.0 }
}

#declare Torus =
quartic {
   <    1.000000 0.000000 0.000000 0.000000 2.000000
   0.000000 0.000000 2.000000 0.000000 -2.125000
   0.000000 0.000000 0.000000 0.000000 0.000000
   0.000000 0.000000 0.000000 0.000000 0.000000
   1.000000 0.000000 0.000000 2.000000 0.000000
   1.875000 0.000000 0.000000 0.000000 0.000000
   1.000000 0.000000 -2.125000 0.000000 0.878906 >
}

#declare LeftTopElbow = object {
   quartic { Torus rotate <90 0 0> }
   clipped_by {
      plane { <1  0 0> 0 }
      plane { <0 -1 0> 0 }
   }
   color Red
   texture { Copper_Texture }
}
#declare RightTopElbow = object {
   quartic { Torus rotate <90 0 0> }
   clipped_by {
      plane { <-1 0 0> 0 }
      plane { <0 -1 0> 0 }
   }
   color Red
   texture { Copper_Texture }
}

#declare XTube = object {
   quadric { Cylinder_X }
   clipped_by {
      plane { <1  0 0> -1 inverse }
      plane { <1 0 0>   1 }
   }
   color Red
   texture { Copper_Texture }
}

#declare YTube = object {
   quadric { Cylinder_Y }
   clipped_by {
      plane { <0 1 0> -1 inverse }
      plane { <0 1 0>  1 }
   }
   color Red
   texture { Copper_Texture }
}

camera {
   location <20 15 -35>
   direction <0 0 1.2>
   up  <0 1 0>
   right <1.33333 0 0>
   look_at <1.5 5 0>
}

#declare Light = color red 0.75 green 0.75 blue 0.75

object {   /* light */
   light_source { <-30 500 -1.0>
      colour Light
   }
}

object {   /* light */
   light_source { <-5 30 -50>
      colour Light
   }
}


object {   /* floor */
   plane { <0 1 0> -6 }
   color White
   texture { granite
      reflection 0.15
      ambient 0.075  diffuse 0.5
      color_map { [0.0 1.01 color BlueViolet color DarkPurple] }
   }
}

#declare Oak =  texture {
   wood
   turbulence 0.05
   colour_map {
      [0.00 0.10 color SemiSweetChoc color SemiSweetChoc]
      [0.10 0.90 color SemiSweetChoc color DarkWood]
      [0.90 1.01  color DarkWood  color DarkWood ]
   }
}

#declare WoodSphere = object {
   sphere { <0 0 0> 10 }
   colour White

   texture {
      Shiny
      reflection 0.12
      Oak
      scale <0.5 0.5 1.0>
      translate <0.25 0.05 0.5>
      rotate <0 0 0.5>
      ambient 0.075 diffuse 0.7
   }
   texture {
      Shiny
      color Clear
      ambient 0.075 diffuse 0.7
   }
}

#declare Chamfer = union {
   intersection { Disk_X scale <12.5 0.65 0.65> translate <0 0 -12> }
   intersection { Disk_X scale <12.5 0.65 0.65> translate <0 0 12>  }
   intersection { Disk_Z scale <0.65 0.65 12.5> translate <-12 0 0> }
   intersection { Disk_Z scale <0.65 0.65 12.5>  translate <12 0 0> }
}

#declare BrassBall = object {
   sphere { <0 0 0> 1.5 }
   color Cyan
   texture { Brass_Texture }
}

#declare Railing = composite {
   object { LeftTopElbow  translate <-8 0 0> }
   object { RightTopElbow translate <+8 0 0> }
   object { XTube  scale <8.0 0.25 0.25> translate <0 1 0>  }
   object { YTube  scale <0.25 2 0.25>    translate <+9 -2 0> }
   object { YTube  scale <0.25 2 0.25>    translate <-9 -2 0> }
}

#declare Base =  object {
   difference {
      intersection { Cube scale <12 2 12> }
      union { Chamfer translate <0 2 0> }
   }
   color Red

   texture {
      Shiny
      reflection 0.15
      Oak scale <0.45 0.45 0.7>
      rotate <0.20 0.10 0.05>
      translate <0 -1 0>
      ambient 0.075 diffuse 0.7
   }
   texture {
      Shiny
      color Clear
      ambient 0.075 diffuse 0.7
   }
   translate <0 -1 0>
}

composite {
   object { WoodSphere  translate <0 9.5 0> }
   object { Base }
   object { BrassBall translate < 10 -4.5  10> }
   object { BrassBall translate < 10 -4.5 -10> }
   object { BrassBall translate <-10 -4.5  10> }
   object { BrassBall translate <-10 -4.5 -10> }
   composite { Railing translate <0 3  10> }
   composite { Railing translate <0 3 -10> }
   composite { Railing rotate <0 90 0> translate < 10 3 0> }
   composite { Railing rotate <0 90 0> translate <-10 3 0> }
}
