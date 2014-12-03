#include "cca_seq.h"

void* SafeMalloc( size_t size){
	void* temp = malloc(size);
	
	if(temp != NULL)
		return temp;
	else{
		printf("Problem when allocating memory!\n");
		exit(-1);
	}
}


void ShowSquaredMatrix( const char* message, real* data, const unsigned int dimension){
	/* prints out a squared matrix along with a user message */
	printf("\n ShowMatrix info: %s\n", message);
	for (int j = 0; j < dimension; ++j) {
		for (int i = 0; i < dimension; ++i) {
			printf("%.3f  ", data[j*dimension + i]);
		}
		printf("\n");
	}
}

void ComputeEigenvalues(real* A, const unsigned int NumberOfVaribles){
	/*
	 * Calcula los valores propios de la matriz A para el caso particular
	 * del problema real no simetrico.
	 * El problema se aborda empleando las funciones de Lapack declaradas con anterioridad
	 * para ser llamadas desde C.
	 * Puesto que la matriz es densa y no tiene elementos nulos, no realizamos el balanceo.
	 * El primer paso es llamar a la funcion sgehrd_ con el parametro lwork=-1
	 * para obtener el valor de block optimo. Posteriormente se calcula la forma
	 * de Hessenberg de A empleando la misma funcion.
	 * Por ultimo se ejecuta shseqr_ para calcular los valores propios de la matriz
	 *
	 * Para obtener informacion detallada sobre las rutinas de lapack
	 * > man sgehrd
	 * > man shseqr
	 */

	real* tau = SafeMalloc( (NumberOfVaribles -1)*sizeof(real));
	int i;	 								// just loop counters
	int info;
	int order = (int) NumberOfVaribles;
	int ilo = 1;
	int ihi = order;
	int lwork_sgehrd = -1; 					// workspace query
	real wwork;
	char job   = 'E'; 						// only eigenvalues
	char compz = 'N'; 						// no schur vectors are computed

	sgehrd_( &order, &ilo, &ihi, A, &order, tau, &wwork, &lwork_sgehrd, &info);
	if(info!=0){printf("sgehrd_ execution : failed\n");}

	lwork_sgehrd = (int) wwork;
	real* work = SafeMalloc( sizeof(real) * lwork_sgehrd);

	sgehrd_( &order, &ilo, &ihi, A, &order, tau, work, &lwork_sgehrd, &info);
	if(info!=0){printf("sgehrd_ execution : failed\n");}

	free( tau); tau = NULL;


	real* wr = SafeMalloc( sizeof(real) * order); 	// real part of eigenvalues
	real* wi = SafeMalloc( sizeof(real) * order); 	// imaginary part of eigenvalues
	real* z  = SafeMalloc( sizeof(real) );        	// not used
	int ldz = 1;
	int lwork_shseqr = -1; 								// query optimal dimensions

	shseqr_ (&job, &compz, &order, &ilo, &ihi, A, &order, wr, wi, z, &ldz, &wwork, &lwork_shseqr, &info);
	if(info!=0){printf("shseqr_ execution : failed\n");}


	lwork_shseqr = (int) wwork;
	if( lwork_shseqr != lwork_sgehrd) /* work array has to be resized */
		work = (real*) realloc( work, (int) sizeof(real) * lwork_shseqr);


	shseqr_ (&job, &compz, &order, &ilo, &ihi, A, &order, wr, wi, z, &ldz, work, &lwork_shseqr, &info);

	//for (i = 0; i < NumberOfVaribles; ++i)
	//	printf("Eigenvalue %d = %.4f %.4f i\n", i, wr[i], wi[i]);

	free( wr);
	free( wi);
	free( z );
	free( work);
}

