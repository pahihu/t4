/*

		   Persistence of Vision Raytracer 

		  Stone textures by Mike Miller  1992

    Contains declared texture statements defining a variety of
    stone granite & marble textures. Most use the granite texture.
    Turbulence has no effect on granite, but turbulence is stated
    before the color map for convenience of switching to marble, which
    does need a turbulence to swirl the color. I tried to avoid using
    a random dither, but I find it helps create a subtle grain & can
    spark lost colors in the map. On multi-layered texture, try a float
    of about 0.05 on the first texture.

    Final Note: I would not "INCLUDE" this whole data file into a scene
		file if only one or two of these textures were being
		used...There are too many declares being used.

    -----------------------------------------------------------------------
    The textures Grnt0-Grnt29, Grnt0A-Grnt24A and Crack1-Crack4 are
    "building blocks" that are used to create the final "usable" textures,
    Stone1 - Stone24 (and other textures that *you* design, of course!)
	    
    INDEX:

	    Grnt0  - Grnt29   color maps (generally) contain no alpha values
	    ------------------------------
		Grnt0  - Gray/Tan with Rose.
		Grnt1  - Creamy Whites with Yellow & Light Gray.
		Grnt2  - Deep Cream with Light Rose, Yellow, Orchid, & Tan.
		Grnt3  - Warm tans olive & light rose with cream.
		Grnt4  - Orchid, Sand & Mauve.
		Grnt5  - Medium Mauve Med.Rose & Deep Cream.
		Grnt6  - Med. Orchid, Olive & Dark Tan "mud pie".
		Grnt7  - Dark Orchid, Olive & Dark Putty.
		Grnt8  - Rose & Light Cream Yellows
		Grnt9  - Light Steely Grays
		Grnt10 - Gray Creams & Lavender Tans
		Grnt11 - Creams & Grays  Kahki
		Grnt12 - Tan Cream & Red Rose
		Grnt13 - Cream Rose Orange
		Grnt14 - Cream Rose & Light Moss w/Light Violet
		Grnt15 - Black with subtle chroma
		Grnt16 - White Cream & Peach
		Grnt17 - Bug Juice & Green
		Grnt18 - Rose & Creamy Yellow
		Grnt19 - Gray Marble with White feather Viens
		Grnt20 - White Marble with Gray feather Viens
		Grnt21 - Green Jade
		Grnt22 - Clear with White feather Viens (has some transparency)
		Grnt23 - Light Tan to Mauve
		Grnt24 - Light Grays
		Grnt25 - Moss Greens & Tan
		Grnt26 - Salmon with thin Green Viens
		Grnt27 - Dark Green & Browns
		Grnt28 - Red Swirl
		Grnt29 - White, Tan, w/ thin Red Viens

	    Grnt0A - Grnt24A  color maps containing alpha
	    ------------------------------
		Grnt0a  - Translucent Grnt0
		Grnt1a  - Translucent Grnt1
		Grnt2a  - Translucent Grnt2
		Grnt3a  - Translucent Grnt3
		Grnt4a  - Translucent Grnt4
		Grnt5a  - Translucent Grnt5
		Grnt6a  - Translucent Grnt6
		Grnt7a  - Translucent Grnt7
		Grnt8a  - Aqua Tints
		Grnt9a  - Alpha Creams With Cracks
		Grnt10a - Alpha Cream Rose & light yellow
		Grnt11a - Alpha Light Grays
		Grnt12a - Alpha Creams & Tans
		Grnt13a - Alpha Creams & Grays
		Grnt14a - Cream Rose & light moss
		Grnt15a - Alpha Sand & light Orange
		Grnt16a - Cream Rose & light moss (again?)
		Grnt17a - ???
		Grnt18a - ???
		Grnt19a - Gray Marble with White feather Viens with Alpha
		Grnt20a - White Feather Viens
		Grnt21a - Thin White Feather Viens
		Grnt22a - ???
		Grnt23a - Transparent Green Moss
		Grnt24a - ???

	    Crack1 - Crack4   clear with an thin opaque band for Cracks
	    ------------------------------
		Crack1 - Crack & Red Overtint
		Crack2 - Translucent Dark Cracks
		Crack3 - Overtint Green w/ Black Cracks
		Crack4 - Overtint w/ White Crack

	    OTHERS

	    Stone1 - Stone24  complete texture statements - edit to your
			      scene & lighting situations.
	    ------------------------------
		Stone1 - Deep Rose & Green Marble with large White Swirls
		Stone2 - Light Greenish Tan Marble with Agate style veining
		Stone3 - Rose & Yellow Marble with fog white veining
		Stone4 - Tan Marble with Rose patches
		Stone5 - White Cream Marble with Pink veining
		Stone6 - Rose & Yellow Cream Marble
		Stone7 - Light Coffee Marble with darker patches
		Stone8 - Gray Granite with white patches
		Stone9 - White & Light Blue Marble with light violets
		Stone10- Dark Brown & Tan swirl Granite with gray undertones
		Stone11- Rose & White Marble with dark tan swirl
		Stone12- White & Pinkish Tan Marble
		Stone13- Medium Gray Blue Marble
		Stone14- Tan & Olive Marble with gray white veins
		Stone15- Deep Gray Marble with white veining
		Stone16- Peach & Yellow Marble with white veining
		Stone17- White Marble with gray veining
		Stone18- Green Jade with white veining
		Stone19- Peach Granite with white patches & green trim
		Stone20- Brown & Olive Marble with white veining
		Stone21- Red Marble with gray & white veining
		Stone22- Dark Tan Marble with gray & white veining
		Stone23- Peach & Cream Marble with orange veining
		Stone24- Green & Tan Moss Marble

    -----------------------------------------------------------------------
*/



//--------- Gray  Tan with Rose
#declare Grnt0 = texture {
   granite
   turbulence 0.4
   colour_map {
      [0.000 0.153   colour red  0.729  green  0.502  blue  0.451  alpha  0.000
      colour red  0.769  green  0.686  blue  0.592  alpha  0.000]
      [0.153 0.398   colour red  0.769  green  0.686  blue  0.592  alpha  0.000
      colour red  0.843  green  0.753  blue  0.718  alpha  0.000]
      [0.398 0.559   colour red  0.843  green  0.753  blue  0.718  alpha  0.000
      colour red  0.780  green  0.667  blue  0.561  alpha  0.000]
      [0.559 0.729   colour red  0.780  green  0.667  blue  0.561  alpha  0.000
      colour red  0.741  green  0.659  blue  0.576  alpha  0.000]
      [0.729 1.001   colour red  0.741  green  0.659  blue  0.576  alpha  0.000
      colour red  0.729  green  0.502  blue  0.451  alpha  0.000]
   }
}

//----- Creamy Whites with yellow & light gray
#declare Grnt1 = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.212   colour red  0.898  green  0.898  blue  0.851  alpha  0.000
      colour red  0.969  green  0.980  blue  0.875  alpha  0.000]
      [0.212 0.424   colour red  0.969  green  0.980  blue  0.875  alpha  0.000
      colour red  0.859  green  0.859  blue  0.859  alpha  0.000]
      [0.424 0.627   colour red  0.859  green  0.859  blue  0.859  alpha  0.000
      colour red  0.992  green  0.922  blue  0.659  alpha  0.000]
      [0.627 0.881   colour red  0.992  green  0.922  blue  0.659  alpha  0.000
      colour red  0.937  green  0.965  blue  0.902  alpha  0.000]
      [0.881 1.001   colour red  0.937  green  0.965  blue  0.902  alpha  0.000
      colour red  0.898  green  0.898  blue  0.851  alpha  0.000]
   }
}

//------- Deep Cream with light rose, yellow orchid & tan
#declare Grnt2 = texture {
   granite
   turbulence 0.5
   colour_map {
      [0.000 0.178   colour red  0.863  green  0.757  blue  0.596  alpha  0.000
      colour red  0.925  green  0.792  blue  0.714  alpha  0.000]
      [0.178 0.356   colour red  0.925  green  0.792  blue  0.714  alpha  0.000
      colour red  0.871  green  0.702  blue  0.659  alpha  0.000]
      [0.356 0.525   colour red  0.871  green  0.702  blue  0.659  alpha  0.000
      colour red  0.992  green  0.922  blue  0.659  alpha  0.000]
      [0.525 0.729   colour red  0.992  green  0.922  blue  0.659  alpha  0.000
      colour red  0.902  green  0.812  blue  0.714  alpha  0.000]
      [0.729 1.001   colour red  0.902  green  0.812  blue  0.714  alpha  0.000
      colour red  0.863  green  0.757  blue  0.596  alpha  0.000]
   }
}

//------- Warm tans olive & light rose with cream
#declare Grnt3 = texture {
   granite
   turbulence 0.5
   colour_map {
      [0.000 0.178   colour red  0.831  green  0.631  blue  0.569  alpha  0.000
      colour red  0.925  green  0.831  blue  0.714  alpha  0.000]
      [0.178 0.356   colour red  0.925  green  0.831  blue  0.714  alpha  0.000
      colour red  0.871  green  0.702  blue  0.659  alpha  0.000]
      [0.356 0.525   colour red  0.871  green  0.702  blue  0.659  alpha  0.000
      colour red  0.831  green  0.631  blue  0.569  alpha  0.000]
      [0.525 0.729   colour red  0.831  green  0.631  blue  0.569  alpha  0.000
      colour red  0.937  green  0.882  blue  0.820  alpha  0.000]
      [0.729 1.001   colour red  0.937  green  0.882  blue  0.820  alpha  0.000
      colour red  0.831  green  0.631  blue  0.569  alpha  0.000]
   }
}

