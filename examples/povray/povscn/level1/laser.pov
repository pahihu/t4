// Persistence of Vision Raytracer Version 1.0
//A couple of tricks with spotlights and wood texture here.  Read the
//comments.  By Dan Farmer.


#include "colors.inc"
#include "shapes.inc"

camera {
   location  <-8 3 -14>
   direction <0 0 1>
   up        <0 1 0>
   right     <1.3333 0 0>
   look_at   <0 0 0>
}

// Overhead spotlight, shining "backwards"
object { light_source { <0 50 -1>  color LightGray
      spotlight
      point_at <0 0 8>
      tightness 50
      radius 50
      falloff 100
   }
}

// Ground plane
object {
   plane { <0.0 1.0 0.0> -1.0  }
   texture {
      color White
      ambient 0.3
      diffuse 0.7
      specular 0.5  roughness 0.05
   }
}

// Three spotlights positioned in front of three cylinders.  These could
// be put into composites if you wanted to really do it right.  Each light
// is associated with a cylinder.
//----------
// Red spotlight, goes with  left cylinder
object { light_source { <-3 -0.5 -2>  color Red
      spotlight
      point_at <-3 -1 -10>
      tightness 10
      radius 100
      falloff 250
   }
}

// Green spotlight, goes with center cylinder
object { light_source { <0 -0.5 -2>   color Green
      spotlight
      point_at <0 -1 -10>
      tightness 10
      radius 100
      falloff 250
   }
}

// Blue spotlight, goes with right cylinder
object { light_source { <3 -0.5 -2>   color Blue
      spotlight
      point_at <3 -1 -10>
      tightness 10
      radius 100
      falloff 250
   }
}

// Set default textures for shapes to come
default {
   texture {
      ambient 0.5     // Unusually high ambient setting.
      diffuse 0.5     // Unusually low diffuse setting.
      refraction 0.75 // Don't really need an IOR value this time.
      reflection 0.15
      specular 0.25 roughness 0.001
   }
}

// Red cylinder on the left.  Goes with red spotlight.
object {
   intersection { Disk_Z  }
   texture {     // These texture components are unique to this shape
      wood
      turbulence 0  // I want concentric rings,  not wood.
      octaves 0     // Irrelevant with turbulence 0.
      // colormap from opaque red to "clear red"
      color_map { [0.0 1.0  color Red alpha 0 color Red alpha 1] }
      scale <2 2 1>
   }
   scale <1 1 6>        // Scale texture with the object now.
   translate <-3 0 4>  // Move it to its final restingplace
}

// Green cylinder in the center.  Goes with green spotlight.
object {
   intersection { Disk_Z  }
   texture {
      wood
      turbulence 0
      octaves 0
      color_map { [0.0 1.0  color Green alpha 0 color Green alpha 1] }
      scale <2 2 1>
   }
   scale <1 1 6>
   translate <0 0 4>
}

// Blue cylinder on the right.  Goes with blue spotlight, right?
object {
   intersection { Disk_Z  }
   texture {
      wood
      turbulence 0
      octaves 0
      color_map { [0.0 1.0  color Blue alpha 0 color Blue alpha 1] }
      scale <2 2 1>
   }
   scale <1 1 6>
   translate <3 0 4>
}