/* Inicializacion del dataset mediante carga de fichero externo */
Sample* ImportSampleData( const char* pathToSample){
	int i,j;
	Sample* dataBuffer = SafeMalloc( sizeof(Sample));

	FILE *file = fopen(pathToSample, "r");
	if( !file ){
		perror("Error when reading matrix data\n");
		exit(-1);
	}

	// los dos primeros numeros son las filas y las columnas
	if( !fscanf(file, "%d", &dataBuffer->nrows )) {
		perror("Problem reading number of rows!");
		exit(-1);
	}

	if( !fscanf(file, "%d", &dataBuffer->ncols )) {
		perror("Problem reading number of columns!");
		exit(-1);
	}

	// printf("\nInfo: Loading %s sample on memory. Dimensions: (%d,%d)\n", pathToSample, dataBuffer->nrows, dataBuffer->ncols);

	/* Reservamos memoria necesaria para el array de medias, la matriz de covarianzas y los datos en crudo */
	dataBuffer->rawData = SafeMalloc( dataBuffer->nrows * dataBuffer->ncols * sizeof(real));
	if( dataBuffer->rawData == NULL){
		perror("Error while allocating memory for coefficients!");
		exit(-1);
	}

	dataBuffer->arrayOfMeans = SafeMalloc( dataBuffer->nrows * sizeof(real));
	if( dataBuffer->arrayOfMeans == NULL){
		perror("Error while allocating memory for means array!");
		exit(-1);
	}

	dataBuffer->covarianceMatrix = SafeMalloc( dataBuffer->nrows * dataBuffer->nrows * sizeof(real));
	if( dataBuffer->covarianceMatrix == NULL){
		perror("Error while allocating memory for covariance matrix!");
		exit(-1);
	}


	/* leemos los datos del fichero de coeficientes */
	for (j = 0; j < dataBuffer->nrows; ++j) {
		for (i = 0; i < dataBuffer->ncols; ++i) {
			if( !fscanf( file, "%f", &dataBuffer->rawData[j*dataBuffer->ncols + i])){
				break;
			}
			//printf("%.1f  ", dataBuffer->rawData[ j*dataBuffer->ncols + i]);
		}
		//printf("\n");
	}

	fclose( file);

	printf("       Sample %s succesfully loaded\n\n", pathToSample);

	return dataBuffer;
}
/* Una vez inicializada la estructura, esta funcion:
 * 				calcula la media de la variable y la almacena en el array de medias
 * 				centra el array, restandole la media para ahorrar calculos posteriores
 * 				calcula la varianza de la variable y la devuelve
 */
real leverage( real *var_1,  real* arrayOfMeans, unsigned int length){
	real mean = 0.0;
	int i;

	for (i = 0; i < length; ++i) {
		mean += var_1[i];
	}

	mean = mean / (real) length;
	//printf("Media calculada en la funcion %f\n", mean);

	*arrayOfMeans = mean;

	for (i = 0; i < length; ++i) {
		var_1[i] -= mean;
	}
	return 1.0;
};

/* centra las variables restando la media y coloca un 1 en la matriz de covarianzas */
void Sample_applyLeverage(Sample* S){
	int i;
	for (i = 0; i < S->nrows; ++i) {
		S->covarianceMatrix[i*S->nrows + i] = \
		leverage( &S->rawData[i * S->ncols], &S->arrayOfMeans[i], S->ncols);
	}
}

/* funcion para calcular la covarianza entre dos variables */
real covariance( real* var_1, real* var_2, unsigned int length){
	real cov = 0.0;
	real sum1 = 0.0;
	real sum2 = 0.0;

	int i;

	for (i = 0; i < length; ++i) {
		cov  += var_1[i] * var_2[i];	/* numerador   */

		sum1 += var_1[i] * var_1[i];	/* denominador */
		sum2 += var_2[i] * var_2[i];	/* demoninador */
	}

	return cov / (sqrt(sum1) * sqrt(sum2));
}

