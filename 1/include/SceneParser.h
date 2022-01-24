#pragma once
#include<iostream>
#include<fstream>
#include <sstream>

#include "SphereObject.h"
#include "Camera.h"
#include "Scene.h"
#include <string>
#include <vector>
using namespace std;

//these codes are dependant on how the input is formated
string PARSECODE_SPHERE = "SPHERE";
string PARSECODE_DIFFUSE = "DIFFUSE";
string PARSECODE_BOX = "BOX";
string PARSECODE_AMBIENT = "AMBIENT";
string PARSECODE_CAMERA = "CAMERA";

glm::vec3 ExtractVector(string input)
{
	glm::vec3 result;
	//iterate through the input to get the indices of the dividing chars
	//dividing chars: "(", ",", ")"

	//Make a substring from input removing the ( and ) characters
	int inputsize = input.size();
	string sub_input = input.substr(1, (inputsize-2));

	istringstream mstream(sub_input);
	string currentNum;
	int i = 0;
	while (std::getline(mstream, currentNum, ','))
	{
		result[i] = stof(currentNum);
		i++;
	}

	return result;
}
void ParseSceneFromFile(string filepath, SceneStruct & scene) 
{
	std::ifstream infile;
	std::string inputLine;
	infile.open(filepath);
	if (!infile)
	{
		std::cout << "ERRROR: File not Found";
		return;

	}
	std::string input;
	std::vector<std::string> lines;
	while (getline(infile, inputLine))//return file
	{
		lines.push_back(inputLine);
	}
	infile.close();
	//temp variables read from the lines word by word
	Camera inputCamera;
	glm::vec3 inputSphereCenter;
	float inputFloat;
	vector<SphereObject> inputSpheres;
	glm::vec3 inputAmbient;
	glm::vec3 inputDiffuse;
	//once I have all the lines in an array of strings I parse them one by one
	//iterate over the array of strings and subdivide into words
	vector<string> words;
	string space_delimeter = " ";
	int space_delim_pos = 0;
	//we will use booleans to keep track of what object we are creating
	bool currentlyMakingSphere = false;	//this starts as false
	for (auto it = lines.begin(); it != lines.end(); ++it)
	{
		//sanity check to skip comments
		if ((*it)[0] == '#')
		{
			continue;
		}
		stringstream sstream(*it);
		string firstword;
		string vec3_word;
		string x_str, y_str, z_str;
		getline(sstream, firstword, ' ');
		firstword;
		//now that we have the first word, we need to check what it is
		if (firstword == PARSECODE_SPHERE)
		{
			currentlyMakingSphere = true;	//this will be true until we are done making a sphere
			//if this is a sphere next up is the vec3 sphere center
			//get the next word
			getline(sstream, vec3_word, ' ');
			//pass the word into our vec3 extractor
			inputSphereCenter = ExtractVector(vec3_word);	//spherecenter
			//now we get the next word to get the scale
			getline(sstream, vec3_word, ' ');
			inputFloat = stof(vec3_word);
		}
		else if (firstword == PARSECODE_AMBIENT)
		{
			getline(sstream, vec3_word, ' ');
			inputAmbient = ExtractVector(vec3_word);
		}
		else if (firstword == PARSECODE_CAMERA)
		{
			//pos
			getline(sstream, vec3_word, ' ');
			glm::vec3 v = ExtractVector(vec3_word);
			inputCamera.SetCPosition(v);
			//target
			getline(sstream, vec3_word, ' ');
			v = ExtractVector(vec3_word);
			inputCamera.SetTPosition(v);
			//up
			getline(sstream, vec3_word, ' ');
			v = ExtractVector(vec3_word);
			inputCamera.SetUpVector(v);
			//focal length
			getline(sstream, vec3_word, ' ');
			float f = stof(vec3_word);
			inputCamera.SetFocalLength(f);
		}
		else if (firstword == PARSECODE_DIFFUSE)
		{
			//if we get this, we have been doing stuff with SPHERE
			//get the next word
			getline(sstream, vec3_word, ' ');
			//pass the word into our vec3 extractor
			inputDiffuse = ExtractVector(vec3_word);	//sphere diffuse
		}
		else if (firstword == "")
		{
			if (currentlyMakingSphere == true)
			{
				//create a sphere using the current values inside and add to the vector
				auto sph = SphereObject(inputSphereCenter, inputDiffuse, inputFloat);
				inputSpheres.push_back(sph);
				currentlyMakingSphere = false; //reset this to false;
			}
		}
		//unknown, continue
		else 
		{
			std::cout << "unknown parsecode" << std::endl;
			continue;
		}
	}
	//return all of the elements of the scene in the parameter struct
	SceneStruct inputScene;
	inputScene.mSceneAmbient = inputAmbient;
	inputScene.mSceneCamera = inputCamera;
	inputScene.mSceneSpheres = inputSpheres;
	scene = inputScene;
	return;
}
