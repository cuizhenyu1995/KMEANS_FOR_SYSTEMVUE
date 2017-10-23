#pragma once
#include "Kmeans_run.h"
#include "SystemVue/ModelBuilder.h"
#include "SystemVue/CircularBuffer.h"
#include "SystemVue/Matrix.h"
#include "SystemVue/MatrixCircularBuffer.h"
#include "SystemVue/DFParam.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

using namespace std;

namespace SystemVueModelBuilder {

#ifndef SV_CODE_KMEANS_RUN
	DEFINE_MODEL_INTERFACE(Kmeans_run)
	{
		SET_MODEL_NAME("Kmeans_run");
		SET_MODEL_DESCRIPTION("");
		SET_MODEL_CATEGORY("Kmeans");
		ADD_MODEL_HEADER_FILE("Kmeans_run.h");
		model.SetModelCodeGenName("Kmeans_run");
		model.SetModelNamespace("SystemVueModelBuilder");

		// Add parameters
		/*SystemVueModelBuilder::DFParam param = NULL;
		param = ADD_MODEL_PARAM(Number_of_k);
		param.SetDefaultValue("3");
		param.SetDescription("Kmeans_run");*/
		

		// Add input/output ports
		DFPort IN_N = ADD_MODEL_INPUT(IN_ELEMENT);
		IN_N.SetDescription("Input the number of elements");
		DFPort IN_M = ADD_MODEL_INPUT(IN_ATTRIBUTE);
		IN_M.SetDescription("Input the number of attributes");
		DFPort IN_K = ADD_MODEL_INPUT(IN_CATEGORY);
		IN_K.SetDescription("Input the number of categories");
		DFPort IN_EPS = ADD_MODEL_INPUT(IN_PRECISION);
		IN_EPS.SetDescription("Input precision");
		DFPort IN_FILE = ADD_MODEL_INPUT(IN_INPUTFILE);
		IN_FILE.SetDescription("Input the data");


		DFPort OutputPort = ADD_MODEL_OUTPUT(Output);
		OutputPort.SetDescription("Output");

		return true;
	}
#endif

	Kmeans_run::Kmeans_run()
	{
	}

	Kmeans_run::~Kmeans_run()
	{
	}

	bool Kmeans_run::Setup()
	{
		IN_ELEMENT.SetRate(1);
		IN_ATTRIBUTE.SetRate(1);
		IN_CATEGORY.SetRate(1);
		IN_PRECISION.SetRate(1);
		IN_INPUTFILE.SetRate(1);
		Output.SetRate(1);

		return true;
	};

	bool Kmeans_run::Initialize()
	{
		return true;
	}

	bool Kmeans_run::Run()
	{
		
		int n;
		int k;
		int dim;
		double threshold;
		double* h_data;
		double* h_centriods;
		
		int* h_belong;
		clock_t start, end;

		getData(&h_data, &n, &k, &dim, &threshold);

		h_centriods = (double*)malloc(sizeof(double) * k * dim);
		h_belong = (int*)malloc(sizeof(int) * n);

		start = clock();

		kMean(h_data, h_centriods, h_belong, n, &k, dim, threshold);
		end = clock();
		
		displayResult(h_data, h_centriods, h_belong, n, k, dim);

		free(h_data);
		free(h_centriods);
		free(h_belong);


		

		

		Output[0] = out_tmp;
		return true;
	}


	bool Kmeans_run::Finalize()
	{
		return true;
	}

	int Kmeans_run::getData(double** h_data,
		int* n,
		int* k,
		int* dim,
		double* threshold)
	{
		int i, j;


		*n = IN_ELEMENT[0];
		*dim = IN_ATTRIBUTE[0];
		*k = IN_CATEGORY[0];
		*threshold = IN_PRECISION[0];
		in_tmp = IN_INPUTFILE[0]; 
		
		

		*h_data = (double*)malloc(sizeof(double) * (*n) * (*dim));

		for (i = 0; i < *n; ++i)
			for (j = 0; j < *dim; j++)
				*(*h_data + j * (*n) + i) = in_tmp(i,j);

		return 0;
	}

	void Kmeans_run::kMean(double* h_data,
		double* h_centriods,
		int* h_belong,
		int n,
		int* k,
		int dim,
		double threshold)
	{
		int i, j;

		int flag;

		double *h_centriods1;
		double *h_centriods2;
		int k1, k2;

		k1 = k2 = *k;

		h_centriods1 = h_centriods;

		// initialize k centriods
		for (i = 0; i < k1; i++)
			for (j = 0; j < dim; j++)
				h_centriods1[j * k1 + i] = h_data[j * n + i];

		//allocate host memory for centriods
		h_centriods2 = (double*)malloc(sizeof(double) * k1 * dim);

		flag = 0;

		while (!flag){
			reCluster(h_data, h_centriods1, h_belong, n, k1, dim);

			updateCentriods(h_data, h_belong, h_centriods2, n, &k2, dim);

			flag = isEqual(h_centriods1, h_centriods2, k1, k2, dim, threshold);

			if (!flag){
				k1 = k2;
				// swap performance is batter
				memcpy((void*)h_centriods1, (void*)h_centriods2, sizeof(double) * k2 * dim);
			}
		}

		*k = k1;

		free(h_centriods2);

		return;
	}

