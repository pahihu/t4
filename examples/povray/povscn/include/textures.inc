// Persistence of Vision Raytracer
// Standard textures include file
// See textures.doc for info.

//*****************************************************************************
//                      STONE TEXTURES
//*****************************************************************************

// A nice Jade.  Color map works nicely with other textures, too.
#declare Jade = texture {
   marble
   turbulence 1.8
   colour_map {
      [0.0 0.8   colour red 0.1 green 0.6 blue 0.1
      colour red 0.0 green 0.3 blue 0.0]
      [0.8 1 colour red 0.1 green 0.6 blue 0.1
      colour red 0.0 green 0.2 blue 0.0]
   }
}

// Classic white marble with red veins.  Over-worked, like checkers.
#declare Red_Marble = texture {
   marble
   turbulence 1.0
   colour_map {
      [0.0 0.8   colour red 0.8 green 0.8 blue 0.6
      colour red 0.8 green 0.4 blue 0.4]
      [0.8 1 colour red 0.8 green 0.4 blue 0.4
      colour red 0.8 green 0.2 blue 0.2]
   }
}

// White marble with black veins.
#declare White_Marble = texture {
   marble
   turbulence 1.0
   colour_map {
      [0.0 0.8  colour red 0.9 green 0.9 blue 0.9
      colour red 0.5 green 0.5 blue 0.5]
      [0.8 1 colour red 0.5 green 0.5 blue 0.5
      colour red 0.2 green 0.2 blue 0.2]
   }
}

// Light blue and black marble with a thin red vein 
// Try changing LBlue and Vein below to modify the marble.
#declare LBlue = colour red 0.0 green 0.6 blue 0.6
#declare Vein = colour red 0.6 green 0.0 blue 0.0
#declare Blood_Marble = texture {
   marble
   turbulence 2.3
   colour_map {
      [0.0 0.8   colour Black  colour LBlue]
      [0.8 0.9   colour LBlue  colour Vein]
      [0.9 1 colour Vein   colour Black]
   }
}

// a grey blue agate -- kind of purplish.
#declare Blue_Agate = texture {
   agate
   colour_map {
      [0.0 0.5   colour red 0.30 green 0.30 blue 0.50
      colour red 0.30 green 0.30 blue 0.50]
      [0.5 0.55  colour red 0.30 green 0.30 blue 0.50
      colour red 0.20 green 0.20 blue 0.30]
      [0.55 0.6  colour red 0.20 green 0.20 blue 0.30
      colour red 0.25 green 0.25 blue 0.35]
      [0.6 0.7   colour red 0.25 green 0.25 blue 0.35
      colour red 0.15 green 0.15 blue 0.26]
      [0.7 0.8   colour red 0.15 green 0.15 blue 0.26
      colour red 0.10 green 0.10 blue 0.20]
      [0.8 0.9   colour red 0.10 green 0.10 blue 0.20
      colour red 0.30 green 0.30 blue 0.50]
      [0.9 1 colour red 0.30 green 0.30 blue 0.50
      colour red 0.10 green 0.10 blue 0.20]
   }
}

// Deep blue agate -- almost glows.
#declare Sapphire_Agate = texture {
   agate
   colour_map {
      [0.0 0.3   colour red 0.0 green 0.0 blue 0.9
      colour red 0.0 green 0.0 blue 0.8]
      [0.3 1 colour red 0.0 green 0.0 blue 0.8
      colour red 0.0 green 0.0 blue 0.4]
   }
}

// Brown and white agate -- very pretty.
#declare Brown_Agate = texture {
   agate
   colour_map {
      [0.0 0.5   colour red 1.0 green 1.0 blue 1.0
      colour red 0.9 green 0.7 blue 0.6]
      [0.5 0.6   colour red 0.9 green 0.7 blue 0.6
      colour red 0.9 green 0.7 blue 0.4]
      [0.6 1 colour red 0.9 green 0.7 blue 0.4
      colour red 0.7 green 0.4 blue 0.2]
   }
}


