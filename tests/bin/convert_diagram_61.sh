#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.287089,0.51262,-0.809198 --closure-method=open --output=convert_diagram_61.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_61.txt "${sourcepath}"/output/convert_diagram_61.txt || exit $?
