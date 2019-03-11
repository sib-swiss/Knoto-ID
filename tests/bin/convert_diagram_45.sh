#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.790317,0.117451,-0.601336 --closure-method=open --output=convert_diagram_45.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_45.txt "${sourcepath}"/output/convert_diagram_45.txt || exit $?
