#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.149606,-0.529028,-0.835313 --closure-method=open --output=convert_diagram_25.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_25.txt "${sourcepath}"/output/convert_diagram_25.txt || exit $?
