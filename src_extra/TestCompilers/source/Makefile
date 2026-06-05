#DSTestCompilers GPU/CPU v1.01 05-03-2024

#=============== Compilation Options (YES/NO) ===============
USE_DEBUG=NO
USE_FAST_MATH=YES
USE_NATIVE_CPU_OPTIMIZATIONS=NO
COMPILE_VTKLIB=YES

LIBS_DIRECTORIES=-L./
LIBS_DIRECTORIES:=$(LIBS_DIRECTORIES) -L../lib/linux_gcc

EXECNAME=DSTestCompilers_linux64
EXECS_DIRECTORY=../bin

# -std=c++0x ---> Used to avoid errors for calls to enums
ifeq ($(USE_DEBUG), YES)
  CCFLAGS=-c -O0 -g -Wall -fopenmp -D_WITHGPU -std=c++0x
else
  CCFLAGS=-c -O3 -fopenmp -D_WITHGPU -std=c++0x
  ifeq ($(USE_FAST_MATH), YES)
    CCFLAGS+= -ffast-math
  endif
  ifeq ($(USE_NATIVE_CPU_OPTIMIZATIONS), YES)
    CCFLAGS+= -march=native
  endif
endif
CC=g++
CCLINKFLAGS=-fopenmp -lgomp

ifeq ($(COMPILE_VTKLIB), NO)
  CCFLAGS:=$(CCFLAGS) -DDISABLE_VTKLIB
endif

#=============== CUDA selection ===============
CUDAVER=11

#=============== CUDA toolkit directory (make appropriate for local CUDA installation) ===============
ifeq ($(CUDAVER),00)
  DIRTOOLKIT=/usr/local/cuda
endif
ifeq ($(CUDAVER),75)
  DIRTOOLKIT=/exports/opt/NVIDIA/cuda-7.5
endif
ifeq ($(CUDAVER),91)
  DIRTOOLKIT=/exports/opt/NVIDIA/cuda-9.1
endif
ifeq ($(CUDAVER),92)
  DIRTOOLKIT=/exports/opt/NVIDIA/cuda-9.2
endif
ifeq ($(CUDAVER),11)
  DIRTOOLKIT=/exports/opt/NVIDIA/cuda-11.7
endif

#=============== Select GPU architectures ===============
ifeq ($(CUDAVER),00)
  GENCODE:=$(GENCODE) -gencode=arch=compute_20,code=\"sm_20,compute_20\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_30,code=\"sm_30,compute_30\"
endif
ifeq ($(CUDAVER),75)
  GENCODE:=$(GENCODE) -gencode=arch=compute_20,code=\"sm_20,compute_20\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_30,code=\"sm_30,compute_30\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_35,code=\"sm_35,compute_35\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_37,code=\"sm_37,compute_37\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_50,code=\"sm_50,compute_50\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_52,code=\"sm_52,compute_52\"
endif
ifeq ($(CUDAVER),91)
  GENCODE:=$(GENCODE) -gencode=arch=compute_30,code=\"sm_30,compute_30\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_35,code=\"sm_35,compute_35\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_50,code=\"sm_50,compute_50\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_52,code=\"sm_52,compute_52\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_61,code=\"sm_61,compute_61\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_70,code=\"sm_70,compute_70\"
endif
ifeq ($(CUDAVER),92)
  # module load cuda/9.2
  GENCODE:=$(GENCODE) -gencode=arch=compute_30,code=\"sm_30,compute_30\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_35,code=\"sm_35,compute_35\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_50,code=\"sm_50,compute_50\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_52,code=\"sm_52,compute_52\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_61,code=\"sm_61,compute_61\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_70,code=\"sm_70,compute_70\"
endif
ifeq ($(CUDAVER),11)
  # module load cuda/11.7
  # GENCODE:=$(GENCODE) -gencode=arch=compute_30,code=\"sm_30,compute_30\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_35,code=\"sm_35,compute_35\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_50,code=\"sm_50,compute_50\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_52,code=\"sm_52,compute_52\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_61,code=\"sm_61,compute_61\"
  GENCODE:=$(GENCODE) -gencode=arch=compute_70,code=\"sm_70,compute_70\"
endif

#=============== Files to compile ===============
OBJECTS=CudaTest.o Functions.o FunGeo3d.o JDataArrays.o JException.o JObject.o JRangeFilter.o main.o

#=============== DualSPHysics libs to be included ===============
JLIBS=${LIBS_DIRECTORIES}
ifeq ($(COMPILE_VTKLIB), YES)
  JLIBS:=$(JLIBS) -ljvtklib_64
endif

#=============== GPU Code Compilation ===============
CCFLAGS := $(CCFLAGS) -I./ -I$(DIRTOOLKIT)/include
CCLINKFLAGS := $(CCLINKFLAGS) -L$(DIRTOOLKIT)/lib64 -lcudart_static -ldl -lrt
NCC=nvcc
ifeq ($(USE_DEBUG), NO)
  NCCFLAGS+=-c $(GENCODE) -O3 -ccbin $(CC)
else
  NCCFLAGS+=-c $(GENCODE) -O0 -ccbin $(CC) -g
endif
ifeq ($(USE_FAST_MATH), YES)
  NCCFLAGS+= -use_fast_math
endif
ifeq ($(CUDAVER),11)	
  NCCFLAGS+= -Wno-deprecated-gpu-targets
endif

all:$(EXECS_DIRECTORY)/$(EXECNAME)
	rm -rf *.o
ifeq ($(USE_DEBUG), NO)
	@echo "  --- Compiled Release GPU version ---"
else
	@echo "  --- Compiled Debug GPU version ---"
	mv $(EXECS_DIRECTORY)/$(EXECNAME) $(EXECNAME)_debug
endif

$(EXECS_DIRECTORY)/$(EXECNAME):  $(OBJECTS)
	$(CC) $(OBJECTS) $(CCLINKFLAGS) -o $@ $(JLIBS)

.cpp.o: 
	$(CC) $(CCFLAGS) $< 

CudaTest.o: CudaTest.cu
	$(NCC) $(NCCFLAGS) CudaTest.cu

clean:
	rm -rf *.o $(EXECNAME) $(EXECNAME)_debug

