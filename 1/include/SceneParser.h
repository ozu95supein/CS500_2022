#pragma once
#include<iostream>
#include<fstream>
#include <sstream>

#include "SphereObject.h"
#include "LightObject.h"
#include "BoxObject.h"
#include "Camera.h"
#include "Scene.h"
#include <string>
#include <vector>
#include "Material.h"
using namespace std;

//these codes are dependant on how the input is formated
string PARSECODE_SPHERE = "SPHERE";
string PARSECODE_DIFFUSE = "DIFFUSE";
string PARSECODE_BOX = "BOX";
string PARSECODE_AMBIENT = "AMBIENT";
string PARSECODE_CAMERA = "CAMERA";
string PARSECODE_LIGHT = "LIGHT";
string PARSECODE_METAL = "METAL";

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
void ParseSceneFromFile(string filepath, SceneStruct& scene)
{
	std::ifstream infile;
	std::string inputLine;
	infile.open(filepath);
	if (!infile)
	{
		std::cerr << "ERRROR: File not Found";
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
	glm::vec3 inputLightCenter;
	glm::vec3 inputLightColor;
	glm::vec3 inputBoxCorner;
	glm::vec3 inputBoxLength;
	glm::vec3 inputBoxWidth;
	glm::vec3 inputBoxHeight;
	Material inputMaterial();
	float inputFloat;
	float inputRadiusFloat;
	float inputRoughness;
	//these will be the vectors filled with the raycasting objects
	vector<SphereObject> inputSpheres;
	vector<BoxObject> inputBoxes;
	vector<LightObject> inputLights;

	glm::vec3 inputAmbient;
	glm::vec3 inputDiffuse;
	glm::vec3 inputMetallic;
	//we will use booleans to keep track of what object we are creating
	bool currentlyMakingSphere = false;	//this starts as false
	bool currentlyMakingLight = false;	//same as above
	bool currentlyMakingBox = false;	//same as above
	MaterialType currentMaterialBeingMade = MaterialType::E_DIFFUSE_MAT;
	for (auto it = lines.begin(); it != lines.end(); ++it)
	{
		//sanity check to skip comments with #
		if ((*it)[0] == '#')
		{
			continue;
		}
		stringstream sstream(*it);
		string firstword;
		string vec3_word;
		string x_str, y_str, z_str;
		getline(sstream, firstword, ' ');
		//debug firstword
		firstword;
		//check the first word to see what it is
		//now that we have the first word, we need to check what it is
		if (firstword == PARSECODE_SPHERE)
		{
			currentlyMakingSphere = true;	//this will be true until we are done making a sphere
			currentlyMakingBox = false;
			currentlyMakingLight = false;
			//if this is a sphere next up is the vec3 sphere center
			//get the next word
			getline(sstream, vec3_word, ' ');
			//pass the word into our vec3 extractor
			inputSphereCenter = ExtractVector(vec3_word);	//spherecenter
			//now we get the next word to get the scale
			getline(sstream, vec3_word, ' ');
			inputRadiusFloat = stof(vec3_word);
		}
		else if (firstword == PARSECODE_LIGHT)
		{
			//if we get this we are doing stuff with the light
			currentlyMakingLight = true;	//this will be true until we are done making a Light
			currentlyMakingBox = false;
			currentlyMakingSphere = false;
			//Light has position, radius, light color
			//get the next word
			getline(sstream, vec3_word, ' ');
			//pass the word into our vec3 extractor
			inputLightCenter = ExtractVector(vec3_word);	//lightcenter
			//now we get the next word to get the scale of the light
			getline(sstream, vec3_word, ' ');
			inputRadiusFloat = stof(vec3_word);	//radius
			//get the next word
			getline(sstream, vec3_word, ' ');
			//pass the word into our vec3 extractor
			inputLightColor = ExtractVector(vec3_word);		//lightcolor

			//since lights dont have info on a second line, we construct the light
			//	then push it back
			inputLights.push_back(LightObject(inputLightCenter, inputLightColor, inputRadiusFloat));
			currentlyMakingLight = false;	//this will be true until we are done making a Light
		}
		else if (firstword == PARSECODE_BOX)
		{
			//if we get this we are doing stuff with the Box
			currentlyMakingBox = true;	//this will be true until we are done making a Box
			currentlyMakingSphere = false;
			currentlyMakingLight = false;

			// BoxCorner, length, width, height, diffuse
			//get the next word, corner
			getline(sstream, vec3_word, ' ');
			inputBoxCorner = ExtractVector(vec3_word);
			//get the next word, length
			getline(sstream, vec3_word, ' ');
			inputBoxLength = ExtractVector(vec3_word);
			//get the next word, width
			getline(sstream, vec3_word, ' ');
			inputBoxWidth = ExtractVector(vec3_word);
			//get the next word, height
			getline(sstream, vec3_word, ' ');
			inputBoxHeight = ExtractVector(vec3_word);
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
			//get the next word
			getline(sstream, vec3_word, ' ');
			//pass the word into our vec3 extractor
			inputDiffuse = ExtractVector(vec3_word);
			currentMaterialBeingMade = MaterialType::E_DIFFUSE_MAT;
		}
		else if (firstword == PARSECODE_METAL)	//metalic material
		{
			//get the next word
			getline(sstream, vec3_word, ' ');
			//pass the word into our vec3 extractor
			inputMetallic = ExtractVector(vec3_word);	//box metallic
			//now we get the next word to get the scale
			getline(sstream, vec3_word, ' ');
			inputRoughness = stof(vec3_word);
			currentMaterialBeingMade = MaterialType::E_METALLIC_MAT;
		}
		else if (firstword == "")
		{
			if (currentlyMakingSphere == true)
			{
				if (currentMaterialBeingMade == MaterialType::E_DIFFUSE_MAT)
				{
					//create a sphere using the current values inside and add to the vector
					SphereObject sph = SphereObject(inputSphereCenter, inputDiffuse, inputRadiusFloat);
					sph.SetMaterial(Material(inputDiffuse, 0.0f, MaterialType::E_DIFFUSE_MAT));
					inputSpheres.push_back(sph);
					currentlyMakingSphere = false; //reset this to false;
				}
				else if (currentMaterialBeingMade == MaterialType::E_METALLIC_MAT)
				{
					//create a sphere using the current values inside and add to the vector
					SphereObject sph = SphereObject(inputSphereCenter, inputMetallic, inputRadiusFloat);
					sph.SetMaterial(Material(inputMetallic, inputRoughness, MaterialType::E_METALLIC_MAT));
					inputSpheres.push_back(sph);
					currentlyMakingSphere = false; //reset this to false;
				}
				else
				{
					//create a sphere using the current values inside and add to the vector
					SphereObject sph = SphereObject(inputSphereCenter, inputDiffuse, inputRadiusFloat);
					sph.SetMaterial(Material(inputDiffuse, 0.0f, MaterialType::E_DIFFUSE_MAT));
					inputSpheres.push_back(sph);
					currentlyMakingSphere = false; //reset this to false;
				}
			}
			else if (currentlyMakingBox == true)
			{
				if (currentMaterialBeingMade == MaterialType::E_DIFFUSE_MAT)
				{
					//BoxObject(glm::vec3 c, glm::vec3 l, glm::vec3 w, glm::vec3 h, glm::vec3 mat_diff);
					BoxObject b = BoxObject(inputBoxCorner, inputBoxLength, inputBoxWidth, inputBoxHeight, inputDiffuse);
					b.SetMaterial(Material(inputDiffuse, 0.0f, MaterialType::E_DIFFUSE_MAT));
					inputBoxes.push_back(b);
					currentlyMakingBox = false;
				}
				else if (currentMaterialBeingMade == MaterialType::E_METALLIC_MAT)
				{
					//create a sphere using the current values inside and add to the vector
					BoxObject b = BoxObject(inputBoxCorner, inputBoxLength, inputBoxWidth, inputBoxHeight, inputDiffuse);
					b.SetMaterial(Material(inputDiffuse, inputRoughness, MaterialType::E_METALLIC_MAT));
					inputBoxes.push_back(b);
					currentlyMakingSphere = false; //reset this to false;
				}
				else
				{
					//BoxObject(glm::vec3 c, glm::vec3 l, glm::vec3 w, glm::vec3 h, glm::vec3 mat_diff);
					BoxObject b = BoxObject(inputBoxCorner, inputBoxLength, inputBoxWidth, inputBoxHeight, inputDiffuse);
					b.SetMaterial(Material(inputDiffuse, 0.0f, MaterialType::E_DIFFUSE_MAT));
					inputBoxes.push_back(b);
					currentlyMakingBox = false;
				}
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
	inputScene.mSceneBoxes = inputBoxes;
	inputScene.mSceneLights = inputLights;
	scene = inputScene;
	return;
}