//-------- Orchid sand & mouve
#declare Grnt4 = texture {
   granite
   turbulence 0.5
   colour_map {
      [0.000 0.178   colour red  0.804  green  0.569  blue  0.494  alpha  0.000
      colour red  0.816  green  0.725  blue  0.537  alpha  0.000]
      [0.178 0.356   colour red  0.816  green  0.725  blue  0.537  alpha  0.000
      colour red  0.820  green  0.580  blue  0.522  alpha  0.000]
      [0.356 0.525   colour red  0.820  green  0.580  blue  0.522  alpha  0.000
      colour red  0.882  green  0.725  blue  0.537  alpha  0.000]
      [0.525 0.729   colour red  0.882  green  0.725  blue  0.537  alpha  0.000
      colour red  0.855  green  0.729  blue  0.584  alpha  0.000]
      [0.729 1.001   colour red  0.855  green  0.729  blue  0.584  alpha  0.000
      colour red  0.804  green  0.569  blue  0.494  alpha  0.000]
   }
}

//------- Medium Mauve Med.Rose & deep cream
#declare Grnt5 = texture {
   granite
   turbulence 0.5
   colour_map {
      [0.000 0.178   colour red  0.804  green  0.569  blue  0.494  alpha  0.000
      colour red  0.855  green  0.729  blue  0.584  alpha  0.000]
      [0.178 0.356   colour red  0.855  green  0.729  blue  0.584  alpha  0.000
      colour red  0.667  green  0.502  blue  0.478  alpha  0.000]
      [0.356 0.525   colour red  0.667  green  0.502  blue  0.478  alpha  0.000
      colour red  0.859  green  0.624  blue  0.545  alpha  0.000]
      [0.525 0.729   colour red  0.859  green  0.624  blue  0.545  alpha  0.000
      colour red  0.855  green  0.729  blue  0.584  alpha  0.000]
      [0.729 1.001   colour red  0.855  green  0.729  blue  0.584  alpha  0.000
      colour red  0.804  green  0.569  blue  0.494  alpha  0.000]
   }
}

//--------- Med. Orchid Olive & Dark Tan "mud pie"
#declare Grnt6 = texture {
   granite
   turbulence 0.5
   colour_map {
      [0.000 0.153   colour red  0.545  green  0.380  blue  0.345  alpha  0.000
      colour red  0.588  green  0.475  blue  0.333  alpha  0.000]
      [0.153 0.398   colour red  0.588  green  0.475  blue  0.333  alpha  0.000
      colour red  0.675  green  0.478  blue  0.404  alpha  0.000]
      [0.398 0.559   colour red  0.675  green  0.478  blue  0.404  alpha  0.000
      colour red  0.757  green  0.635  blue  0.522  alpha  0.000]
      [0.559 0.729   colour red  0.757  green  0.635  blue  0.522  alpha  0.000
      colour red  0.659  green  0.549  blue  0.443  alpha  0.000]
      [0.729 1.001   colour red  0.659  green  0.549  blue  0.443  alpha  0.000
      colour red  0.545  green  0.380  blue  0.345  alpha  0.000]
   }
}

//------- Dark Orchid Olive & Dark Putty
#declare Grnt7 = texture {
   granite
   turbulence 0.5
   colour_map {
      [0.000 0.153   colour red  0.439  green  0.310  blue  0.282  alpha  0.000
      colour red  0.463  green  0.369  blue  0.259  alpha  0.000]
      [0.153 0.398   colour red  0.463  green  0.369  blue  0.259  alpha  0.000
      colour red  0.541  green  0.369  blue  0.298  alpha  0.000]
      [0.398 0.559   colour red  0.541  green  0.369  blue  0.298  alpha  0.000
      colour red  0.573  green  0.424  blue  0.286  alpha  0.000]
      [0.559 0.729   colour red  0.573  green  0.424  blue  0.286  alpha  0.000
      colour red  0.494  green  0.396  blue  0.306  alpha  0.000]
      [0.729 1.001   colour red  0.494  green  0.396  blue  0.306  alpha  0.000
      colour red  0.439  green  0.310  blue  0.282  alpha  0.000]
   }
}

//--------- Rose & Light cream Yellows
#declare Grnt8 = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.179   colour red  0.843  green  0.655  blue  0.655  alpha  0.000
      colour red  0.886  green  0.769  blue  0.627  alpha  0.000]
      [0.179 0.368   colour red  0.886  green  0.769  blue  0.627  alpha  0.000
      colour red  0.906  green  0.820  blue  0.714  alpha  0.000]
      [0.368 0.538   colour red  0.906  green  0.820  blue  0.714  alpha  0.000
      colour red  0.851  green  0.671  blue  0.671  alpha  0.000]
      [0.538 0.846   colour red  0.851  green  0.671  blue  0.671  alpha  0.000
      colour red  0.890  green  0.792  blue  0.675  alpha  0.000]
      [0.846 0.983   colour red  0.890  green  0.792  blue  0.675  alpha  0.000
      colour red  0.827  green  0.612  blue  0.612  alpha  0.000]
      [0.983 1.001   colour red  0.827  green  0.612  blue  0.612  alpha  0.000
      colour red  0.843  green  0.655  blue  0.655  alpha  0.000]
   }
}

//--------- Light Steely Grays
#declare Grnt9 = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.894  green  0.886  blue  0.886  alpha  0.000
      colour red  0.745  green  0.745  blue  0.753  alpha  0.000]
      [0.154 0.308   colour red  0.745  green  0.745  blue  0.753  alpha  0.000
      colour red  0.902  green  0.902  blue  0.859  alpha  0.000]
      [0.308 0.444   colour red  0.902  green  0.902  blue  0.859  alpha  0.000
      colour red  0.729  green  0.706  blue  0.694  alpha  0.000]
      [0.444 0.615   colour red  0.729  green  0.706  blue  0.694  alpha  0.000
      colour red  0.588  green  0.592  blue  0.635  alpha  0.000]
      [0.615 0.803   colour red  0.588  green  0.592  blue  0.635  alpha  0.000
      colour red  0.608  green  0.616  blue  0.659  alpha  0.000]
      [0.803 1.001   colour red  0.608  green  0.616  blue  0.659  alpha  0.000
      colour red  0.894  green  0.886  blue  0.886  alpha  0.000]
   }
}

//--------- Gray Creams & lavender tans
#declare Grnt10 = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.890  green  0.690  blue  0.690  alpha  0.000
      colour red  0.996  green  0.835  blue  0.737  alpha  0.000]
      [0.154 0.308   colour red  0.996  green  0.835  blue  0.737  alpha  0.000
      colour red  0.745  green  0.635  blue  0.651  alpha  0.004]
      [0.308 0.444   colour red  0.745  green  0.635  blue  0.651  alpha  0.004
      colour red  0.733  green  0.596  blue  0.557  alpha  0.004]
      [0.444 0.615   colour red  0.733  green  0.596  blue  0.557  alpha  0.004
      colour red  0.996  green  0.835  blue  0.737  alpha  0.000]
      [0.615 0.803   colour red  0.996  green  0.835  blue  0.737  alpha  0.000
      colour red  0.765  green  0.616  blue  0.659  alpha  0.000]
      [0.803 1.001   colour red  0.765  green  0.616  blue  0.659  alpha  0.000
      colour red  0.890  green  0.690  blue  0.690  alpha  0.000]
   }
}

//--------- Creams & Grays Kakhi
#declare Grnt11 = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.800  green  0.651  blue  0.557  alpha  0.000
      colour red  0.996  green  0.835  blue  0.737  alpha  0.000]
      [0.154 0.308   colour red  0.996  green  0.835  blue  0.737  alpha  0.000
      colour red  0.800  green  0.651  blue  0.557  alpha  0.000]
      [0.308 0.444   colour red  0.800  green  0.651  blue  0.557  alpha  0.000
      colour red  0.694  green  0.624  blue  0.604  alpha  0.004]
      [0.444 0.615   colour red  0.694  green  0.624  blue  0.604  alpha  0.004
      colour red  0.800  green  0.651  blue  0.557  alpha  0.000]
      [0.615 0.812   colour red  0.800  green  0.651  blue  0.557  alpha  0.000
      colour red  0.725  green  0.655  blue  0.651  alpha  0.000]
      [0.812 1.001   colour red  0.725  green  0.655  blue  0.651  alpha  0.000
      colour red  0.800  green  0.651  blue  0.557  alpha  0.000]
   }
}

