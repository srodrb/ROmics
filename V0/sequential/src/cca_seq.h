#ifndef __CCA_SEQ_H_DEFINED__
#define __CCA_SEQ_H_DEFINED__

#define ID(j,i,n) (j*n +i)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "debug.h"

#ifdef DOUBLE
	typedef double real;
#else
	typedef float  real;
#endif

void* SafeMalloc( size_t size);

void ShowSquaredMatrix( const char* message, real* data, const unsigned int dimension);
void ComputeEigenvalues(real* A, const unsigned int NumberOfVaribles);


typedef struct {
	unsigned int nrows, ncols;
	real* arrayOfMeans;
	real* rawData;
	real* covarianceMatrix;
} Sample;


/* carga en memoria la matriz de datos especificada en la ruta */
Sample* ImportSampleData( const char* pathToSample);


/* Una vez inicializada la estructura, esta funcion:
 * 				calcula la media de la variable y la almacena en el array de medias
 * 				centra el array, restandole la media para ahorrar calculos posteriores
 * 				calcula la varianza de la variable y la devuelve
 */
real leverage( real *var_1,  real* arrayOfMeans, unsigned int length);

// centra las variables restando la media y coloca un 1 en la matriz de var-cov
void Sample_applyLeverage(Sample* S);

/* funcion para calcular la covarianza entre dos variables */
real covariance( real* var_1, real* var_2, unsigned int length);
void Sample_applyCovariance( Sample* S);

void Sample_invertCovarianceMatrix( Sample* S);
void Sample_ShowCovarianceMatrix( Sample* S);
void FreeSample( Sample* S);


// interrelaciones entre samples
typedef struct{
	Sample *sample_x;
	Sample *sample_y;

	real* betweenSetsCovariance;
}cca;

cca* cca_perform( const char* path_sample1, const char* path_sample2);
void cca_free( cca* C);

#endif // __CCA_SEQ_H_DEFINED__
