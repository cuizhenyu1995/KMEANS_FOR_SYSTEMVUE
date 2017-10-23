#pragma once
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

namespace SystemVueModelBuilder
{
	class Kmeans_run : public SystemVueModelBuilder::DFModel
	{
	public:
		DECLARE_MODEL_INTERFACE(Kmeans_run);
		Kmeans_run();
		virtual ~Kmeans_run();

		virtual bool Setup();
		virtual bool Initialize();
		virtual bool Run();
		virtual bool Finalize();
		int getData(double** h_data,int* n,int* k,int* dim,double* threshold);
		void kMean(double* h_data,double* h_centriods,int* belong,int n,int *k,int dim,double threshold);
		void reCluster(double* d_data,double* d_centriods,int* d_belong,int n,int k,int dim);
		int isEqual(double* h_centriods1,
		double* h_centriods2,int k1,int k2,int dim,double threshold);
		void updateCentriods(double* h_data,int* h_belong,double* h_centriods,int n,int* k,int dim);
		void displayResult(double* h_data,double* h_centriods,int* h_belong,int n,int k,int dim);

		/// define parameters
		int NUMBER_OF_ELEMENT;
		int NUMBER_OF_ATTRIBUTE;
		int NUMBER_OF_CATEGORY;
		double PRECISION;

		/// define ports
		IntCircularBuffer IN_ELEMENT;
		IntCircularBuffer IN_ATTRIBUTE;
		IntCircularBuffer IN_CATEGORY;
		DoubleCircularBuffer IN_PRECISION;
		DoubleMatrixCircularBuffer IN_INPUTFILE;

		DoubleMatrixCircularBuffer Output;

	protected:
		DoubleMatrix in_tmp;
		DoubleMatrix out_tmp;

	};
}
