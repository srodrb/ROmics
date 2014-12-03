X = read.table("x_sample_big_r.txt", sep="\t");
Y = read.table("y_sample_big_r.txt", sep="\t");
eigen( solve( cor(X)) %*% cor( X, Y) %*% solve(cor(Y)) %*% cor(Y,X) );
