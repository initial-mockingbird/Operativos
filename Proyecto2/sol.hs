fuelFunny :: Int -> Int
fuelFunny 0 = 0
fuelFunny 1 = 20
fuelFunny n
  | odd n     = fuelFunny (3*n`div` 7)
  | otherwise = fuelFunny (n `div` 4)