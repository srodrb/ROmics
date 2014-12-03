#ifndef __CCA_SEQ_H_DEFINED__
#define __CCA_SEQ_H_DEFINED__

#include <stdio.h>
#include <stdlib.h>


	#include "cblas.h"

    // LU decomoposition of a general matrix
    void sgetrf_(int* M, int *N, float* A, int* lda, int* IPIV, int* INFO);

    // generate inverse of a matrix given its LU decomposition
    void sgetri_(int* N, float* A, int* lda, int* IPIV, float* WORK, int* lwork, int* INFO);

    // computes the hessenberg form of the matrix A
    void sgehrd_( int* n, int* ilo, int* ihi, float* A, int* lda, float* tau, float* work, int* lwork, int* info);
    // computes the eigenvalues from the Hessenberg form matrix A (given by H)
    void shseqr_ (char *job, char* compz, int* n, int* ilo, int* ihi, float* H, int* ldh, float* wr, float* wi, float* z, int* ldz, float* work, int* lwork, int* info);


void ShowSquaredMatrix( const char* message, float* data, const unsigned int dimension);
void ComputeEigenvalues(float* A, const unsigned int NumberOfVaribles);

#define ID(j,i,n) (j*n +i)

typedef struct {
	unsigned int nrows, ncols;
	float* arrayOfMeans;
	float* rawData;
	float* covarianceMatrix;
} Sample;


typedef enum{
	ELIMINATE_COVARIANCE_MATRIX = 0,
	DUPLICATE_COVARIANCE_MATRIX = 1
}MemConfig;

/* carga en memoria la matriz de datos especificada en la ruta */
Sample* ImportSampleData( const char* pathToSample);


/* Una vez inicializada la estructura, esta funcion:
 * 				calcula la media de la variable y la almacena en el array de medias
 * 				centra el array, restandole la media para ahorrar calculos posteriores
 * 				calcula la varianza de la variable y la devuelve
 */
inline float leverage( float *var_1,  float* arrayOfMeans, unsigned int length);

// centra las variables restando la media y coloca un 1 en la matriz de var-cov
void Sample_applyLeverage(Sample* S);

/* funcion para calcular la covarianza entre dos variables */
inline float covariance( float* var_1, float* var_2, unsigned int length);
void Sample_applyCovariance( Sample* S);

void Sample_invertCovarianceMatrix( Sample* S);
void Sample_ShowCovarianceMatrix( Sample* S);
void FreeSample( Sample* S);


// interrelaciones entre samples
typedef struct{
	Sample *sample_x;
	Sample *sample_y;

	float* betweenSetsCovariance;
}cca;

void cca_free( cca* C);
cca* cca_perform( const char* path_sample1, const char* path_sample2);

#endif // __CCA_SEQ_H_DEFINED__
