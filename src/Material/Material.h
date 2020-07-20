#include<String>
#include "Profile.h"

class ElectroMagn;

class Material
{
	public:
	
	void apply(ElectroMagn *field);
	
	Pofile profile;
	std::string name;
}