//--------- Tan Cream & Red Rose 
#declare Grnt12 = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.996  green  0.969  blue  0.800  alpha  0.000
      colour red  0.996  green  0.682  blue  0.604  alpha  0.000]
      [0.154 0.308   colour red  0.996  green  0.682  blue  0.604  alpha  0.000
      colour red  0.906  green  0.820  blue  0.714  alpha  0.000]
      [0.308 0.444   colour red  0.906  green  0.820  blue  0.714  alpha  0.000
      colour red  0.816  green  0.631  blue  0.537  alpha  0.000]
      [0.444 0.615   colour red  0.816  green  0.631  blue  0.537  alpha  0.000
      colour red  0.890  green  0.792  blue  0.675  alpha  0.000]
      [0.615 0.812   colour red  0.890  green  0.792  blue  0.675  alpha  0.000
      colour red  0.973  green  0.627  blue  0.627  alpha  0.000]
      [0.812 1.001   colour red  0.973  green  0.627  blue  0.627  alpha  0.000
      colour red  0.996  green  0.969  blue  0.800  alpha  0.000]
   }
}

//--------- Cream Rose orange 
#declare Grnt13 = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.996  green  0.824  blue  0.780  alpha  0.000
      colour red  0.996  green  0.698  blue  0.624  alpha  0.000]
      [0.154 0.308   colour red  0.996  green  0.698  blue  0.624  alpha  0.000
      colour red  0.906  green  0.675  blue  0.553  alpha  0.000]
      [0.308 0.444   colour red  0.906  green  0.675  blue  0.553  alpha  0.000
      colour red  0.996  green  0.682  blue  0.604  alpha  0.000]
      [0.444 0.615   colour red  0.996  green  0.682  blue  0.604  alpha  0.000
      colour red  0.996  green  0.824  blue  0.780  alpha  0.000]
      [0.615 0.812   colour red  0.996  green  0.824  blue  0.780  alpha  0.000
      colour red  0.973  green  0.627  blue  0.627  alpha  0.000]
      [0.812 1.001   colour red  0.973  green  0.627  blue  0.627  alpha  0.000
      colour red  0.996  green  0.824  blue  0.780  alpha  0.000]
   }
}

//--------- Cream Rose & light moss & light Violet
#declare Grnt14 = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.690  green  0.612  blue  0.569  alpha  0.000
      colour red  0.737  green  0.596  blue  0.522  alpha  0.000]
      [0.154 0.368   colour red  0.737  green  0.596  blue  0.522  alpha  0.000
      colour red  0.776  green  0.702  blue  0.624  alpha  0.000]
      [0.368 0.538   colour red  0.776  green  0.702  blue  0.624  alpha  0.000
      colour red  0.796  green  0.678  blue  0.643  alpha  0.000]
      [0.538 0.846   colour red  0.796  green  0.678  blue  0.643  alpha  0.000
      colour red  0.690  green  0.612  blue  0.569  alpha  0.000]
      [0.846 0.932   colour red  0.690  green  0.612  blue  0.569  alpha  0.000
      colour red  0.773  green  0.612  blue  0.569  alpha  0.000]
      [0.932 1.001   colour red  0.773  green  0.612  blue  0.569  alpha  0.000
      colour red  0.690  green  0.612  blue  0.569  alpha  0.000]
   }
}

//--------- Black with subtle chroma
#declare Grnt15 = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.104   colour red  0.161  green  0.133  blue  0.118  alpha  0.000
      colour red  0.110  green  0.082  blue  0.071  alpha  0.000]
      [0.104 0.252   colour red  0.110  green  0.082  blue  0.071  alpha  0.000
      colour red  0.161  green  0.133  blue  0.118  alpha  0.000]
      [0.252 0.383   colour red  0.161  green  0.133  blue  0.118  alpha  0.000
      colour red  0.000  green  0.000  blue  0.000  alpha  0.000]
      [0.383 0.643   colour red  0.000  green  0.000  blue  0.000  alpha  0.000
      colour red  0.161  green  0.133  blue  0.118  alpha  0.000]
      [0.643 0.783   colour red  0.161  green  0.133  blue  0.118  alpha  0.000
      colour red  0.220  green  0.149  blue  0.137  alpha  0.000]
      [0.783 0.922   colour red  0.220  green  0.149  blue  0.137  alpha  0.000
      colour red  0.000  green  0.000  blue  0.000  alpha  0.000]
      [0.922 0.983   colour red  0.000  green  0.000  blue  0.000  alpha  0.000
      colour red  0.220  green  0.149  blue  0.137  alpha  0.000]
      [0.983 1.001   colour red  0.220  green  0.149  blue  0.137  alpha  0.000
      colour red  0.161  green  0.133  blue  0.118  alpha  0.000]
   }
}

//----- White Cream & Peach
#declare Grnt16 = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.316   colour red  0.910  green  0.788  blue  0.788  alpha  0.000
      colour red  0.922  green  0.914  blue  0.871  alpha  0.000]
      [0.316 0.453   colour red  0.922  green  0.914  blue  0.871  alpha  0.000
      colour red  0.894  green  0.867  blue  0.780  alpha  0.000]
      [0.453 0.624   colour red  0.894  green  0.867  blue  0.780  alpha  0.000
      colour red  0.784  green  0.788  blue  0.788  alpha  0.000]
      [0.624 0.726   colour red  0.784  green  0.788  blue  0.788  alpha  0.000
      colour red  0.851  green  0.812  blue  0.741  alpha  0.000]
      [0.726 0.863   colour red  0.851  green  0.812  blue  0.741  alpha  0.000
      colour red  0.647  green  0.655  blue  0.655  alpha  0.000]
      [0.863 1.001   colour red  0.647  green  0.655  blue  0.655  alpha  0.000
      colour red  0.910  green  0.788  blue  0.788  alpha  0.000]
   }
}

//----- Bug Juice & Green
#declare Grnt17 = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.303   colour red  0.000  green  0.239  blue  0.000  alpha  0.000
      colour red  0.333  green  0.294  blue  0.000  alpha  0.000]
      [0.303 0.588   colour red  0.333  green  0.294  blue  0.000  alpha  0.000
      colour red  0.000  green  0.239  blue  0.341  alpha  0.000]
      [0.588 0.790   colour red  0.000  green  0.239  blue  0.341  alpha  0.000
      colour red  0.000  green  0.020  blue  0.000  alpha  0.000]
      [0.790 1.001   colour red  0.000  green  0.020  blue  0.000  alpha  0.000
      colour red  0.000  green  0.239  blue  0.000  alpha  0.000]
   }
}

//------------ Rose & cream yellow
#declare Grnt18 = texture {
   granite
   turbulence 0.4
   colour_map {
      [0.000 0.202   colour red  1.000  green  0.718  blue  0.541  alpha  0.000
      colour red  0.890  green  0.651  blue  0.612  alpha  0.000]
      [0.202 0.298   colour red  0.890  green  0.651  blue  0.612  alpha  0.000
      colour red  1.000  green  0.820  blue  0.675  alpha  0.000]
      [0.298 0.377   colour red  1.000  green  0.820  blue  0.675  alpha  0.000
      colour red  0.890  green  0.643  blue  0.612  alpha  0.000]
      [0.377 0.465   colour red  0.890  green  0.643  blue  0.612  alpha  0.000
      colour red  0.937  green  0.729  blue  0.561  alpha  0.000]
      [0.465 0.544   colour red  0.937  green  0.729  blue  0.561  alpha  0.000
      colour red  0.878  green  0.604  blue  0.565  alpha  0.000]
      [0.544 0.640   colour red  0.878  green  0.604  blue  0.565  alpha  0.000
      colour red  0.984  green  0.780  blue  0.655  alpha  0.000]
      [0.640 0.860   colour red  0.984  green  0.780  blue  0.655  alpha  0.000
      colour red  1.000  green  0.863  blue  0.635  alpha  0.000]
      [0.860 0.982   colour red  1.000  green  0.863  blue  0.635  alpha  0.000
      colour red  1.000  green  0.765  blue  0.620  alpha  0.000]
      [0.982 1.001   colour red  1.000  green  0.765  blue  0.620  alpha  0.000
      colour red  1.000  green  0.718  blue  0.541  alpha  0.000]
   }
}

//--------- Gray Marble with White feather Viens
#declare Grnt19 = texture {
   granite 
   turbulence 0.0
   color_map {
      [0.0 0.3 color White color DimGray]
      [0.2 0.4 color DimGray color DimGray]
      [0.4 0.6 color DimGray color DimGray]
      [0.6 1.0 color DimGray color DimGray]
   }
   0.02
}

//--------- White Marble with Gray feather Viens
#declare Grnt20 = texture {
   granite 
   turbulence 0.0
   color_map {
      [0.0 0.3 color Mica color White]
      [0.2 0.4 color White color White]
      [0.4 0.6 color White color White]
      [0.6 1.0 color White color White]
   }
   0.02
}

//-------- Declare Green Colors
#declare g1 = color red 0.26 green 0.41 blue 0.31 //---Light Gray Green
#declare g2 = color red 0.27 green 0.34 blue 0.26 //---Med Gray Green
#declare g3 = color red 0.13 green 0.29 blue 0.28 //---Med Gray Aqua
#declare g4 = color red 0.03 green 0.18 blue 0.08 //---Dark Green

