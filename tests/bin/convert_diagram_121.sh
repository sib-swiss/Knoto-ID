#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.563829,-0.207893,0.799298 --closure-method=open --output=convert_diagram_121.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_121.txt "${sourcepath}"/output/convert_diagram_121.txt || exit $?
