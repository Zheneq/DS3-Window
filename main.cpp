#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <windows.h>
#include <direct.h>
#include <string>
#include <vector>
#include <random>
#include "../../DS3/Inih/cpp/INIREADER.h"
#include "field.h"

using namespace std;

double lt, ts;
field *data;
vector<double> centr, halfw;

FILE *GetFile(char *name)
{
	FILE *f = fopen(name, "w");
	return f;
}

void Init(int argc, char **argv)
{
	INIReader *config = NULL;
	const unsigned len = 512;
	char Path[len];

	// Если не задан файл конфигурации, ищем дефолтный
	if (argc == 1)
	{
		strcpy(Path, argv[0]); //strcpy_s(Path, strlen(argv[0]), argv[0]);
		for (int i = strlen(argv[0]); i; --i) if (Path[i] == '\\' || Path[i] == '/') { Path[i] = '\0'; break; }
		strcat(Path, "\\default.ini");
	}
	else
	{
		strcpy(Path, argv[1]);
	}
	printf("%s\n", Path);

	config = new INIReader(Path);

	if (config->ParseError() < 0)
	{
		throw("Can't load ini file");
	}

	//std::cout << "Config loaded from 'test.ini': version="
	//	<< reader.GetInteger("protocol", "version", -1) << ", name="
	//	<< reader.Get("user", "name", "UNKNOWN") << ", email="
	//	<< reader.Get("user", "email", "UNKNOWN") << ", pi="
	//	<< reader.GetReal("user", "pi", -1) << ", active="
	//	<< reader.GetBoolean("user", "active", true) << "\n";


	lt = config->GetReal("Data", "Time", 0.0);
	ts = config->GetReal("Data", "Step", 0.0);
	int nt = ((int)(lt / ts)) + 1;

	for (int i = 0; ; ++i)
	{
		char a[64], b[64];
		sprintf(a, "centr%d", i);
		sprintf(b, "halfw%d", i);

		if (config->GetReal("Data", a, INFINITY) == INFINITY) break;

		centr.push_back(config->GetReal("Data", a, -1.0));
		halfw.push_back(config->GetReal("Data", b, -1.0));
	}

	// Инициализация классов поля
	data = new field();
	data->Init(nt, FFTW_ESTIMATE);

	FILE *f = fopen(config->Get("Data", "DataPath", "").c_str(), "rt");
	for (int i = 0; i < nt; ++i)
	{
		fscanf(f, " %*lf %lf", &data->data[i]);
	}
	fclose(f);
}

int main(int argc, char **argv)
{
	try
	{
		Init(argc, argv);
	}
	catch (char *error)
	{
		printf(error);
		return 1;
	}
	return 0;
}