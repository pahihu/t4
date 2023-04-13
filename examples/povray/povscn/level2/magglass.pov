// Persistence of Vision Raytracer
// Magnifying glass created using POV-Ray's refraction.
// A convex lens created with CSG 
// (and something to view through it)
// This example doesn't work very well, but it gives a good 
// starting point for a better use of the magnifying glass.


#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"
#include "fov.inc"                  // Field of view constants
#include "ior.inc"                  // Index of refraction constants



camera {
   location <0.0  2  -10>
   direction <0.0 0.0  FoV_60>     // 60o field of view
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 0 0>
}


object { light_source  { <30 50 -50>  color White  } }
object { light_source  { <-30 10 20>  color Gray50  } }

fog { color Gray50 200}         // This fog reaches max density at 200z

// Background sphere
object {
    sphere { <0 0 0> 1 }
    texture {
        0.015
        gradient <0 1 0>
        color_map {
            [0.0 1.0 color Gray80 color Gray30]
        }
    }
    scale <10000 10000 10000>
}

// A lens.  This uses the Ellipsoid quadric to make it independantly
// scalable, but it would be faster to use spheres.
// It is designed "sideways" so you can see the thickness.
// It is then rotated 90o on Y so the viewer is looking through the lens.
#declare Lens_Thickness = 0.35
#declare Lens_Diameter = 1.5
#declare Lens = object {
    intersection {
        quadric { Ellipsoid scale <1.5 1.5 1.5> translate < 0.75 0 0> }
        quadric { Ellipsoid scale <1.5 1.5 1.5> translate <-0.75 0 0> }
    }
    texture {
        Glass2                           // Try Glass and Glass3, too
        ior Flint_Glass_Ior              // Over-ride the Glass IOR value
        reflection 0                     // Over-ride reflection, too
    }
    scale <Lens_Thickness Lens_Diameter Lens_Diameter >
}


object { plane { <0 1 0> -4 }
    texture {
        checker
            color HuntersGreen
            color SummerSky
        scale <3 1 3>
        ambient 0.2
        diffuse 0.6
    }
}

object { Lens rotate <0 80 0> }

// A sphere in the distance
object { sphere { <3 1 30> 2 } texture { Phong_Shiny color Orange }}

object { quadric { Cylinder_X }
    rotate <0 -75 0>
    translate <0 -3 25>
    texture {
        Phong_Shiny
        granite
        scale <2 2 2>
        ambient 0.25
        diffuse 0.6
    }
}
