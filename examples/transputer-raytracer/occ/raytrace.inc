
---------------------------- COMMANDS PROTOCOL -----------------------------
  
VAL c.stop    IS 0 :
VAL c.render  IS 1 :  -- render; x0; y0
VAL c.object  IS 2 :  -- object; size; [ size ]
VAL c.light   IS 3 :  -- light;  size; [ size ]
VAL c.patch   IS 4 :  -- patch;  x; y; patchSize; [ patchSize ][ patchSize ]
VAL c.runData IS 5 :  -- ambient light, rendering mode, screen pos etc.
VAL c.message IS 6 :  -- message; size; [ words ]

---------------------------- RENDERING MODES ------------------------------

VAL m.adaptive   IS 0 :
VAL m.stochastic IS 1 :
VAL m.dumb       IS 2 :
VAL m.test       IS 3 :

---------------------------- TREE MAINTENANCE FIELDS ----------------------

VAL nil       IS -1 :

VAL n.reflect IS 0 :        -- point to children
VAL n.refract IS 1 :
VAL n.next    IS 2 :        -- maintain linked list of leaf nodes
VAL n.type    IS 3 :        -- reflected, refracted or primary ray

-- Valid ray types for n.type slot

VAL rt.root IS 0 :   -- only first ray is of this type
VAL rt.spec IS 1 :   -- reflected ray
VAL rt.frac IS 2 :   -- transmitted ray


---------------------------- SURFACE INTERSECTION FIELDS -------------------

VAL n.objptr  IS 4 :        -- pointer into world model for intersect

VAL n.sectx   IS 5 :        -- point of intersection
VAL n.secty   IS 6 :
VAL n.sectz   IS 7 :

VAL n.normx   IS 8 :        -- surface normal at point of intersection
VAL n.normy   IS 9 :
VAL n.normz   IS 10 :

VAL n.t       IS 11 :       -- distance parameter 't' of intersection


---------------------------- RAY START / DIRECTION FIELDS ------------------

VAL n.startx  IS 12 :       -- ray origin
VAL n.starty  IS 13 :
VAL n.startz  IS 14 :

VAL n.dx      IS 15 :       -- ray direction
VAL n.dy      IS 16 :
VAL n.dz      IS 17 :

---------------------------- SHADE FIELDS ----------------------------------

VAL n.red   IS 18 :
VAL n.green IS 19 :
VAL n.blue  IS 20 :

VAL nodeSize IS n.blue + 1 :

---------------------------- SIZE OF TREE ----------------------------------

VAL maxDepth IS 6 :
VAL maxNodes IS 4 + (2 << maxDepth) :  -- traverse tree 5 deep max,
                                       -- PLUS 4 for good measure
VAL treeSize  IS nodeSize * maxNodes : -- shadow generation

---------------------------- OBJECT TYPES ----------------------------------

VAL o.sphere    IS 0 :
VAL o.xyplane   IS 1 :
VAL o.xzplane   IS 2 :
VAL o.yzplane   IS 3 :
VAL o.plane     IS 4 :
VAL o.cone      IS 5 :
VAL o.cylinder  IS 6 :
VAL o.ellipsoid IS 7 :
VAL o.quadric   IS 8 : -- general quadric, hell to compute

---------------------------- FIELDS COMMON TO ALL OBJECT CLASSES -----------

VAL x.axis     IS  0 :
VAL y.axis     IS  1 :
VAL z.axis     IS  2 :

-- the attribute word contains gross attributes of the object as
-- bits. The relevant words in the object descriptor expand upon these

VAL a.spec     IS #01 :  -- use lambert / use phong and reflect
VAL a.frac     IS #02 :  -- refract on surface
VAL a.bound0   IS #04 :  -- bounded in 1 dimension
VAL a.bound1   IS #08 :  -- bounded in 2 dimension
VAL a.tex      IS #10 :  -- texture map ?
VAL a.bump     IS #20 :  -- bump map    ?

VAL o.type     IS  0 : -- type field of all objects
VAL o.attr     IS  1 : -- attribute field of all object

VAL o.kdR      IS  2 :  -- coeffs of diffuse reflection R G B
VAL o.kdG      IS  3 :
VAL o.kdB      IS  4 :

VAL o.ks       IS  5 :  -- coeff of specular reflection ( for reflections)

VAL o.kg       IS  6 :  -- coeff of gloss for phong shading
VAL o.xmitR    IS  7 :  -- coeffs of transmission R G B
VAL o.xmitG    IS  8 :
VAL o.xmitB    IS  9 :

