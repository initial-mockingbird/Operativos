f :: a -> b -> c -> d
f = undefined

g :: d -> f -> g
g = undefined

h :: a -> f -> g
h = g . f