void Sample_applyCovariance( Sample* S){
	unsigned int i, j;
	real cov;

	for (j = 0; j < S->nrows; ++j) {
		for (i = 0; i < S->nrows; ++i) {

			if( i != j){
				/* those are variances */
				cov = covariance( &S->rawData[j*S->ncols], &S->rawData[i*S->ncols], S->ncols);
				S->covarianceMatrix[j*S->nrows + i] = cov; /* upper diagonal elements */
				S->covarianceMatrix[i*S->nrows + j] = cov; /* lower diagonal elements */
			}
		}
	}
}

/* ahora tengo que invertir el sistema de ecuaciones */
void Sample_invertCovarianceMatrix( Sample* S){
	int N = S->nrows;
	/* llegado este punto no necesito mas los datos
	 * importados, por lo que puedo liberar la matriz de rawdata
	 * y en su lugar hacer una copia de la matriz de covarianza
	 * que necesito para las premultiplicaciones
	 */

	/* ya no necesitamos los datos originales en memoria */
	free( S->rawData);
	S->rawData = NULL; /* if the pointer is set to NULL free() does nothing */

	int   *ipiv = SafeMalloc( N * sizeof( int)	);		/* matriz de pivoteo */
	int   lwork = N * N;
	real *work = SafeMalloc( lwork * sizeof(real));
	int   errorHandler;

	sgetrf_(&N,&N, S->covarianceMatrix, &N, ipiv, &errorHandler);
	sgetri_(&N, S->covarianceMatrix,&N, ipiv,work,&lwork,&errorHandler);

	free(ipiv);
	free(work);

	//printf("Info: Covariance matrix succesfuly was inverted\n");
}

void Sample_ShowCovarianceMatrix( Sample* S){
	int i,j;
	printf("\n Covariance matrix\n");
	for (j = 0; j < S->nrows; ++j) {
		for (i = 0; i < S->nrows; ++i) {
				printf("%0.3f  ", S->covarianceMatrix[j*S->nrows + i]);
			}
		printf("\n");
	}
	printf("\n");
}

/* funcion free para la estructura sample */
void FreeSample( Sample* S){
	free( S->rawData); 				S->rawData = NULL;
	free( S->arrayOfMeans); 		S->arrayOfMeans = NULL;
	free( S->covarianceMatrix); 	S->covarianceMatrix = NULL;
	free( S); 						S = NULL;
}






// ************************** ahora vamos con las interrelaciones entre samples ************************ //

void cca_free( cca* C){
	FreeSample( C->sample_x);
	FreeSample( C->sample_y);

	free( C->betweenSetsCovariance);		C->betweenSetsCovariance = NULL;
	free( C);								C = NULL;
}