VAL o.power    IS 10 :  -- power for phong shading
VAL o.refix    IS 11 :  -- refractive index (relative)

VAL o.specific IS 12 :


---------------------------- SPHERE RECORD ---------------------------------

VAL first      IS o.specific :

VAL s.rad      IS  first + 0 :
VAL s.x        IS  first + 1 :
VAL s.y        IS  first + 2 :
VAL s.z        IS  first + 3 :

VAL s.map      IS  first + 4 :  -- which map ( texture or bump)
VAL s.uoffs    IS  first + 5 :  -- offsets for u and v in map
VAL s.voffs    IS  first + 6 :
VAL s.size     IS  first + 7 :


---------------------------- PLANE RECORD ----------------------------------

VAL first      IS o.specific :

VAL p.axis     IS first + 0 : -- is axis x y or z ?
VAL p.x        IS first + 1 : -- offset from x  -- or y or z
VAL p.y        IS first + 2 : -- offset from x  -- or y or z
VAL p.z        IS first + 3 : -- offset from x  -- or y or z
VAL p.size0    IS first + 4 : -- offset from y  -- bounding
VAL p.size1    IS first + 5 : -- offset from z  -- bounding value

VAL p.map      IS first + 6 :  -- which map ( texture or bump)
VAL p.uoffs    IS first + 7 :  -- offsets for u and v in map
VAL p.voffs    IS first + 8 :
VAL p.size     IS first + 9 :

---------------------------- SIZE OF RECORD ---------------------------------

VAL sizeWorld IS 1800 :  -- a sphere = 17 words + nextPtr,
                         -- giving 100 spheres


---------------------------- LIGHTS LIST ------------------------------------

VAL l.ir   IS 0 :   -- light intensity fields  R G B
VAL l.ig   IS 1 :
VAL l.ib   IS 2 :

VAL l.dx   IS 3 :   -- light direction cosines x y z
VAL l.dy   IS 4 :   -- this is a quick'n'tacky shading sheme
VAL l.dz   IS 5 :

VAL l.size IS 6 :


---------------------------- SIZE OF LIGHTS LIST ----------------------------

-- each light is |nextPtr|r|g|b|dx|dy|dz

VAL sizeLight  IS  7 :

VAL maxLights  IS 10 :   -- any more than 2 is hell to shade !!

VAL sizeLights IS sizeLight * maxLights :

---------------------------- MAX RECURSION LEVEL -----------------------------

VAL maxDescend   IS 2 :    -- this is a space limitation on the sub-pixel
                           -- grid rather than a compute saver
VAL descendPower IS 4 :    -- 2 ^^ maxDescend


---------------------------- SUB PIXEL ARRAY ---------------------------------

VAL patchSize IS 16 :
VAL gridSize  IS ((patchSize + 1) * descendPower) + 1 :

---------------------------- MINT --------------------------------------------

VAL mint  IS MOSTNEG INT :


---------------------------- COLOURS IN WORD ---------------------------------

-- colours are nr | 10 blue bits | 10 green bits | 10 red bits |

VAL threshold   IS 10 << 2 : -- this is 10 on a scale of 256, but we
VAL colourBits  IS 10 :      -- generate 10 bits

VAL rMask       IS (1 << colourBits) - 1 :
VAL gMask       IS rMask    << colourBits :
VAL bMask       IS gMask    << colourBits :

VAL rMaxDiff    IS threshold :               -- threshold values for
VAL gMaxDiff    IS rMaxDiff << colourBits :  -- instancing super sample
VAL bMaxDiff    IS gMaxDiff << colourBits :

VAL notRendered IS mint :  -- top bit set = not rendered

------------------------------------------------------------------------------

VAL first      IS o.specific:
VAL e.x        IS first + 0:       -- center of the ellipsoid
VAL e.y        IS first + 1:
VAL e.z        IS first + 2:
VAL e.a11      IS first + 3:       -- transformation matrix of the ellipsoid
VAL e.a12      IS first + 4:       -- from gobal to local coord.
VAL e.a13      IS first + 5:
VAL e.a21      IS first + 6:
VAL e.a22      IS first + 7:
VAL e.a23      IS first + 8:
VAL e.a31      IS first + 9:
VAL e.a32      IS first + 10:
VAL e.a33      IS first + 11:
VAL e.radx     IS first + 12:      -- size on x axis ( of the new system )
VAL e.rady     IS first + 13:      -- size on y axis ( of the new system )
VAL e.radz     IS first + 14:      -- size on z axis ( of the new system )
VAL e.size     IS first + 15:

