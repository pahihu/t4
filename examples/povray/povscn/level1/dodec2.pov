// Persistence of Vision Raytracer
// Image by Dan Farmer.  Wood textures used here have been moved to
// textures.inc (wooden1 is same as DMFWood1, etc.)
// File creates a pentagram made up of dodecahedrons.

#include "shapes.inc"
#include "shapes2.inc"
#include "colors.inc"

#declare AnObject = object {
   intersection {Dodecahedron }
   bounded_by {
      sphere{ <0 0 0> 1.5 }
   }
   translate <0 1.8 0>
   rotate <72 0 0>
}

#declare VeryDarkWood1 = color red 0.30 green 0.15 blue 0.09
#declare DarkWood1 =  color red 0.60 green 0.30 blue 0.18

#declare Wooden1 = texture {
   wood
   turbulence 0.04
   octaves 3
   scale <0.05 .05 1>
   colour_map {
      [0.00 0.10 color DarkWood1 color DarkWood1]
      [0.10 0.90 color DarkWood1 color VeryDarkWood1]
      [0.90 1.01  color VeryDarkWood1  color VeryDarkWood1 ]
   }
   ambient 0.2
   diffuse 0.8
   rotate <0 90 0>
}

#declare DarkWood2 = color red 0.52 green 0.37 blue 0.26
#declare VeryDarkWood2  = color red 0.42 green 0.26 blue 0.15
#declare Wooden2 = texture {
   wood
   turbulence 0.03
   octaves 4
   scale <0.05 .05 1>
   colour_map {
      [0.00 0.10 color DarkWood2 color DarkWood2]
      [0.10 0.90 color DarkWood2 color VeryDarkWood2]
      [0.90 1.01  color VeryDarkWood2  color VeryDarkWood2 ]
   }
   ambient 0.2
   diffuse 0.8
   rotate <0 90 0>
}


#declare DarkWood3  = colour red 0.4 green 0.133 blue 0.066
#declare VeryDarkWood3  = colour red 0.2 green 0.065 blue 0.033
#declare Wooden3 = texture {
   wood
   turbulence 0.05
   octaves 2
   scale <0.05 .05 1>
   colour_map {
      [0.00 0.10 color DarkWood3 color DarkWood3]
      [0.10 0.90 color DarkWood3 color VeryDarkWood3]
      [0.90 1.01  color VeryDarkWood3  color VeryDarkWood3 ]
   }
   ambient 0.2
   diffuse 0.8
   rotate <0 90 0>
}

#declare DarkWood4 = colour red 0.888 green 0.600 blue 0.3
#declare VeryDarkWood4  = colour red 0.6 green 0.4 blue 0.2
#declare Wooden4 = texture {
   wood
   turbulence 0.04
   octaves 3
   scale <0.05 .05 1>
   colour_map {
      [0.00 0.10 color DarkWood4 color DarkWood4]
      [0.10 0.90 color DarkWood4 color VeryDarkWood4]
      [0.90 1.01  color VeryDarkWood4  color VeryDarkWood4 ]
   }
   ambient 0.2
   diffuse 0.8
   rotate <0 90 0>
}

#declare DarkWood5  = colour red 0.3 green 0.1 blue 0.05
#declare VeryDarkWood5  = colour red 0.25 green 0.07 blue 0.038
#declare Wooden5 = texture {
   wood
   turbulence 0.05
   octaves 6
   scale <0.075 .075 1>
   colour_map {
      [0.00 0.10 color DarkWood5 color DarkWood5]
      [0.10 0.90 color DarkWood5 color VeryDarkWood5]
      [0.90 1.01  color VeryDarkWood5  color VeryDarkWood5 ]
   }
   ambient 0.2
   diffuse 0.8
   rotate <0 89 0>
}



camera {
   location <0.0  0.0  -10>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 0 0>
}

// Light source
object { light_source { <5 5 -30>  color White }}
object { light_source { <0 -10 -10>  color LightGray } no_shadow }


// Counter-clockwise, from top

object { AnObject
   texture {
      Wooden1
      specular 0.75
      roughness 0.008
      reflection 0.15
   }
}
object { AnObject
   texture {
      Wooden2
      specular 0.75
      roughness 0.008
      reflection 0.15
   }
   rotate <0 0 72>
}

object { AnObject
   texture {
      Wooden4
      specular 0.75
      roughness 0.008
      reflection 0.15
   }
   rotate <0 0 144>
}
object { AnObject
   texture {
      Wooden3
      specular 0.75
      roughness 0.008
      reflection 0.15
   }
   rotate <0 0 216>
}
object { AnObject
   texture {
      Wooden5
      specular 0.75
      roughness 0.008
      reflection 0.15
   }
   rotate <0 0 288>
}

// end of file
