// Persistence Of Vision raytracer version 1.0 sample file.

// This data declares Hardwood, a Floor & walls w/base moulding

//Light tan to d.brown

#declare br01 = colour red 1.00 green 0.94 blue 0.71
#declare br02 = colour red 1.00 green 0.88 blue 0.65
#declare br03 = colour red 0.94 green 0.80 blue 0.59
#declare br03 = colour red 0.94 green 0.80 blue 0.59
#declare br04 = colour red 0.80 green 0.69 blue 0.52
#declare br05 = colour red 0.72 green 0.63 blue 0.46
#declare br06 = colour red 0.78 green 0.63 blue 0.46
#declare br07 = colour red 0.74 green 0.59 blue 0.49
#declare br08 = colour red 0.66 green 0.51 blue 0.41
#declare br09 = colour red 0.61 green 0.50 blue 0.42
#declare br10 = colour red 0.58 green 0.45 blue 0.32
#declare br11 = colour red 0.65 green 0.45 blue 0.32
#declare br12 = colour red 0.65 green 0.41 blue 0.28
#declare br13 = colour red 0.58 green 0.41 blue 0.28
#declare br14 = colour red 0.57 green 0.37 blue 0.24
#declare br15 = colour red 0.60 green 0.37 blue 0.24
#declare br16 = colour red 0.54 green 0.35 blue 0.25
#declare br17 = colour red 0.56 green 0.31 blue 0.21
#declare br18 = colour red 0.45 green 0.31 blue 0.16
#declare br19 = colour red 0.45 green 0.31 blue 0.21
#declare br20 = colour red 0.41 green 0.27 blue 0.17
#declare br21 = colour red 0.40 green 0.24 blue 0.14
#declare br22 = colour red 0.37 green 0.24 blue 0.11
#declare br23 = colour red 0.37 green 0.24 blue 0.18
#declare br24 = colour red 0.34 green 0.21 blue 0.15
#declare br25 = colour red 0.40 green 0.21 blue 0.15
#declare br26 = colour red 0.32 green 0.19 blue 0.07
#declare br27 = colour red 0.30 green 0.14 blue 0.02
#declare br28 = colour red 0.30 green 0.14 blue 0.10
#declare br29 = colour red 0.24 green 0.16 blue 0.12
#declare br30 = colour red 0.25 green 0.13 blue 0.09
#declare br31 = colour red 0.21 green 0.09 blue 0.05


#declare pop_wood = texture {
   marble
   color_map {
      [0.0 0.2 color br16 color br16]	
      [0.2 0.4 color br16 color br14]
      [0.4 0.6 color br14 color br17]
      [0.6 0.8 color br17 color br17]
      [0.8 0.9 color br17 color br14]
      [0.9 1.01 color br14 color br16]
   }

   turbulence 0.4
   scale <13 13 1> 
   ambient 0.8 phong 0.5 
}

texture {
   wood
   color_map {
      [0.0 0.05 color br25 color br25 alpha 0.5]
      [0.05 0.2 color br25 alpha 0.5 color br21 alpha 1.0]
      [0.2 0.7 color br21 alpha 1.0 color br21 alpha 0.8]
      [0.7 0.8 color br27 alpha 0.7 color br27 alpha 1.0]
      [0.8 0.9 color br27 alpha 1.0 color br27 alpha 0.7]
      [0.9 1.01 color br27 alpha 1.0 color br30 alpha 0.5]
   }
   turbulence 0.5
   ambient 0.9
   phong 1.0
   scale <5 15 1>
}

/*************************************************************/

#declare zeb_wood = texture {
   marble
   color_map {
      [0.0 0.2 color br16 color br16]	
      [0.2 0.4 color br16 color br14]
      [0.4 0.6 color br14 color br17]
      [0.6 0.8 color br17 color br17]
      [0.8 0.9 color br17 color br14]
      [0.9 1.01 color br14 color br16]
   }
   ambient 0.3
   diffuse 1.2
   turbulence 0.25
   scale <13 13 1>
}

