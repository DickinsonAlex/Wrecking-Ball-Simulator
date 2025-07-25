#include "PhysicsEngine.h"  

#include <iostream>  

using namespace std;  

int main()  {  
   try  
   {  
       // Initialize the game manager  
       PhysicsEngine::Init("Wrecking Simulation", 1280, 720);
   }  
   catch (const exception& e)  
   {  
       cout << e.what() << endl;  
       return 0;  
   }  

   PhysicsEngine::Start();
   return 0;  
}