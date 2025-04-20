#include "GameManager.h"  

#include <iostream>  

using namespace std;  

int main()  
{  
   GameManager* gameManager = new GameManager(); 

   try  
   {  
       // Initialize the game manager  
       gameManager->Init("Wrecking Simulation", 1280, 720);  
   }  
   catch (const exception& e)  
   {  
       cout << e.what() << endl;  
       return 0;  
   }  

   gameManager->Start();  

   //delete gameManager; // cleanup to avoid memory leak  
   return 0;  
}