//--------- Green Jade
#declare Grnt21 = texture {
   granite 
   turbulence 0.0
   color_map {
      [0.0 0.1 color White alpha 0.3 color SeaGreen alpha 0.4]
      [0.1 0.3 color SeaGreen alpha 0.4 color g2 alpha 0.7 ]
      [0.3 0.5 color g2  alpha 0.7 color DarkGreen alpha 0.7 ]
      [0.5 0.7 color DarkGreen  alpha 0.7 color g4 alpha 0.7 ]
      [0.7 0.8 color g4 color  alpha 0.7 DarkGreen alpha 0.7 ]
      [0.8 1.0 color DarkGreen  alpha 0.7 color DarkGreen alpha 0.7 ]
   }
   0.02
}

//--------- Clear with White feather Viens ----- This one does contain Alpha
#declare Grnt22 = texture {
   granite 
   turbulence 0.0
   color_map {
      [0.0 0.07 color White color White]
      [0.07 0.2 color White color DimGray ]
      [0.2 0.3 color DimGray color Clear]
      [0.3 0.7 color Clear color Clear]
      [0.7 1.0 color Clear color DimGray ]
   }
   0.02
}

//---------- Light Tan to Mouve 
#declare Grnt23 = texture {
   marble
   turbulence 0.5
   colour_map {
      [0.000 0.178   colour red  0.831  green  0.631  blue  0.569  alpha  0.000
      colour red  0.925  green  0.831  blue  0.714  alpha  0.000]
      [0.178 0.356   colour red  0.925  green  0.831  blue  0.714  alpha  0.000
      colour red  0.871  green  0.702  blue  0.659  alpha  0.000]
      [0.356 0.525   colour red  0.871  green  0.702  blue  0.659  alpha  0.000
      colour red  0.831  green  0.631  blue  0.569  alpha  0.000]
      [0.525 0.729   colour red  0.831  green  0.631  blue  0.569  alpha  0.000
      colour red  0.937  green  0.882  blue  0.820  alpha  0.000]
      [0.729 1.001   colour red  0.937  green  0.882  blue  0.820  alpha  0.000
      colour red  0.831  green  0.631  blue  0.569  alpha  0.000]
   }
}

//--------- Light Grays
#declare Grnt24 = texture {
   marble
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.894  green  0.886  blue  0.886  alpha  0.000
      colour red  0.745  green  0.745  blue  0.753  alpha  0.000]
      [0.154 0.308   colour red  0.745  green  0.745  blue  0.753  alpha  0.000
      colour red  0.902  green  0.902  blue  0.859  alpha  0.000]
      [0.308 0.444   colour red  0.902  green  0.902  blue  0.859  alpha  0.000
      colour red  0.729  green  0.706  blue  0.694  alpha  0.000]
      [0.444 0.615   colour red  0.729  green  0.706  blue  0.694  alpha  0.000
      colour red  0.588  green  0.592  blue  0.635  alpha  0.000]
      [0.615 0.803   colour red  0.588  green  0.592  blue  0.635  alpha  0.000
      colour red  0.608  green  0.616  blue  0.659  alpha  0.000]
      [0.803 1.001   colour red  0.608  green  0.616  blue  0.659  alpha  0.000
      colour red  0.894  green  0.886  blue  0.886  alpha  0.000]
   }
}

//------------ Moss Greens & Tan 
#declare Grnt25 = texture {
   marble
   turbulence 0.7
   colour_map {
      [0.000 0.168   colour red  0.824  green  0.725  blue  0.584  alpha  0.000
      colour red  0.514  green  0.584  blue  0.533  alpha  0.000]
      [0.168 0.301   colour red  0.514  green  0.584  blue  0.533  alpha  0.000
      colour red  0.298  green  0.376  blue  0.318  alpha  0.000]
      [0.301 0.398   colour red  0.298  green  0.376  blue  0.318  alpha  0.000
      colour red  0.263  green  0.337  blue  0.282  alpha  0.000]
      [0.398 0.558   colour red  0.263  green  0.337  blue  0.282  alpha  0.000
      colour red  0.431  green  0.506  blue  0.451  alpha  0.000]
      [0.558 0.655   colour red  0.431  green  0.506  blue  0.451  alpha  0.000
      colour red  0.529  green  0.631  blue  0.471  alpha  0.000]
      [0.655 0.735   colour red  0.529  green  0.631  blue  0.471  alpha  0.000
      colour red  0.333  green  0.376  blue  0.318  alpha  0.000]
      [0.735 0.823   colour red  0.333  green  0.376  blue  0.318  alpha  0.000
      colour red  0.298  green  0.376  blue  0.318  alpha  0.000]
      [0.823 0.876   colour red  0.298  green  0.376  blue  0.318  alpha  0.000
      colour red  0.416  green  0.376  blue  0.318  alpha  0.000]
      [0.876 0.929   colour red  0.416  green  0.376  blue  0.318  alpha  0.000
      colour red  0.416  green  0.376  blue  0.318  alpha  0.000]
      [0.929 1.001   colour red  0.416  green  0.376  blue  0.318  alpha  0.000
      colour red  0.824  green  0.725  blue  0.584  alpha  0.000]
   }
}

//---------- Salmon with thin Green Viens
#declare Grnt26 = texture {
   granite
   colour_map {
      [0.000 0.241   colour red  0.973  green  0.973  blue  0.976  alpha  0.000
      colour red  0.973  green  0.973  blue  0.976  alpha  0.000]
      [0.241 0.284   colour red  0.973  green  0.973  blue  0.976  alpha  0.000
      colour red  0.600  green  0.741  blue  0.608  alpha  0.000]
      [0.284 0.336   colour red  0.600  green  0.741  blue  0.608  alpha  0.000
      colour red  0.820  green  0.643  blue  0.537  alpha  0.000]
      [0.336 0.474   colour red  0.820  green  0.643  blue  0.537  alpha  0.000
      colour red  0.886  green  0.780  blue  0.714  alpha  0.000]
      [0.474 0.810   colour red  0.886  green  0.780  blue  0.714  alpha  0.000
      colour red  0.996  green  0.643  blue  0.537  alpha  0.000]
      [0.810 0.836   colour red  0.996  green  0.643  blue  0.537  alpha  0.000
      colour red  0.973  green  0.973  blue  0.976  alpha  0.000]
      [0.836 1.001   colour red  0.973  green  0.973  blue  0.976  alpha  0.000
      colour red  0.973  green  0.973  blue  0.976  alpha  0.000]
   }
   0.02
}

//------ Dark Green & Browns
#declare Grnt27 = texture {
   granite
   colour_map {
      [0.000 0.043   colour red  0.773  green  0.647  blue  0.569  alpha  0.000
      colour red  0.431  green  0.322  blue  0.227  alpha  0.000]
      [0.043 0.113   colour red  0.431  green  0.322  blue  0.227  alpha  0.000
      colour red  0.278  green  0.282  blue  0.216  alpha  0.000]
      [0.113 0.304   colour red  0.278  green  0.282  blue  0.216  alpha  0.000
      colour red  0.278  green  0.282  blue  0.216  alpha  0.000]
      [0.304 0.426   colour red  0.278  green  0.282  blue  0.216  alpha  0.000
      colour red  0.459  green  0.341  blue  0.243  alpha  0.000]
      [0.426 0.843   colour red  0.459  green  0.341  blue  0.243  alpha  0.000
      colour red  0.459  green  0.341  blue  0.243  alpha  0.000]
      [0.843 0.878   colour red  0.459  green  0.341  blue  0.243  alpha  0.000
      colour red  0.459  green  0.341  blue  0.243  alpha  0.000]
      [0.878 0.983   colour red  0.459  green  0.341  blue  0.243  alpha  0.000
      colour red  0.278  green  0.282  blue  0.216  alpha  0.000]
      [0.983 1.001   colour red  0.278  green  0.282  blue  0.216  alpha  0.000
      colour red  0.773  green  0.647  blue  0.569  alpha  0.000]
   }
}

//------- Red Swirl
#declare Grnt28 = texture {
   marble
   turbulence 0.7
   colour_map {
      [0.000 0.155   colour red  0.686  green  0.235  blue  0.282  alpha  0.000
      colour red  0.686  green  0.235  blue  0.282  alpha  0.000]
      [0.155 0.328   colour red  0.686  green  0.235  blue  0.282  alpha  0.000
      colour red  0.494  green  0.243  blue  0.294  alpha  0.000]
      [0.328 0.474   colour red  0.494  green  0.243  blue  0.294  alpha  0.000
      colour red  0.769  green  0.329  blue  0.373  alpha  0.000]
      [0.474 0.647   colour red  0.769  green  0.329  blue  0.373  alpha  0.000
      colour red  0.769  green  0.329  blue  0.373  alpha  0.000]
      [0.647 0.810   colour red  0.769  green  0.329  blue  0.373  alpha  0.000
      colour red  0.686  green  0.235  blue  0.282  alpha  0.000]
      [0.810 0.922   colour red  0.686  green  0.235  blue  0.282  alpha  0.000
      colour red  0.792  green  0.388  blue  0.427  alpha  0.000]
      [0.922 1.001   colour red  0.792  green  0.388  blue  0.427  alpha  0.000
      colour red  0.686  green  0.235  blue  0.282  alpha  0.000]
   }
   0.03
}

