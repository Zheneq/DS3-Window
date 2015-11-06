#include "field.h"

void field::Init(int _n, unsigned int flags)
{
	n = _n;

	data = (double*)malloc(sizeof(double)*n);
	sp = fftw_alloc_complex(n / 2 + 1);
	spec = (double*)malloc(sizeof(double)*(n / 2 + 1));

	forward = fftw_plan_dft_r2c_1d(n, data, sp, flags);
	backward = fftw_plan_dft_c2r_1d(n, sp, data, flags);
}

void field::Fourier(bool back)
{
	// TODO: Обратное Фурье не из того массива
	if (back)
	{
		fftw_execute(backward);

		////
		for (int i = 0; i < n; i++)
		{
			data[i] /= n;
		}
		////
	}
	else
	{
		fftw_execute(forward);

		for (int i = 0; i < n / 2 + 1; i++)
		{
			spec[i] = 2 * (sp[i][0] * sp[i][0] + sp[i][1] * sp[i][1]) / n;
		}
	}
}

void field::Free()
{
	fftw_destroy_plan(forward);
	fftw_destroy_plan(backward);
	fftw_free(sp);
	free(spec);
	free(data);
}

void field::DumpFullPrecision(FILE *DataFile, FILE *SpecFile, double(*transform)(int), double(*transformspec)(int))
{
	if (DataFile)
	{
		for (int i = 0; i < n; ++i)
			fprintf(DataFile, "%e %e\n", transform(i), data[i]);
		fprintf(DataFile, "\n\n");
	}

	if (SpecFile)
	{
		for (int i = 0; i <= n / 2; ++i)
			fprintf(SpecFile, "%e %e\n", transformspec(i), spec[i]);
		fprintf(SpecFile, "\n\n");
	}
}
