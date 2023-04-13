// Persistence Of Vision raytracer version 1.0 sample file.
// By Chris Young
// This image contains an example of every shape from SHAPES.INC

#include "colors.inc"
#include "shapes.inc"

camera 
{location <20 10 -30>
   direction <0 0  1>
   up  <0 1 0>
   right <1.33333 0 0>
   look_at <0 5 0>
}

object {light_source {<200 50 -100> color LightGray}}

object {light_source {<50 100 -200> color LightGray}}

object {light_source {<200 200 -200> color LightGray}}

object
{  
   quadric {Ellipsoid}
   texture {color Red}
   translate <17 1 -13>
}

object
{quadric {Cylinder_X}
   texture {color Green}
   translate <0 6 -2>
}

object
{quadric {Cylinder_Y}
   texture {color Blue}
   translate <18 0 -8>
}

object
{quadric {Cylinder_Z}
   texture {color Cyan}
   translate <10 3 0>
}

object
{quadric { QCone_X}
   texture {color Orange}
   scale <9 1 1>
   translate <13 10 -3>
}

object
{quadric { QCone_Y}
   texture {color Yellow}
   scale <1 9 1>
   translate <7 10 -18>
}

object
{quadric { QCone_Z}
   texture {color Maroon}
   scale <1 1 9>
   translate <7 15 -10>
}

object
{quadric {Plane_YZ}
   texture {color Navy}
}

object
{quadric {Plane_XZ}
   texture {color White}
}

object
{quadric {Plane_XY}
   texture {color SkyBlue}
}

object
{quadric {Paraboloid_X}
   texture {color Magenta}
   translate <15 15 -9>
}

object
{quadric {Paraboloid_Y}
   texture {color GreenYellow}
   translate <5 11 -23>
}

object
{quadric {Paraboloid_Z}
   texture {color OrangeRed}
   translate <10 18 -6>
}

object
{quadric {Hyperboloid}
   texture {color Tan}
   scale <7 1 1>
   translate <10 2 -5>
}

object
{quadric {Hyperboloid_Y}
   texture {color NeonPink}
   scale <1 6 1>
   translate <3 10 -10>
}

object
{box {UnitBox}
   texture {color Scarlet}
   translate <14 1 -13>
}

object
{intersection {Cube}
   texture {color ForestGreen}
   translate <12 1 -16>
}

object
{intersection {Disk_X}
   texture {color Coral}
   translate <10 5 -24>
}

object
{intersection {Disk_Y}
   texture {color SeaGreen}
   translate <10 7 -24>
}

object
{intersection {Disk_Z}
   texture {color Brass}
   translate <10 9 -24>
}