cca* cca_perform( const char* path_sample1, const char* path_sample2){
	unsigned int varX_,varY_; /* just counters */
	char transpose   = 'T';
	char notranspose = 'N';
	real alpha, beta;

	cca* cca_analysis = SafeMalloc(sizeof(cca));


	cca_analysis->sample_x = ImportSampleData( path_sample1 );
	Sample_applyLeverage	( cca_analysis->sample_x);
	Sample_applyCovariance  ( cca_analysis->sample_x);

	printf("Mostrando la matrix de covarianza para x\n");
	Sample_ShowCovarianceMatrix( cca_analysis->sample_x);


	cca_analysis->sample_y = ImportSampleData( path_sample2 );
	Sample_applyLeverage	( cca_analysis->sample_y);
	Sample_applyCovariance  ( cca_analysis->sample_y);


	/* Reservo memoria para la matrix de correlaciones entre los dos sets */
	const unsigned int NumberOfVaribles = cca_analysis->sample_x->nrows;
	const unsigned int NumberOfObsInX 	= cca_analysis->sample_x->ncols;
	const unsigned int NumberOfObsInY 	= cca_analysis->sample_y->ncols;

	if( NumberOfObsInX != NumberOfObsInY) perror("Number of observarions is not equal for provided samples!");

	const unsigned int covMatrixDimensions = NumberOfVaribles * NumberOfVaribles; // TODO por ahora he asumido que los samples tienen las mismas dimensiones
	cca_analysis->betweenSetsCovariance    = SafeMalloc( covMatrixDimensions * sizeof(real) );

	if( cca_analysis->betweenSetsCovariance == NULL){
		perror("Error while allocating memory for between-sets correlation matrix!");
		exit(-1);
	}

	/* calculo las correlaciones entre los samples X e Y */
	for (varX_ = 0; varX_ < NumberOfVaribles; ++varX_) {
		for (varY_ = 0; varY_ < NumberOfVaribles; ++varY_) {
			cca_analysis->betweenSetsCovariance[varX_ * NumberOfVaribles + varY_] =
					covariance( &cca_analysis->sample_x->rawData[varX_ * NumberOfObsInX], &cca_analysis->sample_y->rawData[varY_ * NumberOfObsInY], NumberOfObsInX);
			//printf("%.3f  ", cca_analysis->betweenSetsCovariance[varX_ * NumberOfVaribles + varY_]);
		}
		//printf("\n");
	}


	/* una vez tenemos calculadas las matrices de covarianza, podemos invertir y multiplicar */
	Sample_invertCovarianceMatrix( cca_analysis->sample_x); debug("Covariance matrix inverted for sample x");
	Sample_invertCovarianceMatrix( cca_analysis->sample_y); debug("Covariance matrix inverted for sample y");


	/* ahora tengo que realizar los productos de matrices */
	real* CxxCxy = SafeMalloc( covMatrixDimensions * sizeof(real));
	alpha = 1.0;
	beta  = 0.0;
	sgemm_(
			&notranspose,
			&notranspose,
			&NumberOfVaribles,
			&NumberOfVaribles,
			&NumberOfVaribles,
			&alpha,
			cca_analysis->sample_x->covarianceMatrix,
			&NumberOfVaribles,
			cca_analysis->betweenSetsCovariance,
			&NumberOfVaribles,
			&beta,
			CxxCxy,
			&NumberOfVaribles
	);

	debug("Matrix-matrix multiplication completed");

	real* CyyCyx = SafeMalloc( covMatrixDimensions * sizeof(real));
	alpha = 1.0;
	beta  = 0.0;
	sgemm_(
			&notranspose,
			&transpose,
			&NumberOfVaribles,
			&NumberOfVaribles,
			&NumberOfVaribles,
			&alpha,
			cca_analysis->sample_y->covarianceMatrix,
			&NumberOfVaribles,
			cca_analysis->betweenSetsCovariance,
			&NumberOfVaribles,
			&beta,
			CyyCyx,
			&NumberOfVaribles
	); /* TODO haciendo esto en paralelo, no se si tendremos problemas al transponer */

	debug("Matrix-matrix multiplication completed");


		/* finalmente hacemos el producto total, podemos guardarlo en cca_analysis->betweenSetsCovariance */
		alpha = 1.0;
		beta  = 0.0;
		sgemm_(
				&notranspose,
				&notranspose,
				&NumberOfVaribles,
				&NumberOfVaribles,
				&NumberOfVaribles,
				&alpha,
				CxxCxy,
				&NumberOfVaribles,
				CyyCyx,
				&NumberOfVaribles,
				&beta,
				cca_analysis->betweenSetsCovariance,
				&NumberOfVaribles
		);

	debug("Matrix-matrix multiplication completed");
	//ShowSquaredMatrix("Final product", cca_analysis->betweenSetsCovariance, NumberOfVaribles);

	/* Finally we calculate the eigenvalues of the matrix*/
	ComputeEigenvalues( cca_analysis->betweenSetsCovariance, NumberOfVaribles);
	
	debug("Eigenvalue calculation completed");

	printf("\n");
	free( CxxCxy);
	free( CyyCyx);

	return cca_analysis;
};
