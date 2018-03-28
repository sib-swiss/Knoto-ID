#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.882903,0.33102,0.333028 --closure-method=direct --output=convert_diagram_17.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_17.txt "${sourcepath}"/output/convert_diagram_17.txt || exit $?
