# DualSPHysics HPC Package

This repository contains a maintained DualSPHysics v5.4 package prepared for
Linux and HPC use. It keeps the upstream source tree, bundled documentation,
prebuilt Linux tools, and site-specific build profiles that are currently used
for GPU deployments.

DualSPHysics is developed by the DualSPHysics project. This repository is a
deployment-oriented fork/package and should be treated as a redistribution with
local build and documentation changes, not as the official upstream project.

## Repository Layout

- `src/source/`: main DualSPHysics v5.4 source code and build files.
- `src_mphase/`: multiphase and non-Newtonian solver variants.
- `src_extra/`: auxiliary compiler-test and helper sources.
- `bin/linux/`: bundled Linux executables and runtime libraries.
- `doc/`: bundled guides, XML templates, and command help output.
- `example_hpc/`: a small HPC-oriented example case and run script.
- `CHANGES.txt`: upstream package change log.

## Quick Start With Bundled Linux Binaries

Clone the repository and make the bundled tools executable if needed:

```bash
git clone <github-repository-url> DualSPHysics
cd DualSPHysics
chmod +x bin/linux/*_linux64 bin/linux/DSGcc7/*_linux64 bin/linux/DSNNewtonian/*_linux64
```

The main solver binaries are:

```bash
bin/linux/DualSPHysics5.4_linux64
bin/linux/DualSPHysics5.4CPU_linux64
bin/linux/GenCase_linux64
```

For a typical case, generate particles with `GenCase` and then run the GPU or
CPU solver:

```bash
bin/linux/GenCase_linux64 <case>_Def <output-dir>/<case> -save:all
bin/linux/DualSPHysics5.4_linux64 -gpu <output-dir>/<case> <output-dir>
```

Set `LD_LIBRARY_PATH` when running binaries that need bundled shared libraries:

```bash
export LD_LIBRARY_PATH="$PWD/bin/linux:${LD_LIBRARY_PATH}"
```

## Building From Source

The default Linux build files are in `src/source/`:

```bash
cd src/source
make clean
make
```

The default `Makefile` selects CUDA 12 settings unless overridden. You can pass
make variables to adjust the build:

```bash
make CUDA=11
make GCC=7
make -f Makefile_cpu
```

CUDA toolkit paths, GPU architectures, and optional components such as Chrono,
WaveGen, and MoorDynPlus are configured near the top of the Makefiles.

## HPC Deployment Profiles

This repository includes two site-specific HPC build profiles that are kept
because they are part of the currently used deployment workflow:

- `src/source/Makefile_hpcwm1`
- `src/source/Makefile_hpcwm2`
- `src/source/wm1setup/runmake.sh`
- `src/source/wm2setup/runmake.sh`

A typical deployment flow is:

```bash
ssh <user>@<hpc-login-node>
mkdir -p ~/software
cd ~/software
git clone <github-repository-url> DualSPHysics
cd DualSPHysics/src/source
```

For the WM1 profile, the existing script loads CUDA 11.6 and GCC 9.3:

```bash
cd ~/software/DualSPHysics/src/source
bash wm1setup/runmake.sh
```

For the WM2 profile, the existing script loads CUDA 12.6 and GCC 12.2:

```bash
cd ~/software/DualSPHysics/src/source
bash wm2setup/runmake.sh
```

These scripts assume the target HPC site provides matching environment modules
and filesystem paths. If your site uses different module names or CUDA
installations, edit the corresponding `runmake.sh` and `Makefile_hpcwm*` files.

When an interactive compute node is required before compiling, request one with
the scheduler command used by your cluster. For Slurm-based GPU clusters, the
workflow is typically similar to:

```bash
srun -p GPU36 --nodes=1 --gres=gpu:1 --cpus-per-task=8 --time=02:00:00 --qos=low --pty /bin/bash
```

## Running the HPC Example

The example in `example_hpc/` expects the bundled binaries under
`~/software/DualSPHysics/bin/linux`:

```bash
cd ~/software/DualSPHysics/example_hpc
bash run.sh
```

If the repository is installed somewhere else, edit `dirbin` in
`example_hpc/run.sh`.

The script runs `GenCase`, executes the GPU solver, and then performs common
post-processing steps with the bundled tools.

## Documentation

Bundled documentation is available in `doc/`, including XML guides, tool help
output, and package notes. The upstream documentation and wiki remain the best
reference for solver theory, case setup, and citation guidance:

- DualSPHysics website: <https://dual.sphysics.org/>
- DualSPHysics GitHub wiki: <https://github.com/DualSPHysics/DualSPHysics/wiki>
- References and citation guidance: <https://dual.sphysics.org/references>

## Licensing

The root `LICENSE` file contains the GNU Lesser General Public License version
2.1. DualSPHysics source files generally state that they are licensed under
LGPL-2.1-or-later.

This repository also includes third-party components, bundled binaries,
documentation, and auxiliary tools that may carry their own copyright notices
and license terms. Component-level notices remain authoritative. See
`NOTICE.md` and the license headers or license files in each component
directory, especially `src/source/Source_DSphMoorDynPlus/`.

Additional modifications and public-facing repository documentation in this
fork are licensed under LGPL-2.1-or-later unless otherwise stated.

## Maintainer Notes

- Keep upstream copyright and license headers intact.
- Do not commit generated simulation output directories.
- Review public documentation before publishing private deployment details,
  credentials, or internal repository URLs.
