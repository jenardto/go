(Include square "square.obj")

(G corner
   (I i1 square (T 0.25 0.25 -0.5) (color 0.0 0.0 1.0))
   (I i2 square (R -90 1 0 0) (T 0.25 0.5 -0.25))
   (I i3 square (R 90 0 1 0) (T 0.5 0.25 -0.25))
)

(G quad
   (I i1 corner (color 1.0 0.0 0.0))
   (I i2 corner (R 270 1 0 0)) #(T 0.5 0.25 -0.25))
)

(G half
   (I i1 quad)
   (I i2 quad (R 90 0 1 0))# (T 0 0 0.5))
)

(G cube
   (I i1 half)
   (I i2 half (R 180 0 1 0))# (T -0.5 0 0))  
)

(Render corner)