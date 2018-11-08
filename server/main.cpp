#include <iostream>
#include <string>
#include "SpaceTAXI.h"
using namespace std;
int main()
{
	SpaceTAXIServer taxi_system(4000);
	taxi_system.run();

}