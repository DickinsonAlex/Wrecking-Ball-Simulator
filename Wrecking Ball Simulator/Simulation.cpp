#include "GameManager.h"

#include <iostream>

using namespace std;

int main()
{
	try
	{
		GameManager::Init("Destruction", 1280, 720);
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
		return 0;
	}

	GameManager::Start();

	return 0;
}