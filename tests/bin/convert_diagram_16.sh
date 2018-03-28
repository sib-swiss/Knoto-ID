#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.983092,0.152894,-0.100765 --closure-method=direct --output=convert_diagram_16.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_16.txt "${sourcepath}"/output/convert_diagram_16.txt || exit $?
