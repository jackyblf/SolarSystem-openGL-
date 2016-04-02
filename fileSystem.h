#ifndef FILESYSTEM_H
#define FILESYSTEM
#include "base.h"
#include "planet.h"

class fileSystem
{
	FILE *file;

public:

	fileSystem() : file(NULL){}

	~fileSystem(){ closeFile(); }

	bool beginWrite(char * fileName)
	{
		file = fopen(fileName, "w");
		if (!file)
		{
			return false;
		}
		return true;
	}

	void writeInt(int value)
	{
		fprintf(file, "%d ", value);
		printf("the value is %d\n", value);
	}

	void writeDataFrom(planet *p)
	{
		fprintf(file, "%.6f %.6f %.6f %.6f %.6f %.6f ", p->aroundRotatedSpeed, p->selfRotatedSpeed, p->radius, p->pos.x, p->pos.y, p->pos.z);
	}

	bool beginRead(char * fileName)
	{
		file = fopen(fileName, "r");
		if (!file)
		{
			return false;
		}
		return true;
	}

	void readInt(int *value)
	{
		fscanf(file, "%d ", value);
	}

	void readDataTo(planet *p)
	{
		fscanf(file, "%.6f %.6f %.6f %.6f %.6f %.6f ", p->aroundRotatedSpeed, p->selfRotatedSpeed, p->radius, p->pos.x, p->pos.y, p->pos.z);
	}

	void closeFile()
	{
		if (file)
		{
			fclose(file);
			file = NULL;
		}
	}
};
#endif