#declare Pink_Granite =  texture {
   granite
   turbulence 0
   color_map {
      [0.0 0.4 color Black color Black]
      [0.4 0.45 color Quartz color Quartz]
      [0.45 0.5 color Quartz color Gray]
      [0.5 0.55 color Gray color Feldspar]
      [0.55 0.8 color Feldspar color Feldspar]
      [0.8 1.0 color Feldspar color Orange]
   }
}
// Gray-pink alabaster or marble.  Layers are scaled for a unit object
// and relative to each other.
#declare PinkAlabaster =
// Underlying surface is very subtly mottled with bozo
texture {
   ambient 0.25
   bozo
   turbulence 0.25
   color_map {
      [0.0 1.0 color red 0.9 green 0.75 blue 0.75
      color red 0.6 green 0.6 blue 0.6 ]
   }
   scale <.4 .4 .4>
}

// Second layer texture has some alpha values, yet a fair amount of color
// Viening is kept quite thin in color map and by the largish scale.
texture {
   granite
   color_map {
      [0.0 0.9 color DustyRose alpha 1 color DustyRose alpha 0.5 ]
      [0.9 1.0  color DarkSlateGray color DarkSlateGray ]
   }
   // I know this seem contradictory, but it seems to work!
   specular 1.0   roughness 0.001
   phong 0.25     phong_size 75
   brilliance 4
   scale <2 2 2>       // Twice as large as unit shape
}


//*****************************************************************************
//                           SKY TEXTURES
//*****************************************************************************

// Basic Blue Sky w/ clouds.
#declare Blue_Sky = texture {
   bozo
   turbulence 0.3
   colour_map {
      [0.0 0.5   colour red 0.25 green 0.25 blue 0.5
      colour red 0.25 green 0.25 blue 0.5]
      [0.5 0.6   colour red 0.25 green 0.25 blue 0.5
      colour red 0.7 green 0.7 blue 0.7]
      [0.6 1 colour red 0.7 green 0.7 blue 0.7
      colour red 0.3 green 0.3 blue 0.3]
   }
}

// Bright Blue Sky w/ very white clouds.
#declare Bright_Blue_Sky = texture {
   bozo
   turbulence 0.56
   colour_map {
      [0.0 0.5  colour red 0.5 green 0.5 blue 1.0
      colour red 0.5 green 0.5 blue 1.0]
      [0.5 0.6  colour red 0.5 green 0.5 blue 1.0
      colour red 1.0 green 1.0 blue 1.0]
      [0.6 1 colour red 1.0 green 1.0 blue 1.0
      colour red 0.5 green 0.5 blue 0.5]
   }
}

// Another sky
#declare Blue_Sky2 = 
texture {
   agate
   color_map {
      [ 0 .3  color Blue color Blue  ]
      [.3  1  color Blue   color White   ]
   }
   scale <.75 .15 .75>
}

// Small puffs of white clouds
#declare Blue_Sky3 = 
texture {
   granite
   turbulence 0.1
   color_map {
      [ 0 .3  color Blue color Blue  ]
      [.3  1  color Blue   color White   ]
   }
   scale <.75 .15 .75>
}


// Red sky w/ yellow clouds -- very surreal.
#declare Blood_Sky = texture {
   bozo
   turbulence 0.5
   colour_map {
      [0.0 0.5  colour red 0.9 green 0.7 blue 0.0
      colour red 0.3 green 0.2 blue 0.0]
      [0.5 0.6  colour red 0.6 green 0.025 blue 0.0
      colour red 0.9 green 0.7 blue 0.0]
      [0.6 1 colour red 0.6 green 0.025 blue 0.0
      colour red 0.6 green 0.025 blue 0.0]
   }
}

// Black sky with red and purple clouds 
// Try adding turbulence values from 0.1 - 5.0 -- CdW
#declare Apocalypse = texture {
   bozo
   colour_map {
      [0.0 0.4  colour red 0.8 green 0.0 blue 0.0
      colour red 0.4 green 0.0 blue 0.4]
      [0.4 0.6  colour red 0.4 green 0.0 blue 0.4
      colour red 0.0 green 0.0 blue 0.2]
      [0.6 1 colour red 0.0 green 0.0 blue 0.2
      colour red 0.0 green 0.0 blue 0.0]
   }
}

