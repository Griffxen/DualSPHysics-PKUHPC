#!/bin/bash

chmod +x bin/linux/*_linux64
chmod +x bin/linux/AmdGpu/*_linux64
chmod +x bin/linux/DSNNewtonian/*_linux64
chmod +x bin/linux/VResolution/*_linux64
chmod +x bin/linux/DSGcc7/*_linux64

# chmod +x examples/boundcorrection/*/*.sh
chmod +x examples/chrono/*/*.sh
chmod +x examples/flexstruc/*/*.sh
chmod +x examples/inletmesh/*/*.sh
chmod +x examples/inletoutlet/*/*.sh
chmod +x examples/main/*/*.sh
chmod +x examples/mdbc/*/*.sh
chmod +x examples/moordynplus/*/*.sh
chmod +x examples/motion/*.sh
chmod +x examples/others/*/*.sh
chmod +x examples/mphase_liquidgas/*/*.sh
chmod +x examples/mphase_nnewtonian/*/*.sh
chmod +x examples/vresolution/*/*.sh
chmod +x examples/shiftingadv/*/*.sh
chmod +x examples/wavecoupling/*/*.sh
chmod +x examples/mdbc/10_NoSlip/*/*.sh

# dos2unix examples/boundcorrection/*/*.sh
dos2unix examples/chrono/*/*.sh
dos2unix examples/flexstruc/*/*.sh
dos2unix examples/inletmesh/*/*.sh
dos2unix examples/inletoutlet/*/*.sh
dos2unix examples/main/*/*.sh
dos2unix examples/mdbc/*/*.sh
dos2unix examples/moordynplus/*/*.sh
dos2unix examples/motion/*.sh
dos2unix examples/others/*/*.sh
dos2unix examples/mphase_liquidgas/*/*.sh
dos2unix examples/mphase_nnewtonian/*/*.sh
dos2unix examples/vresolution/*/*.sh
dos2unix examples/wavecoupling/*/*.sh
dos2unix examples/shiftingadv/*/*.sh
dos2unix examples/mdbc/10_NoSlip/*/*.sh

chmod +x src_extra/ToVTK/bin/*_linux64
chmod +x src_extra/ToVTK/bin/*.sh
dos2unix src_extra/ToVTK/bin/*.sh