//-------- White Tan & thin Reds
#declare Grnt29 = texture {
   marble
   turbulence 0.5
   colour_map {
      [0.000 0.053   colour red  0.784  green  0.627  blue  0.522  alpha  0.000
      colour red  0.784  green  0.627  blue  0.624  alpha  0.000]
      [0.053 0.263   colour red  0.784  green  0.627  blue  0.624  alpha  0.000
      colour red  0.824  green  0.557  blue  0.376  alpha  0.000]
      [0.263 0.281   colour red  0.824  green  0.557  blue  0.376  alpha  0.000
      colour red  0.643  green  0.380  blue  0.376  alpha  0.000]
      [0.281 0.325   colour red  0.643  green  0.380  blue  0.376  alpha  0.000
      colour red  0.839  green  0.722  blue  0.722  alpha  0.000]
      [0.325 0.711   colour red  0.839  green  0.722  blue  0.722  alpha  0.000
      colour red  0.784  green  0.627  blue  0.522  alpha  0.000]
      [0.711 0.798   colour red  0.784  green  0.627  blue  0.522  alpha  0.000
      colour red  0.769  green  0.380  blue  0.376  alpha  0.000]
      [0.798 0.895   colour red  0.769  green  0.380  blue  0.376  alpha  0.000
      colour red  0.824  green  0.557  blue  0.376  alpha  0.000]
      [0.895 0.982   colour red  0.824  green  0.557  blue  0.376  alpha  0.000
      colour red  0.784  green  0.627  blue  0.522  alpha  0.000]
      [0.982 1.001   colour red  0.784  green  0.627  blue  0.522  alpha  0.000
      colour red  0.784  green  0.627  blue  0.522  alpha  0.000]
   }
}

//***************************************************************
//------------ start of textures with alpha

//----- Translucent Grnt0
#declare Grnt0a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.153   colour red  0.729  green  0.502  blue  0.451  alpha  0.306 
      colour red  0.769  green  0.686  blue  0.592  alpha  0.792]
      [0.153 0.398   colour red  0.769  green  0.686  blue  0.592  alpha  0.792 
      colour red  0.843  green  0.753  blue  0.718  alpha  0.396]
      [0.398 0.559   colour red  0.843  green  0.753  blue  0.718  alpha  0.396 
      colour red  0.780  green  0.667  blue  0.561  alpha  0.976]
      [0.559 0.729   colour red  0.780  green  0.667  blue  0.561  alpha  0.976 
      colour red  0.741  green  0.659  blue  0.576  alpha  0.820]
      [0.729 1.001   colour red  0.741  green  0.659  blue  0.576  alpha  0.820 
      colour red  0.729  green  0.502  blue  0.451  alpha  0.306]
   }
}

//----- Translucent Grnt1
#declare Grnt1a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.212   colour red  0.898  green  0.898  blue  0.851  alpha  0.306 
      colour red  0.969  green  0.980  blue  0.875  alpha  0.792]
      [0.212 0.424   colour red  0.969  green  0.980  blue  0.875  alpha  0.792 
      colour red  0.859  green  0.859  blue  0.859  alpha  0.396]
      [0.424 0.627   colour red  0.859  green  0.859  blue  0.859  alpha  0.396 
      colour red  0.992  green  0.922  blue  0.659  alpha  0.976]
      [0.627 0.881   colour red  0.992  green  0.922  blue  0.659  alpha  0.976 
      colour red  0.937  green  0.965  blue  0.902  alpha  0.820]
      [0.881 1.001   colour red  0.937  green  0.965  blue  0.902  alpha  0.820 
      colour red  0.898  green  0.898  blue  0.851  alpha  0.306]
   }
}

//-----Translucent Grnt2
#declare Grnt2a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.144   colour red  0.863  green  0.757  blue  0.596  alpha  0.596
      colour red  0.925  green  0.792  blue  0.714  alpha  0.349]
      [0.144 0.288   colour red  0.925  green  0.792  blue  0.714  alpha  0.349
      colour red  0.871  green  0.702  blue  0.659  alpha  0.784]
      [0.288 0.644   colour red  0.871  green  0.702  blue  0.659  alpha  0.784
      colour red  0.992  green  0.922  blue  0.659  alpha  0.498]
      [0.644 0.983   colour red  0.992  green  0.922  blue  0.659  alpha  0.498
      colour red  0.902  green  0.812  blue  0.714  alpha  0.722]
      [0.983 1.001   colour red  0.902  green  0.812  blue  0.714  alpha  0.722
      colour red  0.863  green  0.757  blue  0.596  alpha  0.596]
   }
}

//-----Translucent Grnt3
#declare Grnt3a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.153   colour red  0.831  green  0.631  blue  0.569  alpha  0.447
      colour red  0.925  green  0.831  blue  0.714  alpha  0.678]
      [0.153 0.297   colour red  0.925  green  0.831  blue  0.714  alpha  0.678
      colour red  0.871  green  0.702  blue  0.659  alpha  0.475]
      [0.297 0.441   colour red  0.871  green  0.702  blue  0.659  alpha  0.475
      colour red  0.831  green  0.631  blue  0.569  alpha  0.918]
      [0.441 0.763   colour red  0.831  green  0.631  blue  0.569  alpha  0.918
      colour red  0.937  green  0.882  blue  0.820  alpha  0.655]
      [0.763 1.001   colour red  0.937  green  0.882  blue  0.820  alpha  0.655
      colour red  0.831  green  0.631  blue  0.569  alpha  0.447]
   }
}

//-----Translucent Grnt4
#declare Grnt4a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.144   colour red  0.804  green  0.569  blue  0.494  alpha  0.569
      colour red  0.816  green  0.725  blue  0.537  alpha  0.467]
      [0.144 0.449   colour red  0.816  green  0.725  blue  0.537  alpha  0.467
      colour red  0.820  green  0.580  blue  0.522  alpha  0.584]
      [0.449 0.568   colour red  0.820  green  0.580  blue  0.522  alpha  0.584
      colour red  0.882  green  0.725  blue  0.537  alpha  0.871]
      [0.568 0.754   colour red  0.882  green  0.725  blue  0.537  alpha  0.871
      colour red  0.855  green  0.729  blue  0.584  alpha  0.816]
      [0.754 1.001   colour red  0.855  green  0.729  blue  0.584  alpha  0.816
      colour red  0.804  green  0.569  blue  0.494  alpha  0.569]
   }
}

//-----Translucent Grnt4
#declare Grnt5a = texture {
   granite
   turbulence 0.5
   colour_map {
      [0.000 0.178   colour red  0.804  green  0.569  blue  0.494  alpha  0.569 
      colour red  0.855  green  0.729  blue  0.584  alpha  0.467]
      [0.178 0.356   colour red  0.855  green  0.729  blue  0.584  alpha  0.467 
      colour red  0.667  green  0.502  blue  0.478  alpha  0.584]
      [0.356 0.525   colour red  0.667  green  0.502  blue  0.478  alpha  0.584 
      colour red  0.859  green  0.624  blue  0.545  alpha  0.871]
      [0.525 0.729   colour red  0.859  green  0.624  blue  0.545  alpha  0.871 
      colour red  0.855  green  0.729  blue  0.584  alpha  0.816]
      [0.729 1.001   colour red  0.855  green  0.729  blue  0.584  alpha  0.816 
      colour red  0.804  green  0.569  blue  0.494  alpha  0.569]
   }
}

//-----Translucent Grnt6
#declare Grnt6a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.263   colour red  0.545  green  0.380  blue  0.345  alpha  0.733
      colour red  0.588  green  0.475  blue  0.333  alpha  0.741]
      [0.263 0.432   colour red  0.588  green  0.475  blue  0.333  alpha  0.741
      colour red  0.675  green  0.478  blue  0.404  alpha  0.545]
      [0.432 0.551   colour red  0.675  green  0.478  blue  0.404  alpha  0.545
      colour red  0.757  green  0.635  blue  0.522  alpha  0.384]
      [0.551 0.720   colour red  0.757  green  0.635  blue  0.522  alpha  0.384
      colour red  0.659  green  0.549  blue  0.443  alpha  0.675]
      [0.720 1.001   colour red  0.659  green  0.549  blue  0.443  alpha  0.675
      colour red  0.545  green  0.380  blue  0.345  alpha  0.733]
   }
}

//-----Translucent Grnt7
#declare Grnt7a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.119   colour red  0.439  green  0.310  blue  0.282  alpha  0.631
      colour red  0.463  green  0.369  blue  0.259  alpha  0.847]
      [0.119 0.322   colour red  0.463  green  0.369  blue  0.259  alpha  0.847
      colour red  0.541  green  0.369  blue  0.298  alpha  0.549]
      [0.322 0.449   colour red  0.541  green  0.369  blue  0.298  alpha  0.549
      colour red  0.573  green  0.424  blue  0.286  alpha  0.965]
      [0.449 0.729   colour red  0.573  green  0.424  blue  0.286  alpha  0.965
      colour red  0.494  green  0.396  blue  0.306  alpha  0.741]
      [0.729 1.001   colour red  0.494  green  0.396  blue  0.306  alpha  0.741
      colour red  0.439  green  0.310  blue  0.282  alpha  0.631]
   }
}

