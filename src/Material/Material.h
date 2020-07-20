#ifndef Material_H
#define Material_H

#include<string>
#include "Profile.h"

class ElectroMagn;

class Material{
	public:
		
	Material();
	~Material();
	
	void applyElectric(ElectroMagn *field);
	
	Profile* profile;
	std::string name;
};


#endif
