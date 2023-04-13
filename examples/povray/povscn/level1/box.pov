// Persistence Of Vision raytracer version 1.0 sample file.
// written by Alexander Enzmann
// Demonstrates box primitive

camera {
   location  <0 0 -8>
   direction <0 0 1.2071>
   up        <0 1 0>
   right     <1 0 0>
   look_at   <0 0 0>
}

object {
   sphere { <0.0 0.0 0.0> 2 }
   texture {
      ambient 0.2
      diffuse 0.8
      color red 1 green 0 blue 0
      phong 1
   }
}

object {
   box { <-2.0 -0.2 -2.0> <2.0 0.2 2.0> }
   texture {
      ambient 0.2
      diffuse 0.8
      color red 1 green 0 blue 1
   }
   rotate <-20 30 0>
}

object {
   light_source {
      <-10 3 -20>
      color red 1 green 1 blue 1
   }
}