//-----Aqua Tints 
#declare Grnt8a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.119   colour red  0.310  green  0.384  blue  0.420  alpha  0.631
      colour red  0.322  green  0.369  blue  0.416  alpha  0.847]
      [0.119 0.322   colour red  0.322  green  0.369  blue  0.416  alpha  0.847
      colour red  0.424  green  0.369  blue  0.420  alpha  0.549]
      [0.322 0.449   colour red  0.424  green  0.369  blue  0.420  alpha  0.549
      colour red  0.373  green  0.424  blue  0.518  alpha  0.965]
      [0.449 0.729   colour red  0.373  green  0.424  blue  0.518  alpha  0.965
      colour red  0.482  green  0.573  blue  0.533  alpha  0.741]
      [0.729 1.001   colour red  0.482  green  0.573  blue  0.533  alpha  0.741
      colour red  0.310  green  0.384  blue  0.420  alpha  0.631]
   }
}

//-----Alpha Creams With Cracks
#declare Grnt9a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.216   colour red  0.812  green  0.812  blue  0.812  alpha  0.835
      colour red  0.745  green  0.843  blue  0.835  alpha  0.847]
      [0.216 0.241   colour red  0.745  green  0.843  blue  0.835  alpha  0.847
      colour red  0.404  green  0.337  blue  0.337  alpha  0.463]
      [0.241 0.267   colour red  0.404  green  0.337  blue  0.337  alpha  0.463
      colour red  0.773  green  0.729  blue  0.745  alpha  0.622]
      [0.267 0.759   colour red  0.773  green  0.729  blue  0.745  alpha  0.622
      colour red  0.914  green  0.843  blue  0.725  alpha  0.651]
      [0.759 0.784   colour red  0.914  green  0.843  blue  0.725  alpha  0.651
      colour red  0.153  green  0.133  blue  0.208  alpha  0.437]
      [0.784 0.810   colour red  0.153  green  0.133  blue  0.208  alpha  0.437
      colour red  0.812  green  0.812  blue  0.812  alpha  0.835]
      [0.810 1.001   colour red  0.812  green  0.812  blue  0.812  alpha  0.835
      colour red  0.812  green  0.812  blue  0.812  alpha  0.835]
   }
}

//--------- Alpha Cream Rose & light yellow
#declare Grnt10a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.179   colour red  0.843  green  0.655  blue  0.655  alpha  0.455
      colour red  0.886  green  0.769  blue  0.627  alpha  0.608]
      [0.179 0.368   colour red  0.886  green  0.769  blue  0.627  alpha  0.608
      colour red  0.906  green  0.820  blue  0.714  alpha  0.392]
      [0.368 0.538   colour red  0.906  green  0.820  blue  0.714  alpha  0.392
      colour red  0.851  green  0.671  blue  0.671  alpha  0.659]
      [0.538 0.744   colour red  0.851  green  0.671  blue  0.671  alpha  0.659
      colour red  0.890  green  0.792  blue  0.675  alpha  0.392]
      [0.744 0.983   colour red  0.890  green  0.792  blue  0.675  alpha  0.392
      colour red  0.827  green  0.612  blue  0.612  alpha  0.706]
      [0.983 1.001   colour red  0.827  green  0.612  blue  0.612  alpha  0.706
      colour red  0.843  green  0.655  blue  0.655  alpha  0.455]
   }
}

//--------- Alpha Light Grays
#declare Grnt11a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.894  green  0.886  blue  0.886  alpha  0.659
      colour red  0.745  green  0.745  blue  0.753  alpha  0.584]
      [0.154 0.308   colour red  0.745  green  0.745  blue  0.753  alpha  0.584
      colour red  0.902  green  0.902  blue  0.859  alpha  0.780]
      [0.308 0.444   colour red  0.902  green  0.902  blue  0.859  alpha  0.780
      colour red  0.729  green  0.706  blue  0.694  alpha  0.686]
      [0.444 0.615   colour red  0.729  green  0.706  blue  0.694  alpha  0.686
      colour red  0.588  green  0.592  blue  0.635  alpha  0.424]
      [0.615 0.803   colour red  0.588  green  0.592  blue  0.635  alpha  0.424
      colour red  0.608  green  0.616  blue  0.659  alpha  0.761]
      [0.803 1.001   colour red  0.608  green  0.616  blue  0.659  alpha  0.761
      colour red  0.894  green  0.886  blue  0.886  alpha  0.659]
   }
}

//--------- Alpha Creams & Tans
#declare Grnt12a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.890  green  0.690  blue  0.690  alpha  0.659
      colour red  0.996  green  0.835  blue  0.737  alpha  0.659]
      [0.154 0.308   colour red  0.996  green  0.835  blue  0.737  alpha  0.659
      colour red  0.745  green  0.635  blue  0.651  alpha  0.780]
      [0.308 0.444   colour red  0.745  green  0.635  blue  0.651  alpha  0.780
      colour red  0.733  green  0.596  blue  0.557  alpha  0.686]
      [0.444 0.615   colour red  0.733  green  0.596  blue  0.557  alpha  0.686
      colour red  0.996  green  0.835  blue  0.737  alpha  0.659]
      [0.615 0.803   colour red  0.996  green  0.835  blue  0.737  alpha  0.659
      colour red  0.765  green  0.616  blue  0.659  alpha  0.761]
      [0.803 1.001   colour red  0.765  green  0.616  blue  0.659  alpha  0.761
      colour red  0.890  green  0.690  blue  0.690  alpha  0.659]
   }
}

//--------- Alpha Creams & Grays
#declare Grnt13a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.800  green  0.651  blue  0.557  alpha  0.000
      colour red  0.996  green  0.835  blue  0.737  alpha  0.608]
      [0.154 0.308   colour red  0.996  green  0.835  blue  0.737  alpha  0.608
      colour red  0.800  green  0.651  blue  0.557  alpha  0.635]
      [0.308 0.444   colour red  0.800  green  0.651  blue  0.557  alpha  0.635
      colour red  0.694  green  0.624  blue  0.604  alpha  0.294]
      [0.444 0.615   colour red  0.694  green  0.624  blue  0.604  alpha  0.294
      colour red  0.800  green  0.651  blue  0.557  alpha  0.816]
      [0.615 0.812   colour red  0.800  green  0.651  blue  0.557  alpha  0.816
      colour red  0.725  green  0.655  blue  0.651  alpha  0.957]
      [0.812 1.001   colour red  0.725  green  0.655  blue  0.651  alpha  0.957
      colour red  0.800  green  0.651  blue  0.557  alpha  0.000]
   }
}

//--------- Cream Rose & light moss
#declare Grnt14a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.996  green  0.969  blue  0.800  alpha  0.373
      colour red  0.996  green  0.682  blue  0.604  alpha  0.412]
      [0.154 0.308   colour red  0.996  green  0.682  blue  0.604  alpha  0.412
      colour red  0.906  green  0.820  blue  0.714  alpha  0.616]
      [0.308 0.444   colour red  0.906  green  0.820  blue  0.714  alpha  0.616
      colour red  0.816  green  0.631  blue  0.537  alpha  0.443]
      [0.444 0.615   colour red  0.816  green  0.631  blue  0.537  alpha  0.443
      colour red  0.890  green  0.792  blue  0.675  alpha  0.745]
      [0.615 0.812   colour red  0.890  green  0.792  blue  0.675  alpha  0.745
      colour red  0.973  green  0.627  blue  0.627  alpha  0.600]
      [0.812 1.001   colour red  0.973  green  0.627  blue  0.627  alpha  0.600
      colour red  0.996  green  0.969  blue  0.800  alpha  0.373]
   }
}

//--------- Alpha Sand & light Orange
#declare Grnt15a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.154   colour red  0.996  green  0.824  blue  0.780  alpha  0.412
      colour red  0.996  green  0.698  blue  0.624  alpha  0.412]
      [0.154 0.308   colour red  0.996  green  0.698  blue  0.624  alpha  0.412
      colour red  0.906  green  0.675  blue  0.553  alpha  0.616]
      [0.308 0.444   colour red  0.906  green  0.675  blue  0.553  alpha  0.616
      colour red  0.996  green  0.682  blue  0.604  alpha  0.412]
      [0.444 0.615   colour red  0.996  green  0.682  blue  0.604  alpha  0.412
      colour red  0.996  green  0.824  blue  0.780  alpha  0.412]
      [0.615 0.812   colour red  0.996  green  0.824  blue  0.780  alpha  0.412
      colour red  0.973  green  0.627  blue  0.627  alpha  0.600]
      [0.812 1.001   colour red  0.973  green  0.627  blue  0.627  alpha  0.600
      colour red  0.996  green  0.824  blue  0.780  alpha  0.412]
   }
}

//--------- Cream Rose & light moss
#declare Grnt16a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.078   colour red  0.769  green  0.722  blue  0.690  alpha  0.180
      colour red  0.745  green  0.690  blue  0.655  alpha  1.000]
      [0.078 0.96   colour red  0.745  green  0.690  blue  0.655  alpha  1.000
      colour red  0.839  green  0.804  blue  0.780  alpha  1.000]
      [0.96 1.001   colour red  0.839  green  0.804  blue  0.780  alpha  0.278
      colour red  0.769  green  0.722  blue  0.690  alpha  0.180]
   }
}