// White clouds w/ transparent sky.
#declare Clouds = texture {
   bozo
   turbulence 0.0
   colour_map {
      [0.0 0.1   colour red 0.8 green 0.8 blue 0.8
      colour red 0.8 green 0.8 blue 0.8]
      [0.1 0.5   colour red 0.8 green 0.8 blue 0.8 alpha 0.0
      colour red 1.0 green 1.0 blue 1.0 alpha 1.0 ]
      [0.5 1 colour red 1.0 green 1.0 blue 1.0 alpha 1.0
      colour red 1.0 green 1.0 blue 1.0 alpha 1.0]
   }
}

//*****************************************************************************
//                           WOODEN TEXTURES
//*****************************************************************************

// A light reddish wood.
#declare Cherry_Wood = texture {
   wood
   turbulence 0.3
   colour_map {
      [0.0 0.8  colour red 0.666 green 0.312 blue 0.2
      colour red 0.666 green 0.312 blue 0.2]
      [0.8 1 colour red 0.4 green 0.133 blue 0.066
      colour red 0.2 green 0.065 blue 0.033]
   }
}

// A light tan wood with greenish rings.
#declare Pine_Wood = texture {
   wood
   turbulence 0.2
   colour_map {
      [0.0 0.8  colour red 1.0 green 0.71875 blue 0.25
      colour red 1.0 green 0.71875 blue 0.25]
      [0.8 1 colour red 0.5 green 0.5 blue 0.066
      colour red 0.4 green 0.4 blue 0.033]
   }
}

// Dark wood with a greenish hue to it.
#declare Dark_Wood = texture {
   wood
   turbulence 0.2
   colour_map {
      [0.0 0.8  colour red  0.42857 green 0.23810 blue 0.04762
      colour red  0.42857 green 0.23810 blue 0.04762]
      [0.8 1 colour red 0.4 green 0.333 blue 0.066
      colour red 0.2 green 0.033 blue 0.033]
   }
}

// Light tan wood with brown rings.
#declare Tan_Wood = texture {
   wood
   turbulence 0.1
   colour_map {
      [0.0 0.8  colour red 0.888 green 0.600 blue 0.3
      colour red 0.888 green 0.600 blue 0.3]
      [0.8 1 colour red 0.6 green 0.4 blue 0.2
      colour red 0.4 green 0.3 blue 0.2]
   }
}

// A very pale wood with tan rings -- kind of balsa-ish.
#declare White_Wood = texture {
   wood
   turbulence 0.6
   colour_map {
      [0.0 0.8  colour red 0.93 green 0.71 blue 0.532
      colour red 0.98 green 0.81 blue 0.6]
      [0.8 1 colour red 0.6 green 0.333 blue 0.266
      colour red 0.7 green 0.6 blue 0.23]
   }
}

// Brown wood - looks stained.
#declare Tom_Wood = texture {
   wood
   turbulence 0.31
   colour_map {
      [0.0 0.8   colour red 0.7 green 0.3 blue 0.0
      colour red 0.7 green 0.3 blue 0.0]
      [0.8 1  colour red 0.5 green 0.2 blue 0.0
      colour red 0.4 green 0.1 blue 0.0]
   }
}

// The scaling in these definitions is relative to a unit-sized object
// (radius 1).  Note that woods are functionally equivilent to a log lying
// along the z axis.  For best results, think like a woodcutter trying to
// extract the nicest board out of that log.  A little tilt along the x axis
// will give elliptical rings of grain like you'd expect to find on most
// boards.  Experiment.
// (The first five came from DODEC2.POV in the POV-Ray Scenefile Library.)
#declare DMFWood1 = texture {
   wood
   turbulence 0.04
   octaves 3
   scale <0.05 .05 1>
   colour_map {
      [0.00 0.10 color red 0.60 green 0.30 blue 0.18
      color red 0.60 green 0.30 blue 0.18]
      [0.10 0.90 color red 0.60 green 0.30 blue 0.18
      color red 0.30 green 0.15 blue 0.09]
      [0.90 1.0  color red 0.30 green 0.15 blue 0.09
      color red 0.30 green 0.15 blue 0.09]
   }
}

