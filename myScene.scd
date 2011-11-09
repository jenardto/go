# texture information s and t stored in the face

(include square "square.obj")
(surface yellow (rgb 1.0 1.0 0.0))

#(m square
#   (v 0.25 0.25 0.0)
#   (v 0.25 -0.25 0.0)
#   (v -0.25 -0.25 0.0)
#   (v -0.25 0.25 0.0)
#   (f 4 3 2 1)
#)   

(g corner
   (i i1 square (rot 180 1 0 0) (t 0.25 0.25 -0.5) (color 0.0 0.0 1.0))
   (i i2 square (rot -90 1 0 0) (t 0.25 0.5 -0.25) (color 0.0 1.0 0.0))
   (i i3 square (rot 90 0 1 0) (t 0.5 0.25 -0.25) (color yellow))
)

(g quad
   (i i1 corner )#(color 1.0 0.0 0.0))
   (i i2 corner (rot 90 1 0 0)) #(t 0.5 0.25 -0.25))
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