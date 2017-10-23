#include "kmeans_in.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace SystemVueModelBuilder {

#ifndef SV_CODE_KMEANS_IN
	DEFINE_MODEL_INTERFACE(Kmeans_in)
	{
		SET_MODEL_NAME("Kmeans_in");
		SET_MODEL_DESCRIPTION("");
		SET_MODEL_CATEGORY("Kmeans");
		ADD_MODEL_HEADER_FILE("kmeans_in.h");
		model.SetModelCodeGenName("Kmeans_in");
		model.SetModelNamespace("SystemVueModelBuilder");

		// Add parameters
		SystemVueModelBuilder::DFParam param = NULL;
		param = ADD_MODEL_PARAM(NUMBER_OF_ELEMENT);
		param.SetDefaultValue(0);
		param = ADD_MODEL_PARAM(NUMBER_OF_ATTRIBUTE);
		param.SetDefaultValue(0);
		param = ADD_MODEL_PARAM(NUMBER_OF_CATEGORY);
		param.SetDefaultValue(0);
		param = ADD_MODEL_PARAM(PRECISION);
		param.SetDefaultValue(0);

		param = ADD_MODEL_PARAM(INPUT_FILE);
		param.SetParamAsFile();
		
		// Add input/output ports
		DFPort OUT_N = ADD_MODEL_OUTPUT(OUT_ELEMENT);
		OUT_N.SetDescription("Output the number of elements");
		DFPort OUT_M = ADD_MODEL_OUTPUT(OUT_ATTRIBUTE);
		OUT_M.SetDescription("Output the number of attributes");
		DFPort OUT_K = ADD_MODEL_OUTPUT(OUT_CATEGORY);
		OUT_K.SetDescription("Output the number of categories");
		DFPort OUT_EPS = ADD_MODEL_OUTPUT(OUT_PRECISION);
		OUT_EPS.SetDescription("Output precision");
		DFPort OUT_FILE = ADD_MODEL_OUTPUT(OUT_INPUTFILE);
		OUT_FILE.SetDescription("Output the data");

		return true;
	}
#endif

	Kmeans_in::Kmeans_in()
	{
	}

	Kmeans_in::~Kmeans_in()
	{
	}

	bool Kmeans_in::Setup()
	{
		//setRate��������Ե���ÿһ���˿ڣ�����˿ں�����˿ڶ�Ҫ�������ã������õĻ�Ĭ��Ϊ1
		//�˿ڿ�����Ϊ��һ��Buffer, ���ε�����˿ڻ´�����ε�����˿ڵ����ݣ��ÿ���Ϊֵ��������˵�����
		OUT_ELEMENT.SetRate(1);
		OUT_ATTRIBUTE.SetRate(1);
		OUT_CATEGORY.SetRate(1);
		OUT_PRECISION.SetRate(1);
		OUT_INPUTFILE.SetRate(1);

		return true;
	};

	bool Kmeans_in::Initialize()
	{
		return true;
	}

	//Run�������ռ�Ŀ�ľ��ǲ������ݣ�����ֵ����Ӧ�Ķ˿�
	bool Kmeans_in::Run()
	{
		OUT_ELEMENT[0] = NUMBER_OF_ELEMENT;
		OUT_ATTRIBUTE[0] = NUMBER_OF_ATTRIBUTE;
		OUT_CATEGORY[0] = NUMBER_OF_CATEGORY;
		OUT_PRECISION[0] = PRECISION;
		OUT_INPUT.Resize(NUMBER_OF_ELEMENT, NUMBER_OF_ATTRIBUTE);
		OUT_INPUT.Zero();
		FILE* in = fopen(INPUT_FILE,"r");

		for (int i = 0; i < NUMBER_OF_ELEMENT; i++)
		{
			for (int j = 0; j < NUMBER_OF_ATTRIBUTE; j++)
			{
				fscanf(in,"%lf", &OUT_INPUT(i,j));
			}
		}
		OUT_INPUTFILE[0] = OUT_INPUT; 
		fclose(in);
		return true;
	}


	bool Kmeans_in::Finalize() {

		return true;
	}

}