#declare Grnt17a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.034   colour red  0.027  green  0.012  blue  0.012  alpha  0.000
      colour red  0.851  green  0.812  blue  0.741  alpha  0.235]
      [0.034 0.342   colour red  0.851  green  0.812  blue  0.741  alpha  0.235
      colour red  0.792  green  0.694  blue  0.690  alpha  0.839]
      [0.342 0.462   colour red  0.792  green  0.694  blue  0.690  alpha  0.839
      colour red  0.631  green  0.506  blue  0.471  alpha  0.608]
      [0.462 0.632   colour red  0.631  green  0.506  blue  0.471  alpha  0.608
      colour red  0.851  green  0.812  blue  0.741  alpha  0.922]
      [0.632 0.983   colour red  0.851  green  0.812  blue  0.741  alpha  0.922
      colour red  0.647  green  0.655  blue  0.655  alpha  0.282]
      [0.983 1.001   colour red  0.647  green  0.655  blue  0.655  alpha  0.282
      colour red  0.027  green  0.012  blue  0.012  alpha  0.000]
   }
}

#declare Grnt18a = texture {
   granite
   turbulence 0.6
   colour_map {
      [0.000 0.128   colour red  0.820  green  0.580  blue  0.580  alpha  0.000
      colour red  0.851  green  0.812  blue  0.741  alpha  0.235]
      [0.128 0.282   colour red  0.851  green  0.812  blue  0.741  alpha  0.235
      colour red  0.792  green  0.694  blue  0.690  alpha  0.282]
      [0.282 0.393   colour red  0.792  green  0.694  blue  0.690  alpha  0.282
      colour red  0.647  green  0.655  blue  0.655  alpha  0.133]
      [0.393 0.590   colour red  0.647  green  0.655  blue  0.655  alpha  0.133
      colour red  0.851  green  0.812  blue  0.741  alpha  0.333]
      [0.590 0.983   colour red  0.851  green  0.812  blue  0.741  alpha  0.333
      colour red  0.647  green  0.655  blue  0.655  alpha  0.282]
      [0.983 1.001   colour red  0.647  green  0.655  blue  0.655  alpha  0.282
      colour red  0.820  green  0.580  blue  0.580  alpha  0.000]
   }
}

//--------- Gray Marble with White feather Viens with Alpha
#declare Grnt19a = texture {
   granite 
   turbulence 0.0
   color_map {
      [0.0 0.3 color White alpha 0.0 color DimGray alpha 0.5]
      [0.2 0.4 color DimGray alpha 0.5 color DimGray alpha 0.8]
      [0.4 1.0 color DimGray alpha 0.8 color DimGray alpha 0.9]
   }
   0.02
}

//--------- White Feature Viens 
#declare Grnt20a = texture {
   granite 
   turbulence 0.0
   color_map {
      [0.0 0.2 color White alpha 0.0 color White alpha 0.7]
      [0.2 0.3 color White alpha 0.7 color Clear ]
      [0.3 1.0 color Clear color Clear]
   }
   0.02
}
//--------- Thinner White Feature Viens 
#declare Grnt21a = texture {
   granite 
   turbulence 0.0
   color_map {
      [0.0 0.2 color White alpha 0.4 color White alpha 0.8]
      [0.2 0.3 color White alpha 0.8 color Clear ]
      [0.3 1.0 color Clear color Clear]
   }
   0.02
}

#declare Grnt22a = texture {
   granite
   turbulence 0.5
   colour_map {
      [0.000 0.175   colour red  1.000  green  0.718  blue  0.541  alpha  0.890
      colour red  0.843  green  0.678  blue  0.655  alpha  0.753]
      [0.175 0.228   colour red  0.843  green  0.678  blue  0.655  alpha  0.753
      colour red  0.906  green  0.831  blue  0.773  alpha  0.98]
      [0.228 0.386   colour red  0.906  green  0.831  blue  0.773  alpha  0.698
      colour red  0.992  green  0.718  blue  0.545  alpha  0.794]
      [0.386 0.412   colour red  0.992  green  0.718  blue  0.545  alpha  0.794
      colour red  0.333  green  0.188  blue  0.067  alpha  0.784]
      [0.412 0.439   colour red  0.333  green  0.188  blue  0.067  alpha  0.784
      colour red  0.925  green  0.557  blue  0.514  alpha  0.778]
      [0.439 0.684   colour red  0.925  green  0.557  blue  0.514  alpha  0.678
      colour red  0.984  green  0.780  blue  0.655  alpha  0.696]
      [0.684 0.781   colour red  0.984  green  0.780  blue  0.655  alpha  0.696
      colour red  0.965  green  0.847  blue  0.675  alpha  0.880]
      [0.781 0.982   colour red  0.965  green  0.847  blue  0.675  alpha  0.880
      colour red  1.000  green  0.718  blue  0.541  alpha  0.990]
      [0.982 1.001   colour red  1.000  green  0.718  blue  0.541  alpha  0.890
      colour red  1.000  green  0.718  blue  0.541  alpha  0.890]
   }
}

//---------- Transparent Green Moss Colors
#declare Grnt23a = texture {
   granite
   colour_map {
      [0.000 0.168   colour red  0.824  green  0.725  blue  0.584  alpha  0.600
      colour red  0.514  green  0.584  blue  0.533  alpha  0.600]
      [0.168 0.301   colour red  0.514  green  0.584  blue  0.533  alpha  0.600
      colour red  0.298  green  0.376  blue  0.318  alpha  0.600]
      [0.301 0.398   colour red  0.298  green  0.376  blue  0.318  alpha  0.600
      colour red  0.263  green  0.337  blue  0.282  alpha  0.700]
      [0.398 0.558   colour red  0.263  green  0.337  blue  0.282  alpha  0.700
      colour red  0.431  green  0.506  blue  0.451  alpha  0.600]
      [0.558 0.655   colour red  0.431  green  0.506  blue  0.451  alpha  0.600
      colour red  0.529  green  0.631  blue  0.471  alpha  0.500]
      [0.655 0.735   colour red  0.529  green  0.631  blue  0.471  alpha  0.500
      colour red  0.333  green  0.376  blue  0.318  alpha  0.700]
      [0.735 0.823   colour red  0.333  green  0.376  blue  0.318  alpha  0.700
      colour red  0.298  green  0.376  blue  0.318  alpha  0.600]
      [0.823 0.876   colour red  0.298  green  0.376  blue  0.318  alpha  0.600
      colour red  0.416  green  0.376  blue  0.318  alpha  0.500]
      [0.876 0.929   colour red  0.416  green  0.376  blue  0.318  alpha  0.500
      colour red  0.416  green  0.376  blue  0.318  alpha  0.600]
      [0.929 1.001   colour red  0.416  green  0.376  blue  0.318  alpha  0.600
      colour red  0.824  green  0.725  blue  0.584  alpha  0.700]
   }
}

#declare Grnt24a = texture {
   granite
   turbulence 0.5
   colour_map {
      [0.000 0.053   colour red  0.784  green  0.627  blue  0.522  alpha  0.500
      colour red  0.784  green  0.627  blue  0.624  alpha  0.500]
      [0.053 0.263   colour red  0.784  green  0.627  blue  0.624  alpha  0.500
      colour red  0.824  green  0.557  blue  0.376  alpha  0.500]
      [0.263 0.281   colour red  0.824  green  0.557  blue  0.376  alpha  0.500
      colour red  0.643  green  0.380  blue  0.376  alpha  0.500]
      [0.281 0.325   colour red  0.643  green  0.380  blue  0.376  alpha  0.500
      colour red  0.839  green  0.722  blue  0.722  alpha  0.500]
      [0.325 0.711   colour red  0.839  green  0.722  blue  0.722  alpha  0.500
      colour red  0.784  green  0.627  blue  0.522  alpha  0.500]
      [0.711 0.798   colour red  0.784  green  0.627  blue  0.522  alpha  0.500
      colour red  0.769  green  0.380  blue  0.376  alpha  0.500]
      [0.798 0.895   colour red  0.769  green  0.380  blue  0.376  alpha  0.500
      colour red  0.824  green  0.557  blue  0.376  alpha  0.500]
      [0.895 0.982   colour red  0.824  green  0.557  blue  0.376  alpha  0.500
      colour red  0.784  green  0.627  blue  0.522  alpha  0.500]
      [0.982 1.001   colour red  0.784  green  0.627  blue  0.522  alpha  0.500
      colour red  0.784  green  0.627  blue  0.522  alpha  0.500]
   }
}

/*--------------------Crack & OverTint /Red---------------------*/
#declare Crack1 = texture {
   marble
   turbulence 0.85
   color_map {
      [0.0 0.04 color Black alpha 0.6 color Black alpha 1.0]
      [0.04 0.97 color Scarlet alpha 0.80 color DimGray alpha 0.90] 
      [0.97 1.001 color Black alpha 0.9 color Black alpha 1.0]
   }
}       

//-----Alpha  Dark Cracks
#declare Crack2 = texture {
   granite
   turbulence 0.8
   colour_map {
      [0.0 0.5   color Clear color Clear]
      [0.5 0.54   color Clear color Black]
      [0.54 1.0 color Clear color Clear]
   }
}