#declare DMFWood2 = texture {
   wood
   turbulence 0.03
   octaves 4
   scale <0.05 .05 1>
   colour_map {
      [0.00 0.10 color red 0.52 green 0.37 blue 0.26
      color red 0.52 green 0.37 blue 0.26]
      [0.10 0.90 color red 0.52 green 0.37 blue 0.26
      color red 0.42 green 0.26 blue 0.15]
      [0.90 1.0  color red 0.42 green 0.26 blue 0.15
      color red 0.42 green 0.26 blue 0.15]
   }
}

#declare DMFWood3 = texture {
   wood
   turbulence 0.05
   octaves 2
   scale <0.05 .05 1>
   colour_map {
      [0.00 0.10 color red 0.4 green 0.133 blue 0.066
      color red 0.4 green 0.133 blue 0.066]
      [0.10 0.90 color red 0.4 green 0.133 blue 0.066
      color red 0.2 green 0.065 blue 0.033]
      [0.90 1.0  color red 0.2 green 0.065 blue 0.033
      color red 0.2 green 0.065 blue 0.033]
   }
}

#declare DMFWood4 = texture {
   wood
   turbulence 0.04
   octaves 3
   scale <0.05 .05 1>
   colour_map {
      [0.00 0.10 color red 0.888 green 0.600 blue 0.3
      color red 0.888 green 0.600 blue 0.3]
      [0.10 0.90 color red 0.888 green 0.600 blue 0.3
      color red 0.6 green 0.4 blue 0.2]
      [0.90 1.0  color red 0.6 green 0.4 blue 0.2
      color red 0.6 green 0.4 blue 0.2]
   }
}

#declare DMFWood5 = texture {
   wood
   turbulence 0.05
   octaves 6
   scale <0.075 .075 1>
   colour_map {
      [0.00 0.10 color red 0.3 green 0.1 blue 0.05
      color red 0.3 green 0.1 blue 0.05]
      [0.10 0.90 color red 0.3 green 0.1 blue 0.05
      color red 0.25 green 0.07 blue 0.038]
      [0.90 1.0  color red 0.25 green 0.07 blue 0.038
      color red 0.25 green 0.07 blue 0.038]
   }
}

// Is this really oak?  I dunno.  Quite light, maybe more like spruce.
#declare DMFLightOak =  texture {
   wood
   turbulence 0.05            // For best results,  keep this low!
   scale <0.2 0.2 1>          // Scaled or a unit object
   colour_map {
      [0.00 0.10 color red 0.42 green 0.26 blue 0.15
      color red 0.42 green 0.26 blue 0.15]
      [0.10 0.90 color red 0.42 green 0.26 blue 0.15
      color red 0.52 green 0.37 blue 0.26 ]
      [0.90 1 color red 0.52 green 0.37 blue 0.26
      color red 0.52 green 0.37 blue 0.26  ]
   }
}

// Looks like old desk oak if used correctly.
#declare DMFDarkOak = texture {
   wood
   turbulence 0.04            // For best results,  keep this low!
   octaves 3
   scale <0.2 0.2 1>          // Scaled or a unit object
   colour_map {
      [0.00 0.10 color red 0.60 green 0.30 blue 0.18
      color red 0.60 green 0.30 blue 0.18 ]
      [0.10 0.90 color red 0.60 green 0.30 blue 0.18
      color red 0.30 green 0.15 blue 0.09 ]
      [0.90 1 color red 0.30 green 0.15 blue 0.09
      color red 0.30 green 0.15 blue 0.09  ]
   }
}

