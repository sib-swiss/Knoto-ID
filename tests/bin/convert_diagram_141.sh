#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.516157,0.35109,-0.781228 --closure-method=open --output=convert_diagram_141.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_141.txt "${sourcepath}"/output/convert_diagram_141.txt || exit $?
