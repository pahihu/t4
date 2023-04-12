// Persistence Of Vision raytracer version 1.0 sample file.
// written by Alexander Enzmann
// Demonstrates blob primitive

camera {
   location  <0 0 -5>
   direction <0 0 1.2071>
   up        <0 1 0>
   right     <1 0 0>
   look_at   <0 0 0>
}

object {
   light_source {
      <-15 30 -25>
      color red 1 green 1 blue 1
   }
}
object {
   light_source {
      < 15 30 -25>
      color red 1 green 1 blue 1
   }
}


object {
   blob {
      threshold 0.6
      component 1.0 1.0 <0.75 0 0>
      component 1.0 1.0 <-0.375  0.64952 0>
      component 1.0 1.0 <-0.375 -0.64952 0>
   }
   texture {
      ambient 0.2
      diffuse 0.8
      color red 1 green 0 blue 0
      phong 1
   }
   rotate <0 30 0>
}