//   Doug Otwell woods
//   Yellow pine, close grained
//
#declare Yellow_Pine = texture {
   wood
   turbulence 0.02
   colour_map {
      [0.000 0.222   colour red  0.808  green  0.671  blue  0.251  alpha  0.000
      colour red  0.808  green  0.671  blue  0.251  alpha  0.000]
      [0.222 0.342   colour red  0.808  green  0.671  blue  0.251  alpha  0.000
      colour red  0.600  green  0.349  blue  0.043  alpha  0.000]
      [0.342 0.393   colour red  0.600  green  0.349  blue  0.043  alpha  0.000
      colour red  0.808  green  0.671  blue  0.251  alpha  0.000]
      [0.393 0.709   colour red  0.808  green  0.671  blue  0.251  alpha  0.000
      colour red  0.808  green  0.671  blue  0.251  alpha  0.000]
      [0.709 0.821   colour red  0.808  green  0.671  blue  0.251  alpha  0.000
      colour red  0.533  green  0.298  blue  0.027  alpha  0.000]
      [0.821 1   colour red  0.533  green  0.298  blue  0.027  alpha  0.000
      colour red  0.808  green  0.671  blue  0.251  alpha  0.000]
   }
   scale <0.1 0.1 0.1>
   translate <10 0 0>

}
// Yellow_Pine layer 2
texture {
   wood
   turbulence 0.01
   colour_map {
      [0.000 0.120   colour red  1.000  green  1.000  blue  1.000  alpha  1.000
      colour red  0.702  green  0.412  blue  0.118  alpha  0.608]
      [0.120 0.231   colour red  0.702  green  0.412  blue  0.118  alpha  0.608
      colour red  0.702  green  0.467  blue  0.118  alpha  0.608]
      [0.231 0.496   colour red  0.702  green  0.467  blue  0.118  alpha  0.608
      colour red  1.000  green  1.000  blue  1.000  alpha  1.000]
      [0.496 0.701   colour red  1.000  green  1.000  blue  1.000  alpha  1.000
      colour red  1.000  green  1.000  blue  1.000  alpha  1.000]
      [0.701 0.829   colour red  1.000  green  1.000  blue  1.000  alpha  1.000
      colour red  0.702  green  0.467  blue  0.118  alpha  0.608]
      [0.829 1   colour red  0.702  green  0.467  blue  0.118  alpha  0.608
      colour red  1.000  green  1.000  blue  1.000  alpha  1.000]
   }
   scale <0.5 0.5 0.5>
   translate <10 0 0>
}

//
//   Rosewood
//
#declare Rosewood = texture {
   bozo
   turbulence 0.04
   colour_map {
      [0.000 0.256   colour red  0.204  green  0.110  blue  0.078  alpha  0.000
      colour red  0.231  green  0.125  blue  0.090  alpha  0.000]
      [0.256 0.393   colour red  0.231  green  0.125  blue  0.090  alpha  0.000
      colour red  0.247  green  0.133  blue  0.090  alpha  0.000]
      [0.393 0.581   colour red  0.247  green  0.133  blue  0.090  alpha  0.000
      colour red  0.204  green  0.110  blue  0.075  alpha  0.000]
      [0.581 0.726   colour red  0.204  green  0.110  blue  0.075  alpha  0.000
      colour red  0.259  green  0.122  blue  0.102  alpha  0.000]
      [0.726 0.983   colour red  0.259  green  0.122  blue  0.102  alpha  0.000
      colour red  0.231  green  0.125  blue  0.086  alpha  0.000]
      [0.983 1   colour red  0.231  green  0.125  blue  0.086  alpha  0.000
      colour red  0.204  green  0.110  blue  0.078  alpha  0.000]
   }
   scale <0.5 0.5 1>
   translate <10 0 0>
   ambient 0.5
   diffuse 0.8
}
// Rosewood layer 2
texture {
   wood
   turbulence 0.04
   colour_map {
      [0.000 0.139   colour red  0.545  green  0.349  blue  0.247  alpha  1.000
      colour red  0.000  green  0.000  blue  0.000  alpha  0.004]
      [0.139 0.148   colour red  0.000  green  0.000  blue  0.000  alpha  0.004
      colour red  0.000  green  0.000  blue  0.000  alpha  0.004]
      [0.148 0.287   colour red  0.000  green  0.000  blue  0.000  alpha  0.004
      colour red  0.545  green  0.349  blue  0.247  alpha  1.000]
      [0.287 0.443   colour red  0.545  green  0.349  blue  0.247  alpha  1.000
      colour red  0.545  green  0.349  blue  0.247  alpha  1.000]
      [0.443 0.626   colour red  0.545  green  0.349  blue  0.247  alpha  1.000
      colour red  0.000  green  0.000  blue  0.000  alpha  0.004]
      [0.626 0.635   colour red  0.000  green  0.000  blue  0.000  alpha  0.004
      colour red  0.000  green  0.000  blue  0.000  alpha  0.004]
      [0.635 0.843   colour red  0.000  green  0.000  blue  0.000  alpha  0.004
      colour red  0.545  green  0.349  blue  0.247  alpha  1.000]
      [0.843 1   colour red  0.545  green  0.349  blue  0.247  alpha  1.000
      colour red  0.545  green  0.349  blue  0.247  alpha  1.000]
   }
   scale <0.5 0.5 1>
   translate <10 0 0>
   ambient 0.5
   diffuse 0.8
}

