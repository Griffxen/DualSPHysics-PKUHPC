/*
 <DUALSPHYSICS>  Copyright (c) 2022 by Dr Jose M. Dominguez et al. (see http://dual.sphysics.org/index.php/developers/). 

 EPHYSLAB Environmental Physics Laboratory, Universidade de Vigo, Ourense, Spain.
 School of Mechanical, Aerospace and Civil Engineering, University of Manchester, Manchester, U.K.

 This file is part of DualSPHysics. 

 DualSPHysics is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License 
 as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
 
 DualSPHysics is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details. 

 You should have received a copy of the GNU Lesser General Public License along with DualSPHysics. If not, see <http://www.gnu.org/licenses/>. 
*/

/// \file CudaTest.cu \brief Implements functions and CUDA kernels for simple test.

#include "CudaTest.h"

namespace cutest{

//==============================================================================
// Returns gridsize according parameters using X dimension.
//==============================================================================
inline dim3 GetSimpleGridSize(unsigned n,unsigned blocksize){
  const unsigned nb=unsigned(n+blocksize-1)/blocksize;//-Total number of blocks.
  return(dim3(nb,1,1));
}

//------------------------------------------------------------------------------
/// Computes r=v^2.
//------------------------------------------------------------------------------
__global__ void KerPow2(int np,const float *v,float *r)
{
  int cp=blockIdx.x*blockDim.x + threadIdx.x;
  if(cp<np)r[cp]=v[cp]*v[cp];
}

//==============================================================================
/// Call cuda kenel to compute r=v^2.
//==============================================================================
void Pow2(int n,const float *v,float *r){
  if(n){
    const unsigned bksize=256;
    dim3 sgrid=GetSimpleGridSize(n,bksize);
    KerPow2 <<<sgrid,bksize>>> (n,v,r);
  }
}

}


