#ifndef Material_H
#define Material_H

#include<string>
#include "Profile.h"
#include "Field.h"

class ElectroMagn;



class Material{
	public:
		
	Material(unsigned int dims);
	~Material();
		
	Profile* profile;
	
	std::string name;
	
	unsigned int n_dimensions;
			
};


#endif
