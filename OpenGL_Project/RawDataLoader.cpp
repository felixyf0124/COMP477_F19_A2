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

void RawDataLoader::animaTestDumper(vector<SkeletonJoint*>* joint_18_set)
{
	vector<string>* _animaData = new vector<string>();

	for (GLuint i = 0; i < joint_18_set->size(); ++i)
	{

		SkeletonJoint* _root = joint_18_set->at(i)->getRootJoint();

		_root->cookAllQuaternion();

		
	}

	for (GLuint i = 0; i < joint_18_set->size()-1; ++i)
	{

		SkeletonJoint* _root = joint_18_set->at(0)->getRootJoint();
		SkeletonJoint* _root_next = joint_18_set->at(i)->getRootJoint();


		glm::quat _quat_r = _root->getQuatLocal();
		glm::quat _quat_r_next = _root_next->getQuatLocal();
		string _frame_data = to_string(i) + " "
			+ to_string(_quat_r_next.w- _quat_r.w) + " "
			+ to_string(_quat_r_next.x- _quat_r.w) + " "
			+ to_string(_quat_r_next.y- _quat_r.w) + " "
			+ to_string(_quat_r_next.z- _quat_r.w);
		//cout << _quat_r.w<<"|"<< _quat_r.x<<"|" << _quat_r.y << endl;
		for (GLuint j = 1; j < _root->getAllJoints()->size(); ++j)
		{

			SkeletonJoint* _joint = _root->getOffspringById(j);
			SkeletonJoint* _joint_next = _root_next->getOffspringById(j);
			//_joint->cookAllQuaternion();

			glm::quat _quat = _joint->getQuatLocal();
			glm::quat _quat_next = _joint_next->getQuatLocal();
			//cout << _quat.w << "|" << _quat.x << "|" << _quat.y << endl;

			_frame_data += " " + to_string(_quat_next.w-_quat.w)
				+ " " + to_string(_quat_next.x-_quat.x)
				+ " " + to_string(_quat_next.y-_quat.y)
				+ " " + to_string(_quat_next.z-_quat.z);
		}
		_frame_data += "\n";
		//cout << _frame_data;
		_animaData->push_back(_frame_data);
	}

	//time_t _t_now = time(0);
	//string dt = ctime(&_t_now);
	//string _fileName = "sample_" + dt + ".anima";
	string _fileName = "A2_sample.anima";
	//_fileName.replace(_fileName.begin(), _fileName.end(), " ", "_");

	std::ofstream _file;

	_file.open(_fileName);
	for (GLuint i = 0; i < _animaData->size(); ++i)
	{
		_file << _animaData->at(i);
	}
	
	_file.close();
	
}