//
//   Sandalwood ( makes a great burled maple, too)
//
#declare Sandalwood = texture {
   bozo
   turbulence 0.2
   colour_map {
      [0.000 0.171   colour red  0.725  green  0.659  blue  0.455  alpha  0.000
      colour red  0.682  green  0.549  blue  0.420  alpha  0.000]
      [0.171 0.274   colour red  0.682  green  0.549  blue  0.420  alpha  0.000
      colour red  0.557  green  0.451  blue  0.322  alpha  0.000]
      [0.274 0.393   colour red  0.557  green  0.451  blue  0.322  alpha  0.000
      colour red  0.725  green  0.659  blue  0.455  alpha  0.000]
      [0.393 0.564   colour red  0.725  green  0.659  blue  0.455  alpha  0.000
      colour red  0.682  green  0.549  blue  0.420  alpha  0.000]
      [0.564 0.701   colour red  0.682  green  0.549  blue  0.420  alpha  0.000
      colour red  0.482  green  0.392  blue  0.278  alpha  0.000]
      [0.701 1   colour red  0.482  green  0.392  blue  0.278  alpha  0.000
      colour red  0.725  green  0.659  blue  0.455  alpha  0.000]
   }
   scale <0.2 0.2 1>
   scale <2 2 2>
}
// Sandalwood layer 2
texture {
   bozo
   turbulence 0.8
   colour_map {
      [0.000 0.087   colour red  0.682  green  0.604  blue  0.380  alpha  1.000
      colour red  0.761  green  0.694  blue  0.600  alpha  0.020]
      [0.087 0.226   colour red  0.761  green  0.694  blue  0.600  alpha  0.020
      colour red  0.635  green  0.553  blue  0.325  alpha  1.000]
      [0.226 0.348   colour red  0.635  green  0.553  blue  0.325  alpha  1.000
      colour red  0.761  green  0.694  blue  0.600  alpha  0.020]
      [0.348 0.496   colour red  0.761  green  0.694  blue  0.600  alpha  0.020
      colour red  0.682  green  0.604  blue  0.380  alpha  1.000]
      [0.496 0.565   colour red  0.682  green  0.604  blue  0.380  alpha  1.000
      colour red  0.761  green  0.694  blue  0.600  alpha  0.020]
      [0.565 0.661   colour red  0.761  green  0.694  blue  0.600  alpha  0.020
      colour red  0.682  green  0.604  blue  0.380  alpha  1.000]
      [0.661 0.835   colour red  0.682  green  0.604  blue  0.380  alpha  1.000
      colour red  0.761  green  0.694  blue  0.600  alpha  0.020]
      [0.835 1   colour red  0.761  green  0.694  blue  0.600  alpha  0.020
      colour red  0.682  green  0.604  blue  0.380  alpha  1.000]
   }
   scale <0.2 0.2 2.0>
   scale <2 2 2>
}


//*****************************************************************************
//                     SURFACE TEXTURES
//*****************************************************************************


// Dull creates a large, soft highlight on the object's surface
#declare Dull = texture { specular 0.5 roughness 0.15   }


