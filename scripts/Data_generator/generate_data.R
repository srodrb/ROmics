# Fichero para crear las matrices aleatorias necesarias para los benchmarks

setwd("/home/samuel/Dropbox/Romics/testdata")
set.seed(12345)

dx = 10
dy = 10

X <- matrix(round(rnorm(dx*dy, mean =10, sd=2.1), 1) , ncol=dx, nrow=dy)
Y <- matrix(round(rnorm(dx*dy, mean = 5, sd=4.1), 1) , ncol=dx, nrow=dy)


write(X, file="x_sample_small_r.txt", ncolumns =  dy, sep = "\t")
write(Y, file="y_sample_small_r.txt", ncolumns =  dy, sep = "\t")

X = t(X)
Y = t(Y)

write(X, file="x_sample_small.txt", ncolumns = dx, sep = "\t")
write(Y, file="y_sample_small.txt", ncolumns = dx, sep = "\t")

