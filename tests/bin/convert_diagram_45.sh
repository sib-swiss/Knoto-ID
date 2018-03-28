#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.120206,-0.445684,0.887083 --closure-method=open --output=convert_diagram_45.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_45.txt "${sourcepath}"/output/convert_diagram_45.txt || exit $?
