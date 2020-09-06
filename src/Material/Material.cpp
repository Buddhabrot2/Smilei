#include "Material.h"

Material::Material(Patch* patch_in, unsigned int dims){
	patch = patch_in;
	n_dimensions = dims;
}

Material::~Material(){
}

void Material::initConductivity(std::vector<double> cell_length){
		//store the conductivity from the profile in the various fields
		//this directly follows the applyExternalField methods for the specific dimensionality
		
		for(unsigned int ifield=0; ifield<allFields.size(); ifield++){
			
			if(n_dimensions == 1){
				
				double dx = cell_length[0];

				//Field1D *field1D=static_cast<Field1D *>( allFields[ifield] );
				
				std::vector<double> pos( 1 );
				pos[0] = dx * ( ( double )( patch->getCellStartingGlobalIndex( 0 ) )+( allFields[ifield] ->isDual( 0 )?-0.5:0. ) );
				int N = ( int )allFields[ifield] ->dims()[0];
				
				// USING UNSIGNED INT CREATES PB WITH PERIODIC BCs
				for( int i=0 ; i<N ; i++ ) {
					( *allFields[ifield]  )( i ) += profile->valueAt( pos ); 
					pos[0] += dx;
				}
				
			} 
			else if(n_dimensions == 2){
				
				double dx = cell_length[0];
				double dy = cell_length[1];
				
				//Field2D *field2D=static_cast<Field2D *>( allFields[ifield] );
				
				std::vector<double> pos( 2, 0 );
				pos[0]      = dx*( ( double )( patch->getCellStartingGlobalIndex( 0 ) )+( allFields[ifield] ->isDual( 0 )?-0.5:0. ) );
				double pos1 = dy*( ( double )( patch->getCellStartingGlobalIndex( 1 ) )+( allFields[ifield] ->isDual( 1 )?-0.5:0. ) );
				int N0 = ( int ) allFields[ifield] ->dims()[0];
				int N1 = ( int ) allFields[ifield] ->dims()[1];
				
				// UNSIGNED INT LEADS TO PB IN PERIODIC BCs
				for( int i=0 ; i<N0 ; i++ ) {
					pos[1] = pos1;
					for( int j=0 ; j<N1 ; j++ ) {
						( *allFields[ifield]  )( i, j ) += profile->valueAt( pos );
						if (( *allFields[ifield]  )( i, j ) >0){
							std::cout<<"Wrote conductivity " << ( *allFields[ifield]  )( i, j ) << " to "<<allFields[ifield] ->name <<" at    " << i << "    "<< j << "\n";
						}
						pos[1] += dy;
					}
					pos[0] += dx;
				}
							
			}
			else if(n_dimensions == 3){
				
				double dx = cell_length[0];
				double dy = cell_length[1];
				double dz = cell_length[2];
				
				//Field3D *field3D = static_cast<Field3D *>( allFields[ifield] );
    
				std::vector<bool> dual(3, false);
				if ( ( allFields[ifield] ->name.substr(0,2) == "Jx" ) || ( allFields[ifield] ->name.substr(0,2) == "Ex" ) )
					dual[0] = true;
				if ( ( allFields[ifield] ->name.substr(0,2) == "Jy" ) || ( allFields[ifield] ->name.substr(0,2) == "Ey" ) )
					dual[1] = true;
				if ( ( allFields[ifield] ->name.substr(0,2) == "Jz" ) || ( allFields[ifield] ->name.substr(0,2) == "Ez" ) )
					dual[2] = true;

				if ( allFields[ifield] ->name.substr(0,2) == "Bx" ) {
					dual[1] = true;
					dual[2] = true;
				}
				if ( allFields[ifield] ->name.substr(0,2) == "By" ) {
					dual[0] = true;
					dual[2] = true;
				}
				if ( allFields[ifield] ->name.substr(0,2) == "Bz" ) {
					dual[0] = true;
					dual[1] = true;
				}

				std::vector<double> pos( 3 );
				pos[0]      = dx*( ( double )( patch->getCellStartingGlobalIndex( 0 ) )+( dual[0]?-0.5:0. ) );
				double pos1 = dy*( ( double )( patch->getCellStartingGlobalIndex( 1 ) )+( dual[1]?-0.5:0. ) );
				double pos2 = dz*( ( double )( patch->getCellStartingGlobalIndex( 2 ) )+( dual[2]?-0.5:0. ) );
				int N0 = ( int )allFields[ifield] ->dims()[0];
				int N1 = ( int )allFields[ifield] ->dims()[1];
				int N2 = ( int )allFields[ifield] ->dims()[2];
				
				// UNSIGNED INT LEADS TO PB IN PERIODIC BCs
				// Create the x,y,z maps where profiles will be evaluated
				std::vector<Field *> xyz( 3 );
				std::vector<unsigned int> n_space_to_create( 3 );
				n_space_to_create[0] = N0;
				n_space_to_create[1] = N1;
				n_space_to_create[2] = N2;
				
				for( unsigned int idim=0 ; idim<3 ; idim++ ) {
					xyz[idim] = new Field3D( n_space_to_create );
				}
				
				for( int i=0 ; i<N0 ; i++ ) {
					pos[1] = pos1;
					for( int j=0 ; j<N1 ; j++ ) {
						pos[2] = pos2;
						for( int k=0 ; k<N2 ; k++ ) {
							(*allFields[ifield] )(i,j,k) += profile->valueAt(pos);
							for( unsigned int idim=0 ; idim<3 ; idim++ ) {
								( *xyz[idim] )( i, j, k ) = pos[idim];
							}
							pos[2] += dz;
						}
						pos[1] += dy;
					}
					pos[0] += dx;
				}
				
				profile->valuesAt( xyz, *allFields[ifield]  );
				
				for( unsigned int idim=0 ; idim<3 ; idim++ ) {
					delete xyz[idim];
				}
				
			}				
		}
}
		
		void Material::printOutConductivity(){
			
			for(unsigned int ifield=0; ifield<allFields.size(); ifield++){
			
				if(n_dimensions == 2){
					
					
					Field2D *field2D=static_cast<Field2D *>( allFields[ifield] );
					
					int N0 = ( int ) field2D ->dims()[0];
					int N1 = ( int ) field2D ->dims()[1];
					
					bool nopositive=true;
					
					// UNSIGNED INT LEADS TO PB IN PERIODIC BCs
					for( int i=0 ; i<N0 ; i++ ) {
						for( int j=0 ; j<N1 ; j++ ) {
							if (( *allFields[ifield]  )( i, j ) >0){
								std::cout<<"Conductivity is " << ( *field2D  )( i, j ) << " in "<<field2D ->name <<" at    " << i << "    "<< j << "\n";
								nopositive = false;
							}
						}
					}
					if(nopositive)
						std::cout<<"No postive element in " << field2D ->name  <<"\n";
				}
			
			}

}

	