// Shiny creates a small, tight highlight on the object's surface
#declare Shiny = texture { specular 1.0 roughness 0.001 }

// Phong highlights are less "realistic" than specular, but useful
// for different effects.
// Dull creates a large, soft highlight on the object's surface
#declare Phong_Dull = texture { phong 0.5  phong_size 1 }


// Shiny creates a small, tight highlight on the object's surface
#declare Phong_Shiny = texture { phong 1.0  phong_size 200 }

// Very shiny with very tight highlights and a fair amount of reflection
#declare Glossy = texture { specular 1.0 roughness 0.001 reflection 0.13}
#declare Phong_Glossy = texture {phong 1 phong_size 300 reflection 0.13}


// Luminous for shadowless skies and light_sources.
#declare Luminous = texture { ambient 1.0  diffuse 0.0 }

// a perfectly mirrored texture with no highlights
#declare Mirror = texture {
   ambient 0.0
   diffuse 0.0
   reflection 1.0
}

#declare Glass = texture {
   Shiny
   color red 1.0 green 1.0 blue 1.0  alpha 0.7
   ambient 0.0
   diffuse 0.0
   reflection 0.1
   refraction 1.0
   ior 1.5
}

// Probably more of a "Plexiglas" than glass
#declare Glass2 = texture {
   color red 1.0 green 1.0 blue 1.0 alpha 1.0
   ambient 0.0
   diffuse 0.0
   reflection 0.5
   refraction 0.85
   ior 1.5
   phong 0.3
   phong_size 60
}

// An excellent lead crystal glass!
#declare Glass3 = texture {
   color red 0.98 green 0.98 blue 0.98 alpha 0.9
   ambient 0.1
   diffuse 0.1
   specular 0.8
   reflection 0.1
   refraction 0.9
   ior 1.45
   roughness 0.001
   phong 1 phong_size 400
}

#declare Green_Glass = texture {
   Glass3
   color red 0.8 green 1 blue 0.95 alpha 0.9
}

//*****************************************************************************
//                           METAL TEXTURES
//*****************************************************************************


// Good looking "metal" textures 
// IMPORTANT: They REQUIRE that colors.inc be included BEFORE textures.inc!
#declare Chrome_Texture = texture {
   ambient 0.3
   diffuse 0.7
   reflection 0.15
   brilliance 8.0
   specular 0.8
   roughness 0.1
   color LightGray
}

// You need to specify a color when Metal is used
#declare Metal = texture {
   metallic
   ambient 0.2
   diffuse 0.7
   brilliance 6.0
   reflection 0.25
   phong 0.75
   phong_size 80
}

// A series of metallic textures using the Metal texture:
#declare Brass_Texture   = texture {  Metal  colour Brass      }
#declare Gold_Texture    = texture {  Metal  colour BrightGold }
#declare Bronze_Texture  = texture {  Metal  colour Bronze     }
#declare Copper_Texture  = texture {  Metal  colour Copper     }
#declare Silver_Texture  = texture {  Metal  color Silver      }

// In the future, please refer to Chrome_Texture by this name.  I'd like
// to scrap the old name someday. Ditto with other "_Texture" names!
#declare Chrome_Metal = texture { Chrome_Texture  }
#declare Brass_Metal  = texture { Brass_Texture   }
#declare Gold_Metal   = texture { Gold_Texture    }
#declare Bronze_Metal = texture { Bronze_Texture  }
#declare Copper_Metal = texture { Copper_Texture  }
#declare Silver_Metal  = texture {  Metal  color Silver      }

// Interesting texture -- Give it a try.
// Sort of a "Black Hills Gold", black, white, and orange specks or splotches.
#declare Brass_Valley = texture {
   granite
   metallic
   brilliance 6.0
   reflection 0.75
   phong 0.75
   color_map {
      [0.0 0.3   color Feldspar  color Feldspar]
      [0.3 0.6   color Mica      color Quartz]
      [0.6 1 color Feldspar  color Quartz]
   }
}

