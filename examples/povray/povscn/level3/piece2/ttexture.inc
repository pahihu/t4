// Persistence Of Vision raytracer version 1.0 sample file.

#declare oak =  texture {
   wood
   turbulence 0.06
   color_map {
      [0.00 0.10 color red 0.42 green 0.26 blue 0.15
      color red 0.42 green 0.26 blue 0.15]
      [0.10 0.90 color red 0.42 green 0.26 blue 0.15
      color red 0.52 green 0.37 blue 0.26]
      [0.90 1.01 color red 0.52 green 0.37 blue 0.26
      color red 0.52 green 0.37 blue 0.26 ]
   }
   ambient 0.0005
   diffuse 0.6
   specular 1.0
   roughness 0.0006
   reflection 0.35
}

#declare gilt_texture = texture {
   marble
   turbulence 0.55
   color_map {
      [0.0   0.65   color Black
      color DimGray]
      [0.65  0.775  color DimGray
      color SteelBlue]
      [0.775 1.001  color SteelBlue
      color Black]
   }
   ambient 0.05
   diffuse 0.65
   specular 1.0
   roughness 0.00002432
   reflection 0.8
   scale <10 8 8>
}

#declare rod_texture =
texture {
   wood
   turbulence 0.1
   color_map {
      [0.00 0.20 color red 0.33 green 0.17 blue 0.6
      color red 0.33 green 0.17 blue 0.6]
      [0.20 0.95 color red 0.33 green 0.17 blue 0.6 alpha 0.9
      color red 0.43 green 0.28 blue 0.17]
      [0.95 1.01 color red 0.43 green 0.28 blue 0.17
      color red 0.43 green 0.28 blue 0.17]
   }
   ambient 0.0005
   diffuse 0.6
   specular 1.0
   roughness 0.0006
   reflection 0.4
}

#declare brace_texture =
texture {
   color Copper
   ambient 0.005
   diffuse 0.55
   specular 0.85
   roughness 0.0000044
   reflection 0.97
}

#declare plate_texture =
texture {
   color Sienna
   ambient 0.05
   diffuse 0.4
   specular 1.0
   roughness 0.0000245
   reflection 0.45
}

#declare spural_texture =
texture {
   marble
   turbulence 0.4
   color_map {
      [0.0  0.73 color Copper
      color red 0.42 green 0.26 blue 0.15]
      [0.73 0.85 color red 0.42 green 0.26 blue 0.15
      color Firebrick]
      [0.85 0.91 color Firebrick
      color Black]
      [0.91 1.01 color Black
      color Copper]
   }
   specular 1.0
   roughness 0.000024
   ambient 0.05
   diffuse 0.65
   reflection 0.7
   rotate <90 0 0>
   scale <2 2 2>
}

#declare R_Marble1 = texture {
   marble
   turbulence 0.1
   color_map {
      [0.0 0.5   color red 0.6 green 0.5 blue 0.4
      color red 0.5 green 0.2 blue 0.2]
      [0.5 1.001 color red 0.5 green 0.2 blue 0.2
      color red 0.5 green 0.5 blue 0.4]
   }
   ambient 0.05
   diffuse 0.65
   specular 1.0
   roughness 0.000032
   reflection 0.2
   rotate <0 90 0>
   scale <3.8 3.8 3.8>
   translate <0 0 -1.2>
}

#declare R_Marble2 = texture {
   color red 0.8 green 0.8 blue 0.6
   ambient 0.05
   diffuse 0.65
   specular 1.0
   roughness 0.00003
   reflection 0.1
}

#declare Iris = texture {
   color Black
   ambient 0.05
   diffuse 0.55
   phong 1.0
   phong_size 1000
   specular 1.0
   roughness 0.0004
   reflection 0.75
}