	void Kmeans_run::reCluster(double* h_data,
		double* h_centriods,
		int* h_belong,
		int n,
		int k,
		int dim)
	{
		int kind;
		double min_dist;
		double dist;
		double diff;
		int i, j;
		int l;

		for (i = 0; i < n; i++){
			min_dist = FLT_MAX;
			kind = 0;
			for (j = 0; j < k; j++){
				dist = 0;
				for (l = 0; l < dim; l++){
					diff = h_data[i + l * n] - h_centriods[j + l * k];
					dist += diff * diff;
				}
				if (dist < min_dist){
					min_dist = dist;
					kind = j;
				}
			}
			h_belong[i] = kind;
		}

		return;

	}

	int Kmeans_run::isEqual(double* h_centriods1,
		double* h_centriods2,
		int k1,
		int k2,
		int dim,
		double threshold)
	{
		int i, j;
		double diff;
		double dist;

		if (k1 != k2)
			return 0;

		for (i = 0; i < k1; i++){
			dist = 0;
			for (j = 0; j < dim; j++){
				diff = h_centriods1[j * k1 + i] - h_centriods2[j * k1 + i];
				dist += diff * diff;
			}

			if (sqrt(dist) > threshold)
				return 0;
		}

		return 1;
	}

	void Kmeans_run::updateCentriods(double* h_data,
		int* h_belong,
		double* h_centriods,
		int n,
		int* k,
		int dim)
	{
		int* count;
		int kind;
		int i, j;
		int l;

		count = (int*)malloc((*k) * sizeof(int));
		memset((void*)count, 0, sizeof(int) * (*k));

		memset((void*)h_centriods, 0, sizeof(double) * (*k) * dim);

		for (i = 0; i < n; i++){
			kind = h_belong[i];
			count[kind]++;
			for (j = 0; j < dim; j++)
				h_centriods[j * (*k) + kind] += h_data[j * n + i];  // long int h_centriods[] to store huge quantity!!!
		}

		for (i = 0; i < *k; i++){
			if (count[i] == 0)
				continue;
			for (j = 0; j < dim; j++)
				h_centriods[j * (*k) + i] /= count[i];
		}

		l = 0;
		for (i = 0; i < dim; i++)
			for (j = 0; j < *k; j++){
				// if (count[j] == 0)
				//     continue;
				h_centriods[l++] = h_centriods[i * (*k) + j];
			}

		*k = l / dim;  // ????

		free(count);

		return;
	}

	void Kmeans_run::displayResult(double* h_data,
		double* h_centriods,
		int* h_belong,
		int n,
		int k,
		int dim)
	{
		double* p_data;
		int* count;
		int* start;
		int kind;
		int i, j;
		int l;
		int r, s, t;
		FILE* out = fopen("out_kmeans.txt", "w+");

		p_data = (double*)malloc(sizeof(double) * n * dim);
		count = (int*)malloc(sizeof(int) * k);
		start = (int*)malloc(sizeof(int) * k);

		memset((void*)p_data, 0, sizeof(double) * n * dim);
		memset((void*)count, 0, sizeof(int) * k);

		for (i = 0; i < n; i++){
			kind = h_belong[i];
			count[kind]++;
		}
		// accumulated addtion for count[]
		start[0] = 0;
		for (i = 1; i < k; i++)
			start[i] = start[i - 1] + count[i - 1];

		for (i = 0; i < n; i++){
			kind = h_belong[i];
			l = start[kind];

			for (j = 0; j < dim; j++)
				p_data[l + j * n] = h_data[i + j * n];

			start[kind]++;
		}

		out_tmp.Resize(k, dim+1);

		out_tmp.Zero();

		r = 0;
		fprintf(out, "k=%d\n", k);
		for (i = 0; i < k; i++){
			fprintf(out, "cluster %d \n", i);

			fprintf(out, "centriod: ");
			for (j = 0; j < dim; j++)
			{
				fprintf(out, "%f ", h_centriods[i + j * k]);
				out_tmp(i, j) = h_centriods[i + j * k];
			}
			fprintf(out, "\n");

			fprintf(out, "elements number: %d\n", count[i]);
			out_tmp(i, dim) = (double)count[i];
			/*        printf("elements:\n");

			for (s = 0; s < count[i]; s++){
			for (t = 0; t < dim; t++)
			printf("%f ", p_data[r + t * n]);

			printf("\n");

			r++;
			} */

			fprintf(out, "\n");
		}
		fclose(out);

		Output[0] = out_tmp;

		free(p_data);
		free(count);
		free(start);

		return;
	}

}