#include "SceneParser.h"

void ParseSceneFromFile(string filepath)
{
	ifstream myFile;
	myFile.open(filepath);
	string currentLine;
	if (myFile.is_open())
	{
		while (!myFile.eof()) 
		{
			getline(myFile, currentLine);
		}
	}
}
