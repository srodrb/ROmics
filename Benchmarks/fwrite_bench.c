#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	const char* fwrite_output_file = "./fwrite_benchmark_results.txt";

	fprintf(stdout, "Running fwrite() benchmark, the results will be stored in %s\n", fwrite_output_file);
	fprintf(stdout, "End of the fwrite() benchmark\n");
	return 0;
}