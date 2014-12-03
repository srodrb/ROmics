#include "cca_seq.h"
#include <time.h>

int main(void) {

	// asumimos que los dos samples tienen el mismo numero de variables
	// asumimos, por ahora, que la matriz esta transpuesta

	//const char *path_sample1 = "/home/samuel/Dropbox/Romics/testdata/x_sample_small.txt";
	//const char *path_sample1 = "/home/samuel/Dropbox/Romics/testdata/x_sample_small.txt";
	
    const char *path_sample2 = "../../../testdata/y_sample_small.txt";
	const char *path_sample2 = "../../../testdata/y_sample_small.txt";
	clock_t start, end;
	double cpu_time_used;

	start = clock();
	cca* cca_analysis = cca_perform( path_sample1, path_sample2);
	end = clock();
	cpu_time_used = ((double) (end-start)) / CLOCKS_PER_SEC;

	printf("Basic benchmark time shows %lf\n", cpu_time_used);

	/*
	 * 		Debo transponer la matriz para mejorar la localidad en memoria
	 * 		entonces tendre este patron de acceso, mas comodo para C
	 *
	 * 				nobs	nobs	nobs	nobs
	 *       nvars
	 *       nvars
	 */

	cca_free( cca_analysis);
	printf("Fin del programa!\n");
	return 0;
}
