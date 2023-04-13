// Persistence Of Vision raytracer version 1.0 sample file.

// include file for poolball.pov
/* ***************** set up a bunch of declarations ***************************/

#declare Ball = sphere { < 0.0 0.0 0.0 > 1.0 }

#declare _1_Ball = object {
   sphere {  Ball}
   texture { Shiny color Yellow }
   color   Yellow
}

#declare _2_Ball = object {
   sphere {  Ball}
   texture { Shiny color Blue }
   color   Blue
}

#declare _3_Ball = object {
   sphere {  Ball}
   texture { Shiny color Red }
   color   Red
}

#declare _4_Ball = object {
   sphere {  Ball}
   texture { Shiny color Violet }
   color   Violet
}

#declare _5_Ball = object {
   sphere {  Ball}
   texture { Shiny color Orange }
   color   Orange
}

#declare _6_Ball = object {
   sphere {  Ball}
   texture { Shiny color ForestGreen }
   color   ForestGreen
}

#declare _7_Ball = object {
   sphere {  Ball}
   texture { Shiny color Maroon }
   color   Maroon
}

#declare _8_Ball = object {
   sphere {  Ball}
   texture { Shiny color Black }
   color   Black
}


// Striped balls start here


#declare Stripe = color Yellow
#declare _9_Ball = object {
   sphere { Ball }
   texture {
      Shiny
      gradient <0 1 0>
      color_map {
         [0.0 0.33 color White color White]
         [0.33 0.66 color Stripe color Stripe]
         [0.66 1.00 color White color White]
      }
      scale <2 2 2>
      translate <0 1 0>
   }
   color Stripe
}

#declare Stripe = color Blue
#declare _10_Ball =  object {
   sphere { Ball }
   texture {
      Shiny
      gradient <0 1 0>
      color_map {
         [0.0 0.33 color White color White]
         [0.33 0.66 color Stripe color Stripe]
         [0.66 1.00 color White color White]
      }
      scale <2 2 2>
      translate <0 1 0>
   }
   color Stripe
}

#declare Stripe = color Red
#declare _11_Ball = object {
   sphere { Ball }
   texture {
      Shiny
      gradient <0 1 0>
      color_map {
         [0.0 0.33 color White color White]
         [0.33 0.66 color Stripe color Stripe]
         [0.66 1.00 color White color White]
      }
      scale <2 2 2>
      translate <0 1 0>
   }
   color Stripe
}


#declare Stripe = color Violet
#declare _12_Ball = object {
   sphere { Ball }
   texture {
      Shiny
      gradient <0 1 0>
      color_map {
         [0.0 0.33 color White color White]
         [0.33 0.66 color Stripe color Stripe]
         [0.66 1.00 color White color White]
      }
      scale <2 2 2>
      translate <0 1 0>
   }
   color Stripe
}

#declare Stripe = color Orange
#declare _13_Ball = object {
   sphere { Ball }
   texture {
      Shiny
      gradient <0 1 0>
      color_map {
         [0.0 0.33 color White color White]
         [0.33 0.66 color Stripe color Stripe]
         [0.66 1.00 color White color White]
      }
      scale <2 2 2>
      translate <0 1 0>
   }
   color Stripe
}

#declare Stripe = color ForestGreen
#declare _14_Ball = object {
   sphere { Ball }
   texture {
      Shiny
      gradient <0 1 0>
      color_map {
         [0.0 0.33 color White color White]
         [0.33 0.66 color Stripe color Stripe]
         [0.66 1.00 color White color White]
      }
      scale <2 2 2>
      translate <0 1 0>
   }
   color Stripe
}

#declare Stripe = color Maroon
#declare _15_Ball = object {
   sphere { Ball }
   texture {
      Shiny
      gradient <0 1 0>
      color_map {
         [0.0 0.33 color White color White]
         [0.33 0.66 color Stripe color Stripe]
         [0.66 1.00 color White color White]
      }
      scale <2 2 2>
      translate <0 1 0>
   }
   color Stripe
}

#declare CueStick = object {
   intersection {
      quadric { Cylinder_Z  }
      plane { <0  0  1>  1.0 }
   }
   texture {
      Dark_Wood
      scale <0.01 0.01 1>
      ambient 0.1
      diffuse 0.7
      phong 1.0
      phong_size 90
      reflection 0.3
   }
}        
