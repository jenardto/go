# mirroring
  # use a flag, decide whether vertices need to be read backwards or not before render
# texture information s and t stored in the face
# flatten scene for subdivision
  # need merge function to flatten scene, merge vertices of instances that should be a single vertex

# instancing should be able to specify surface texture
# hierarchial coloring
# s,t values must be present when and f is specified

#(include square "square.obj")
(surface yellow (rgb 1.0 1.0 0.0))
(surface textureName (bitmap "lizard.bmp"))

(m square
   (v 0.25 0.25 0.0)
   (v 0.25 -0.25 0.0)
   (v -0.25 -0.25 0.0)
   (v -0.25 0.25 0.0)
   (f 4(0,0) 3(1,0) 2(1,1) 1(0,1) )
   #(f 4(0,0) 3(1,0) 2(1,1) 1(0,1) )
)    

(g corner
   (i i1 square (rot 180 1 0 0) (t 0.25 0.25 -0.5) (color 0.0 0.0 1.0))
   (i i2 square (rot -90 1 0 0) (t 0.25 0.5 -0.25) (color 0.0 1.0 0.0))
   (i i3 square (rot 90 0 1 0) (t 0.5 0.25 -0.25) (surf textureName))
)

(g quad
   (i i1 corner)
   (i i2 corner (rot 90 1 0 0))
)

(g half
   (i i1 quad)
   (i i2 quad (rot -90 0 0 1))# (t 0 0 0.5))
)

(g cube
   (i i1 half)
   (i i2 half (rot 180 0 1 0))# (t -0.5 0 0))  
)




(g nquad
   (i i1 corner)
   (i i2 corner (rot 90 1 0 0))
)

(g nhalf
   (i i1 quad)
   (i i2 quad (rot 180 1 0 0))
)

(g ncube
   (i i1 nhalf)
   (i i2 nhalf (rot 180 0 1 0))
)

(render ncube)