
#include "problem1.hpp"
#include "problem2.hpp"
#include "problem3.hpp"
#include "problem4.hpp"
#include <iostream>
using namespace std;

int main()
{
	while (true)
	{
		cout << "Select which problem to run (1, 2, 3, 4)." << endl;
		cout << "Write 0 to exit the program." << endl;

		int probNum;
		cin >> probNum;

		switch (probNum)
		{
		case 1:
			cout << "Running Problem 1: Cube morphing." << endl << "Close the window to select another problem." << endl;
			problem1();
			break;
		case 2:
			cout << "Running Problem 2: Linked structure control." << endl << "Close the window to select another problem." << endl;
			problem2();
			break;
		case 3:
			cout << "Running Problem 3: Particle system." << endl << "Close the window to select another problem." << endl;
			problem3();
			break;
		case 4:
			cout << "Running Problem 4: Mocap replay." << endl << "Close the window to select another problem." << endl;
			problem4();
			break;
		case 0:
			return 0;
		default:
			cout << "Wrong number. Try again." << endl;
		}
	}
}


