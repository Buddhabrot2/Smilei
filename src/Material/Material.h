#ifndef Material_H
#define Material_H

#include<string>
#include "Profile.h"
#include "Field.h"
#include "Field1D.h"
#include "Field2D.h"
#include "Field3D.h"
#include "Patch.h"



//class ElectroMagn;


class Material{
	public:
		
	Material(Patch *patch_in, unsigned int dims);
	~Material();
		
	Profile* profile;
	
	Patch* patch;
	
	std::string name;
	
	unsigned int n_dimensions;
				
/* 	Field* conductivity_Ex;
	Field* conductivity_Ey;
	Field* conductivity_Ez;
	Field* conductivity_Bx;
	Field* conductivity_By;
	Field* conductivity_Bz; */
	
	std::vector<Field*> allFields;
	
	void initConductivity(std::vector<double> cell_length);
	
};


#endif