//---------- Overtint Green with Black Cracks
#declare Crack3 = texture {
   marble
   turbulence 0.85
   color_map {
      [0.0 0.04 color Black alpha 0.6 color Black alpha 1.0]
      [0.04 0.97 color DarkGreen alpha 0.80 color DarkGreen alpha 0.90] 
      [0.97 1.001 color Black alpha 0.9 color Black alpha 1.0]
   }
}       

//--------- Overtint with White Crack
#declare Crack4 = texture {
   marble
   turbulence 0.85
   color_map {
      [0.0 0.03 color White alpha 0.3 color White alpha 1.0]
      [0.03 0.97 color Black alpha 0.70 color DimGray alpha 0.90] 
      [0.97 1.001 color White alpha 0.9 color White alpha 1.0]
   }
}       


//-----------------  complete texture { statements
//                   Scaled to cover nicely over a 2 unit Cube

//------------- Deep Rose & Green Marble with large White Swirls
#declare Stone1 =
texture { Grnt7 scale <2 3 2> rotate <0 0 40> }
texture { Grnt0a scale <2 3 2> rotate <0 0 -30>  phong 1.0 phong_size 90
}

//------------- Light Greenish Tan Marble with Agate style veining
#declare Stone2 =
texture { Grnt0 scale <2 3 2> rotate <0 0 40> }
texture { Grnt7a scale <1.3 2 1.3> rotate <0 0 -30> rotate <40 0 0> ambient 0.2 phong 1.0 phong_size 90 }

//------------- Rose & Yellow Marble with fog white veining
#declare Stone3 =
texture { Grnt5 scale <2 3 2> rotate <0 0 40> }
texture { Grnt0a scale <2 3 2> rotate <0 0 -30>  phong 1.0 phong_size 90
}

//------------- Tan Marble with Rose patches
#declare Stone4 =
texture { Grnt6 scale <1.5 3 2> rotate <0 0 40> diffuse 0.5 }
texture { Grnt10a scale <1 3 2> rotate <0 0 -30>  phong 1.0 phong_size 90
}

//------------- White Cream Marble with Pink veining
#declare Stone5 =
texture { Grnt12 scale <2 3 2> rotate <0 0 40> }
texture { Grnt17a scale <2 3 2> rotate <0 0 -30>  }
texture { Crack1 scale <1 2 1.4> rotate <10 0 -20> ambient 0.2 phong 1.0 phong_size 90 }

//------------- Rose & Yellow Cream Marble
#declare Stone6 =
texture { Grnt18 scale <1.5 3 3> rotate <0 0 40> }
texture { Grnt19 scale <2 4 1.3> rotate <0 0 30> phong 1.0 phong_size 90 }
texture { Crack1 scale <1 2 1.4> rotate <10 0 -20> ambient 0.2 phong 1.0 phong_size 90 }

//------------- Light Coffee Marble with darker patches
#declare Stone7 =
texture { color Salmon }
texture { Grnt6a scale <1 3 2> rotate <0 0 40> }
texture { Grnt9a scale <3.5 5 4> rotate <0 0 60>  phong 1.0 phong_size 90 }

//------------- Gray Granite with white patches
#declare Stone8 =
texture { color White ambient 0.4 0.06 diffuse 0.7 }
texture { Grnt0a scale <2 3 2> rotate <0 0 -30>   }
texture { Grnt9a scale <5 3 4> rotate <0 0 40>  ambient 0.2 diffuse 0.5 phong 1.0 phong_size 90 }

//------------- White & Light Blue Marble with light violets
#declare Stone9 =
texture { Grnt9 scale <1.2 2.4 1.2> rotate <0 0 -30> rotate <40 0 0> }
texture { Crack1 scale <1 2 1.4> rotate <10 0 -20> ambient 0.2 phong 1.0 phong_size 90 }

//------------- Dark Brown & Tan swirl Granite with gray undertones
#declare Stone10 =
texture { color Black }
texture { Grnt17a scale <3 6 2> rotate <0 0 50> }
texture { Grnt3a scale <1 2 1> rotate <0 0 -50>  phong 1.0 phong_size 90 }

//------------- Rose & White Marble with dark tan swirl
#declare Stone11 =
texture { color Black }
texture { Grnt15a scale <1.2 3 1.5> rotate <70 0 30> 0.03 }
texture { Grnt2a scale <3 3 4> rotate <0 0 40>  }
texture { Crack1 scale <1 2 1.4> rotate <10 0 -20> phong 1.0 phong_size 90 }

//------------- White & Pinkish Tan Marble
#declare Stone12 =
texture { Grnt23 scale <1 5 1> rotate <0 0 50> ambient 0.2 0.03 }
texture { Grnt0a scale <1 3 2> rotate <0 0 -30>  phong 1.0 phong_size 90
}

//------------- Medium Gray Blue Marble
#declare Stone13 =
texture { Grnt24 scale <2 5 2> rotate <0 0 50> ambient 0.2 0.03 }
texture { Grnt8a scale <1 3 2> rotate <0 0 -30>  phong 1.0 phong_size 90
}

//------------- Tan & Olive Marble with gray white veins
#declare Stone14 =
texture { Grnt6 scale <2 3 2> rotate <0 0 -30> ambient 0.2 diffuse 0.9 0.03 }
texture { Grnt19a scale <1 3 1> rotate <0 0 40>  phong 1.0 phong_size 90 }

//------------- Deep Gray Marble with white veining
#declare Stone15 =
texture { Grnt20 scale <1 2 2> rotate <0 0 -30> ambient 0.2 diffuse 0.9 0.03 }
texture { Grnt8a scale <1 2 1> rotate <0 0 40>  phong 1.0 phong_size 90 }

//------------- Peach & Yellow Marble with white veining
#declare Stone16 =
texture { Grnt18 scale <1.3 2 2> rotate <0 0 -30> ambient 0.2 diffuse 0.9 0.03 }
texture { Grnt19 scale <2 4 2> rotate <0 0 -30> ambient 0.2 diffuse 0.9 0.03 }
texture { Grnt20a scale <1 2 1> rotate <0 0 40>  phong 1.0 phong_size 90 }

//------------- White Marble with gray veining
#declare Stone17 =
texture { Grnt20 scale <1 2 2> rotate <0 0 -30> ambient 0.2 diffuse 0.9 0.03 }
//texture { Grnt8a scale <2.5 4.5 3.5> rotate <0 0 40>  phong 1.0 phong_size 90 }
texture { Crack3 scale <1 2 1.4> rotate <10 0 -20> phong 1.0 phong_size 90 }

//------------- Green Jade with white veining
#declare Stone18 =
texture { color SeaGreen ambient 0.3 diffuse 0.6 0.03 }
texture { Grnt22 scale <1.5 0.7 0.5> rotate <0 0 40>  }
texture { Grnt20a scale <2.5 2 0.5> rotate <0 0 -50>  }
texture { Crack4 scale <0.7 1 1> rotate <10 0 -20> phong 1.0 phong_size 90 }

//------------- Peach Granite with white patches & green trim
#declare Stone19 =
texture { Grnt26 scale <1 0.7 0.5> rotate <0 0 40>  }
texture { Grnt20a scale <2 3 1> rotate <10 0 -20> phong 1.0 phong_size 90 }

//------------- Brown & Olive Marble with white veining
#declare Stone20 =
texture { Grnt27 scale <0.7 0.99 0.7> rotate <0 0 40>  }
texture { Grnt12a scale <1 1.3 2> rotate <0 0 40>  }
texture { Grnt20a scale <1.9 3 0.5> rotate <0 0 -50>  }
texture { Crack1 scale <1 0.6 1> rotate <10 0 -20> phong 1.0 phong_size 90 }

//------------- Red Marble with gray & white veining
#declare Stone21 =
texture { Grnt28 scale <1.3 2.5 1.7> rotate <0 0 40>  }
texture { Grnt22 scale <1 2 2> rotate <0 0 40>  }
texture { Crack4 scale <1 0.6 1> rotate <10 0 -20> phong 1.0 phong_size 90 }

//------------- Dark Tan Marble with gray & white veining
#declare Stone22 =
texture { color Feldspar  }
texture { Grnt8a scale <1 2 2> rotate <0 0 40>  }
texture { Grnt22 scale <2 4 1.5> rotate <0 0 -50>  }
texture { Crack4 scale <1 1 1> rotate <10 0 -40> phong 1.0 phong_size 90 }

//------------- Peach & Cream Marble with orange veining
#declare Stone23 =
texture { Grnt29 scale <1 1 2> rotate <40 0 0> rotate <0 0 30> }
texture { Grnt24a scale <2 1 2> rotate <40 0 0> rotate <0 0 30> }
texture { Crack1 scale <1 2 1.5> rotate <0 0 40> }
texture { color Yellow alpha 0.9 phong 1.0 phong_size 90 }  // tint to liking

//------------- Green & Tan Moss Marble
#declare Stone24 =
texture { Grnt25 scale <1 1 2> rotate <0 0 50> rotate <20 0 30> }
texture { Grnt23a scale <2 1 2> rotate <40 0 0> rotate <0 0 -30> }
texture { color Gray alpha 0.8 phong 1.0 phong_size 90 } //   tint to liking



