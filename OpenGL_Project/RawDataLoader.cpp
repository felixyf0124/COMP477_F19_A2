#include "RawDataLoader.h"

RawDataLoader::RawDataLoader()
{
}

vector<SkeletonJoint*>* const RawDataLoader::loadRawData(const string path)
{
	string _line;
	//ifstream _file(path);
	ifstream _file(path);
	if (_file.is_open()) 
	{
		printf("file is opened successfully\n");
		vector<SkeletonJoint*>* _raw_data = new vector<SkeletonJoint*>();
		//SkeletonJoint* _joint = nullptr;
		while (getline(_file,_line))
		{
			if (_line.substr(0, 2) == "at")
			{
				continue;
			}
			else if(_line.substr(0, 3) == "end")
			{
				break;
			}
			else
			{
				int _id;
				GLfloat x, y, z;
				int _parent_id;
				if (istringstream(_line) >> _id >> x >> y >> z >> _parent_id)
				{
					SkeletonJoint* _joint = new SkeletonJoint(_id);
					
					if (_parent_id == -1)
					{
						_raw_data->push_back(_joint);
					}
					else if(_parent_id == 0)
					{
						_raw_data->at(_raw_data->size() - 1)->addChild(_joint);
					}
					else
					{
						SkeletonJoint* myParent = _raw_data->at(_raw_data->size() - 1)->getOffspringById(_parent_id);
						
						myParent->addChild(_joint);
					}

					_joint->setPosition(glm::vec3(x, y, z));
				}
			}
			
		}
		
		_file.close();
		
		return _raw_data;

	}
	else
	{
		printf("Impossible to open the file ! Are you in the right path ?\n");

		return nullptr;
	}

	

	
}

vector<SkeletonJoint*>* const RawDataLoader::animaGenerator(vector<SkeletonJoint*>* raw_data)
{

	
	return nullptr;
}