VAL first      IS o.specific:
VAL co.x       IS first + 0:       -- center of the circle
VAL co.y       IS first + 1:       --  at the beginnig of the cone
VAL co.z       IS first + 2:
VAL co.a11     IS first + 3:       -- transformation matrix of the cone
VAL co.a12     IS first + 4:       -- from gobal to local coord.
VAL co.a13     IS first + 5:
VAL co.a21     IS first + 6:
VAL co.a22     IS first + 7:
VAL co.a23     IS first + 8:
VAL co.a31     IS first + 9:
VAL co.a32     IS first + 10:
VAL co.a33     IS first + 11:
VAL co.rada    IS first + 12:      -- size of the circle at the beginning
VAL co.radb    IS first + 13:      -- size of the circle at the end
VAL co.len     IS first + 14:      -- length of the cone on new z axis
VAL co.size    IS first + 15:

VAL first      IS o.specific:
VAL c.x        IS first + 0:       -- center of the circle
VAL c.y        IS first + 1:       --  at the beginnig of the cylinder
VAL c.z        IS first + 2:
VAL c.a11      IS first + 3:       -- transformation matrix of the cylinder
VAL c.a12      IS first + 4:       -- from gobal to local coord.
VAL c.a13      IS first + 5:
VAL c.a21      IS first + 6:
VAL c.a22      IS first + 7:
VAL c.a23      IS first + 8:
VAL c.a31      IS first + 9:
VAL c.a32      IS first + 10:
VAL c.a33      IS first + 11:
VAL c.rad      IS first + 12:      -- size of the circle
VAL c.len      IS first + 13:      -- length of the cylinder
VAL c.size     IS first + 14:

VAL first      IS o.specific :
VAL p.x        IS first + 1  :
VAL p.y        IS first + 2  :
VAL p.z        IS first + 3  :
VAL p.ux       IS first + 4  :
VAL p.uy       IS first + 5  :
VAL p.uz       IS first + 6  :
VAL p.vx       IS first + 7  :
VAL p.vy       IS first + 8  :
VAL p.vz       IS first + 9  :
VAL p.wx       IS first + 10 :
VAL p.wy       IS first + 11 :
VAL p.wz       IS first + 12 :
VAL p.sizeu    IS first + 13 :
VAL p.sizev    IS first + 14 :
VAL p.map      IS first + 15 : -- which map ( texture or bump)
VAL p.uoffs    IS first + 16 : -- offsets for u and v in map
VAL p.voffs    IS first + 17 :
VAL p.size     IS first + 18 :

VAL first      IS o.specific :
VAL pxy.x      IS first + 0 : -- origin x of the plane
VAL pxy.y      IS first + 1 : -- origin y of the plane
VAL pxy.z      IS first + 2 : -- origin z of the plane
VAL pxy.sizex  IS first + 3 : -- bounding on x axis
VAL pxy.sizey  IS first + 4 : -- bounding on y axis
VAL pxy.map    IS first + 5 : -- which map ( texture or bump)
VAL pxy.uoffs  IS first + 6 : -- offsets for u and v in map
VAL pxy.voffs  IS first + 7 :
VAL pxy.size   IS first + 8 :

VAL first      IS o.specific :
VAL pxz.x      IS first + 0 : -- origin x of the plane
VAL pxz.y      IS first + 1 : -- origin y of the plane
VAL pxz.z      IS first + 2 : -- origin z of the plane
VAL pxz.sizex  IS first + 3 : -- bounding on x axis
VAL pxz.sizez  IS first + 4 : -- bounding on z axis
VAL pxz.map    IS first + 5 : -- which map ( texture or bump)
VAL pxz.uoffs  IS first + 6 : -- offsets for u and v in map
VAL pxz.voffs  IS first + 7 :
VAL pxz.size   IS first + 8 :

VAL first      IS o.specific :
VAL pyz.x      IS first + 0 : -- origin x of the plane
VAL pyz.y      IS first + 1 : -- origin y of the plane
VAL pyz.z      IS first + 2 : -- origin z of the plane
VAL pyz.sizey  IS first + 3 : -- bounding on y axis
VAL pyz.sizez  IS first + 4 : -- bounding on z axis
VAL pyz.map    IS first + 5 : -- which map ( texture or bump)
VAL pyz.uoffs  IS first + 6 : -- offsets for u and v in map
VAL pyz.voffs  IS first + 7 :
VAL pyz.size   IS first + 8 :
