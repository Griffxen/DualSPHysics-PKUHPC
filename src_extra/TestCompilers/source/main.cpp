/*
 <DUALSPHYSICS>  Copyright (c) 2024, 
 Dr Jose M. Dominguez Alonso, Prof. Alejandro Crespo, 
 Dr Georgios Fourtakas, Prof. Benedict Rogers, 
 Dr Renato Vacondio, Dr Corrado Altomare, 
 Dr Angelo Tafuni, Dr Orlando Garcia Feal, 
 Ivan Martinez Estevez, Dr Joseph O'Connor, 
 Dr Aaron English, Dr Francesco Ricci,
 Prof. Moncho Gomez Gesteira, Prof. Peter Stansby

 EPHYSLAB Environmental Physics Laboratory, Universidade de Vigo, Ourense, Spain.
 School of Mechanical, Aerospace and Civil Engineering, University of Manchester, Manchester, U.K.

 This file is part of DualSPHysics. 

 DualSPHysics is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License 
 as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
 
 DualSPHysics is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details. 

 You should have received a copy of the GNU Lesser General Public License along with DualSPHysics. If not, see <http://www.gnu.org/licenses/>. 

 Free download source files and documentation from DualSPHysics website (http://dual.sphysics.org).
 Full list of developers and contributors is available at https://dual.sphysics.org/developers
 Properly cite the use of DualSPHysics (Dominguez et al., 2022) (https://dual.sphysics.org/references).
*/

/** \mainpage DualSPHysics Documentation
\section main-des Description
DualSPHysics is based on the Smoothed Particle Hydrodynamics <a href="http://www.sphysics.org">SPHysics code.</a> \n
The package is a set of C++ and CUDA codes. \n
DualSPHysics is developed to deal with real-life engineering problems <a href="http://www.youtube.com/user/DualSPHysics">DualSPHysics animations.</a> \n

EPHYSLAB Environmental Physics Laboratory, Universidade de Vigo, Ourense, Spain. \n
School of Mechanical, Aerospace and Civil Engineering, University of Manchester, Manchester, U.K.
\section compile_sec Project files
Please download source files and documentation from <a href="http://dual.sphysics.org">DualSPHysics website.</a> \n
\author <a href="http://dual.sphysics.org/index.php/developers">DualSPHysics Developers.</a> 
\version 5.4.322
\date 06-03-2024
\copyright GNU Lesser General Public License <a href="http://www.gnu.org/licenses/">GNU licenses.</a>
*/

/// \file main.cpp \brief Main file of the project that executes the code on CPU or GPU.

#include "JVtkLib.h"
#include "OmpDefs.h"
#include <string>
#include <cstring>
#ifdef _WITHGPU
  #include "CudaTest.h"
#endif

using namespace std;

//==============================================================================
// Saves VTK with results.
//==============================================================================
void SaveVtk(std::string fname,int np,const float *vh,const float *rh){
  printf("Save %s\n",fname.c_str());
  JVtkLib vtk;
  for(int cp=1;cp<np;cp++){
    vtk.AddShapeLine(TFloat3(vh[cp-1],0,rh[cp-1]),TFloat3(vh[cp],0,rh[cp]),cp-1);
  }
  vtk.SaveShapeVtk(fname,"cp");
}

#ifdef _WITHGPU
//==============================================================================
// Computes vh^2 on GPU and saves results in rh.
//==============================================================================
void RunGPU(int gpuid,int np,const float *vh,float *rh){
  printf("\nGPU execution:\n");
  cudaSetDevice(gpuid);
  //-Shows information of GPU selection.
  cudaDeviceProp devp;
  int dev;
  cudaGetDevice(&dev);
  cudaGetDeviceProperties(&devp,dev);
  if(gpuid!=dev)throw "Requested GPU is not available.";
  printf("  Device: %d \"%s\"\n",gpuid,devp.name);
  printf("  Compute capability: %.1f\n",float(devp.major*10+devp.minor)/10);
  printf("  Memory global: %d MB\n",int(devp.totalGlobalMem/(1024*1024)));
  printf("  Memory shared: %u Bytes\n",unsigned(devp.sharedMemPerBlock));
  //-Allocates gpu memory.
  float *vg=NULL,*rg=NULL;
  cudaMalloc((void**)&vg,sizeof(float)*np);
  cudaMalloc((void**)&rg,sizeof(float)*np);
  //-Copy input data to gpu memory.
  cudaMemcpy(vg,vh,sizeof(float)*np,cudaMemcpyHostToDevice);
  cudaMemset(rg,0,sizeof(float)*np);
  //-Run cuda kenel.
  cutest::Pow2(np,vg,rg);
  if(cudaGetLastError()!=cudaSuccess)throw "CUDA error...";
  //-Copy results to cpu memory.
  cudaMemcpy(rh,rg,sizeof(float)*np,cudaMemcpyDeviceToHost);
  //-Free GPU memory.
  cudaFree(vg); vg=NULL;
  cudaFree(rg); rg=NULL;
  //-Destroy all allocations and reset all state on the current device.
  cudaDeviceReset();
}
#endif

//==============================================================================
//==============================================================================
int main(int argc, char** argv){
  int errcode=1;
  printf("\nDSTestCompilers v1.01 (02-03-2024)\n");
  printf("==================================\n");
  try{
    const int np=100;
    float *vh=new float[np];
    float *rh=new float[np];
    for(int cp=0;cp<np;cp++)vh[cp]=(1.f/(np-1))*cp;

    //-CPU execution.
    #ifdef OMP_USE
      omp_set_num_threads(0);
      printf("CPU execution with OpenMP using %d threads.\n",omp_get_max_threads());
    #else
      printf("CPU execution (single thread).\n");
    #endif
    #ifdef OMP_USE
      #pragma omp parallel for schedule (static)
    #endif
    for(int cp=0;cp<np;cp++)rh[cp]=vh[cp]*vh[cp];
    SaveVtk("TestCompilers_Curve_CPU.vtk",np,vh,rh);

    //-GPU execution.
    #ifdef _WITHGPU
      memset(rh,0,sizeof(float)*np);
      const int gpuid=0;
      RunGPU(gpuid,np,vh,rh);
      SaveVtk("TestCompilers_Curve_GPU.vtk",np,vh,rh);
    #endif

    //-Free memory.
    delete[] vh; vh=NULL;
    delete[] rh; rh=NULL;
    errcode=0;
  }
  catch(const char *cad){
    printf("\n*** Exception(chr): %s",cad);
  }
  catch(const string &e){
    printf("\n*** %s\n",e.c_str());
  }
  catch (const exception &e){
    printf("\n*** %s\n",e.what());
  }
  catch(...){
    printf("\n*** Attention: Unknown exception...");
  }
  printf("\nFinished execution (code=%d).\n",errcode);
  return(errcode);
}


