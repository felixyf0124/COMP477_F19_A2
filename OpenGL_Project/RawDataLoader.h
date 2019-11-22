#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "SkeletonJoint.h"
#include "glm\glm.hpp"

using namespace std;

class RawDataLoader
{
public:
	RawDataLoader();
	vector<SkeletonJoint*>* const loadRawData(const string path);

	vector<SkeletonJoint*>* const animaGenerator(vector<SkeletonJoint*>* raw_data);
};