texture {
   wood
   color_map {
      [0.0 0.05 color br25 color br25 alpha 0.7]
      [0.05 0.2 color br25 alpha 0.7 color br21 alpha 1.0]
      [0.2 0.7 color br21 alpha 1.0 color br21 alpha 0.8]
      [0.7 0.8 color br27 alpha 0.7 color br27 alpha 1.0]
      [0.8 0.9 color br27 alpha 1.0 color br27 alpha 0.7]
      [0.9 1.01 color br27 alpha 1.0 color br30 alpha 0.7]
   }
   turbulence 0.3
   ambient 0.6
   diffuse 0.9
   scale <5 15 5>
}

texture {
   wood
   color_map {
      [0.0 0.05 color br25 color br25 alpha 0.4]
      [0.05 0.2 color br25 alpha 0.4 color br21 alpha 0.8]
      [0.2 0.7 color br21 alpha 0.8 color br21 alpha 0.9]
      [0.7 0.8 color br27 alpha 0.8 color br27 alpha 0.6]
      [0.8 0.9 color br27 alpha 0.6 color br27 alpha 0.4]
      [0.9 1.01 color br27 alpha 0.5 color br30 alpha 0.8]
   }
   diffuse 0.9
   ambient 0.3
   turbulence 0.2
   scale <4 20 4>
}


/*------------------ MAKE HARDWOOD FLOOR ----------------------------------*/

#declare wt = texture {
   zeb_wood 0.01 scale <10 20 20> phong 0.5 phong_size 20 }

#declare plank = object {
   intersection { Cube scale <100 700 4> }
   texture { wt }
}

#declare the_floor = composite {
   object { plank texture { wt translate <0 100 0> } translate <0 0 0> }
   object { plank texture { wt translate <0 0 0> } translate <204 0 0> }
   object { plank texture { wt translate <0 50 0> rotate <0 0 -20> } translate <408 0 0> }
   object { plank texture { wt translate <0 -100 0> } translate <612 0 0> }
   object { plank texture { wt translate <0 -30 0> } translate <-204 0 0> }
   object { plank texture { wt translate <0 100 0> } translate <-408 0 0> }
   object { plank texture { wt translate <0 -50 0> rotate <0 0 20> } translate <-612 0 0> }
   object { intersection { Cube scale <500 500 1> }
      texture { color Black } translate <0 -4 20> }
   bounded_by {
      intersection { Cube scale <720 500 5> }
   }
}




/*--------------------START THE WALLS-------------------------*/

#declare cwall = texture {
   color DarkSlateGray 0.05 }

/*----------------  Build BaseBoards  ------------------------*/

#declare baseboard = object {
   difference {
      union {
         intersection { Cube scale <300 50 120> translate <0 50 0> }
         intersection { Cube scale <290 25 110> translate <0 125 0> }
         quadric { Ellipsoid scale <10 10 10> translate <290 110 -110> }
         quadric { Ellipsoid scale <10 10 10> translate <290 110 110> }
         intersection { Disk_X scale <580 10 10> translate <-290 110 -110> }
         intersection { Disk_Z scale <10 10 220> translate <290 110 -110> }
      }
      quadric { Cylinder_X scale <1 20 20> translate <0 150 -110> }
      quadric { Cylinder_Z scale <20 20 1> translate <290 150 0> }
   }
   texture { 
      color Gray ambient 0.2 diffuse 0.7 phong 1.0  
   }
   bounded_by { 
      intersection { Cube scale <305 80 125> translate <0 77 0> }
   }
}

/*--------------- THE PLASTER WALL ------------------------------------*/
#declare plaster = object {
   intersection { Cube scale <260 250 80> }
   texture { cwall }
}

/*-------------- WALL WITH BASE ATTACHED ------------------------------*/
#declare wall = composite {
   object { plaster translate <0 400 0> }
   object { baseboard }
}