#declare Rusty_Iron = texture {
   agate
   colour_map {
      [0.0 0.5   colour red 0.21 green 0.1 blue 0.1
      colour red 0.25 green 0.25 blue 0.01]
      [0.5 0.6   colour red 0.25 green 0.25 blue 0.01
      colour red 0.3 green 0.1 blue 0.1]
      [0.6 1 colour red 0.15 green 0.1 blue 0.1
      colour red 0.15 green 0.1 blue 0.1]
   }
}

#declare Rust  = texture {
   spotted
   colour_map {
      [0.0 0.4   colour red 0.89 green 0.51 blue 0.28
      colour red 0.70 green 0.13 blue 0.00]
      [0.4 0.5   colour red 0.70 green 0.13 blue 0.00
      colour red 0.69 green 0.41 blue 0.08]
      [0.5 0.6   colour red 0.69 green 0.41 blue 0.08
      colour red 0.49 green 0.31 blue 0.28]
      [0.6 1 colour red 0.49 green 0.31 blue 0.28
      colour red 0.89 green 0.51 blue 0.28]
   }
}


//*****************************************************************************
//                    SPECIAL EFFECTS
//*****************************************************************************

// Red & white stripes - Looks best on a y axis Cylinder
// It "spirals" because it's gradient on two axis
#declare Candy_Cane = texture {
   gradient < 1.0 1.0 0.0 > 
   colour_map {
      [0.00 0.25  colour red 1.0 green 0.0 blue 0.0
      colour red 1.0 green 0.0 blue 0.0]
      [0.25 0.75  colour red 1.0 green 1.0 blue 1.0
      colour red 1.0 green 1.0 blue 1.0]
      [0.75 1 colour red 1.0 green 0.0 blue 0.0
      colour red 1.0 green 0.0 blue 0.0]
   }
}

// Orange and Clear stripes spiral around the texture
// to make an object look like it was "Peeled"
// Now, you too can be M.C. Escher 
#declare Peel = texture {
   gradient < 1.0 1.0 0.0 > 
   colour_map {
      [0.00 0.25  colour Orange
      colour Orange]
      [0.25 0.75  colour Clear
      colour Clear]
      [0.75 1.001 colour Orange
      colour Orange]
   }
}

#declare Y_Gradient = texture {
   gradient < 0.0 1.0 0.0 >
   colour_map {
      [0.00 0.33  colour red 1.0 green 0.0 blue 0.0
      colour red 0.0 green 0.0 blue 1.0]
      [0.33 0.66  colour red 0.0 green 0.0 blue 1.0
      colour red 0.0 green 1.0 blue 0.0]
      [0.66 1 colour red 0.0 green 1.0 blue 0.0
      colour red 1.0 green 0.0 blue 0.0]
   }
}

#declare X_Gradient = texture {
   gradient < 1.0 0.0 0.0 >
   colour_map {
      [0.00 0.33  colour red 1.0 green 0.0 blue 0.0
      colour red 0.0 green 0.0 blue 1.0]
      [0.33 0.66  colour red 0.0 green 0.0 blue 1.0
      colour red 1.0 green 1.0 blue 1.0]
      [0.66 1 colour red 1.0 green 1.0 blue 1.0
      colour red 1.0 green 1.0 blue 1.0]
   }
}

// A good wavy water example.
// Requires a sub-plane, and may require scaling to fit your scene.
#declare Water = texture {
   colour Blue alpha 0.9
   ripples 0.75
   frequency 10.0
   reflection 0.3
   refraction 0.5
   ior 1.33
}

#declare Cork = texture {
   granite
   colour_map {
      [0.0 0.6  colour red 0.93 green 0.71 blue 0.532
      colour red 0.98 green 0.81 blue 0.60]
      [0.6 0.65 colour red 0.50 green 0.30 blue 0.20
      colour red 0.50 green 0.30 blue 0.20]
      [0.65 1.0 colour red 0.80 green 0.53 blue 0.46
      colour red 0.85 green 0.75 blue 0.35]
   }
   scale <0.25 0.25 0.25>     // Generally looks best scaled
   // longer on one axis
   specular 0.1 roughness 0.5 // Very dull 
}
