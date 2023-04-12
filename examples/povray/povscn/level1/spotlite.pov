// Sample file demonstrating a spotlight.   
// POV-Ray input file - Alexander Enzmann & CdW

#include "colors.inc"
#include "textures.inc"


camera {
   location <0 5 -12>
   direction <0 0 1.5>
   up      <0 1 0>
   right   <1.333 0 0>
   look_at <0 0 0>
}

// This is the spotlight.
object {
   union {
      sphere { <10 10 0> 0.5 }
      light_source {
         <10 10 0>
         color red 1 green 1 blue 0.5
         spotlight
         point_at <0 1 0>
         tightness 50
         radius 11
         falloff 25
      }
   }
   no_shadow
}

object {
   light_source {
      <-10 10 0>
      color red 0.5 green 0.5 blue 1.0
      spotlight
      point_at <0 1 0>
      tightness 50
      radius 11
      falloff 25

   }
}

object {
   light_source {
      <0 10 -10>
      color red 1.0 green 0.1 blue 0.1
      spotlight
      point_at <0 1 0>
      tightness 20
      radius 11
      falloff 35
   }
}
// Create a sphere with a checker texture
object {
   sphere { <0 0 0> 2 }
   texture {
      Sapphire_Agate
      specular 0.6
      //phong 1
      //phong_size 10
      ambient 0.2
      diffuse 0.8
   }
}

// Create a ground plane
object {
   plane { <0.0  1.0  0.0> -2.01 }
   texture {
      checker colour White colour DarkSlateGrey
      scale < 2 2 2 >
      ambient 0.2
      diffuse 0.8
   }
}
