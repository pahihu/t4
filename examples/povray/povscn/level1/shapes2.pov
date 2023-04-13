// Persistence Of Vision raytracer version 1.0 sample file.
// By Chris Young
// This image contains an example of every shape from SHAPES2.INC

#include "colors.inc"
#include "shapes.inc"
#include "shapes2.inc"

camera 
{location <10 10 -20>
   direction <0 0 1.5>
   up  <0 1 0>
   right <1.33333 0 0>
   look_at <2 0 0>
}

object {light_source {<0 1000 -1000> color LightGray}}

object {light_source {<150 100 -100> color LightGray}}

#declare Col1 =-9
#declare Col2 =-3
#declare Col3 =3
#declare Col4 =8

#declare Row1 =6
#declare Row2 =0
#declare Row3 =-6

object
{intersection{Tetrahedron}
   texture{color Red}
   bounded_by{sphere{<0 0 0> 3}}
   translate <Col1 2 Row1>
}

object
{intersection{Octahedron}
   texture{color Green}
   bounded_by{sphere{<0 0 0> 1.8}}
   translate <Col2 1.8 Row1>
}

object
{intersection{Dodecahedron}
   texture{color Blue}
   bounded_by{sphere{<0 0 0> 1.3}}
   translate <Col3 1.3 Row1>
}

object
{intersection{Icosahedron}
   texture{color Magenta}
   bounded_by{sphere{<0 0 0> 1.3}}
   translate <Col4 1.3 Row1>
}

object
{intersection{HalfCone_Y}
   texture{color Yellow}
   bounded_by{sphere{<0 0 0> 1.5}}
   translate <Col1 1.5 Row2>
}

object
{intersection{Hexagon}
   texture{color Cyan}
   bounded_by{sphere{<0 0 0> 1.5}}
   translate <Col2 1.5 Row2>
}

object
{intersection{Rhomboid}
   texture{color Tan}
   bounded_by{sphere{<0 0 0> 2.3}}
   translate <Col3 1.5 Row2>
}

object
{intersection{Pyramid}
   texture{color Orange}
   bounded_by{sphere{<0 0 0> 5}}
   translate <Col4 1.5 Row2>
}

object
{union{Square_X}
   texture{color NeonPink}
   translate <Col2 1 Row3>
}

object
{union{Square_Y}
   texture{color Scarlet}
   translate <Col3 1 Row3>
}

object
{union{Square_Z}
   texture{color NeonBlue}
   translate <Col4 1 Row3>
}

object{plane{<0 1 0> 0}texture{color White}}
