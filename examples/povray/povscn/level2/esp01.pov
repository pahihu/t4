// Persistence Of Vision raytracer version 1.0 sample file.

// PolyWood.pov - Wooden polyhedron hollowed by a sphere
// on a grassy hilly lawn.  Shows how easy it is to
// create interesting shapes with CSG operations
// and simple primitive shapes.
// By Eduard [esp] Schwan, 6/23/92

include "shapes.inc"
include "colors.inc"
include "textures.inc"

// Moi
camera
{
  location <-3.5  1  -12.0>
  direction <0.0 0.0  1.0>
  up  <0.0  1.0  0.0>
  right <1.33 0.0 0.0>
  look_at <0 0 0>
}

// Some Light
object
{
  light_source { <20 10 -40> color White }
}

// The Cloudy Blue Sky
object
{
  sphere { <0 0 0> 10000  }
  texture { Bright_Blue_Sky  scale <4000 600 1000> }
}

// The Hilly Grassy Land
object
{
  plane { <0.0 1.0 0.0> -4.1  }
  texture
  {
    0.025  // a little randomness to hide the rather severe color banding
    color red 0.2 green 1.0 blue 0.4
    bumps 0.5
    frequency 100
    scale <10 10 10>
    ambient 0.1
    diffuse 0.7
    roughness 1
  }
}

// The Wooden Polyhedron
// The idea here is to take a few cubes rotated at 45 degrees
// along different axes from each other and intersect them,
// thus cutting some corners.  Then we carve out the center
// with a slightly oversized sphere, which protrudes slightly
// through the sides, cutting portholes in the facets of the
// polyhedron.  Then we get out the wooden paint..
object
{
  intersection
  {
    // polyhedron
    box { <-4 -4 -4> <4 4 4> }
    box { <-4 -4 -4> <4 4 4>  rotate <45  0  0> }
    box { <-4 -4 -4> <4 4 4>  rotate < 0 45  0> }
    // Add small amount to box to account for normal numerical innacurracies
    box { <-4.0001 -4.0001 -4.0001> <4.0001 4.0001 4.0001>  rotate < 0  0 45> }
    // hollow it out
    sphere { <0 0 0> 4.25 inverse }
  }
  // semi-dark wood with dark greenish rings
  texture
  {
    wood
    turbulence 0.04
    colour_map
    {
      [0.0 0.4  color red 0.8 green 0.4 blue 0.2
            color red 0.8 green 0.4 blue 0.1]
      [0.4 0.5  color red 0.1 green 0.3 blue 0.1
            color red 0.1 green 0.3 blue 0.2]
      [0.5 0.8  color red 0.1 green 0.3 blue 0.2
            color red 0.8 green 0.4 blue 0.1]
      [0.8 1.001  color red 0.8 green 0.4 blue 0.1
            color red 0.8 green 0.4 blue 0.2]
    }
    scale <0.2  0.2  1>
    rotate <45  0  5>
    translate <2  2  -4>
    ambient 0.15
    diffuse 0.6
  }
}
// ttfn!
