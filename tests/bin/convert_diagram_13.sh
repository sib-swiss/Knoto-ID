#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.431019,-0.738206,0.518917 --closure-method=open --output=convert_diagram_13.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_13.txt "${sourcepath}"/output/convert_diagram_13.txt || exit $?
