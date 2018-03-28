#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.995311,0.0753588,-0.0606449 --closure-method=open --output=convert_diagram_125.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_125.txt "${sourcepath}"/output/convert_diagram_125.txt || exit $?
