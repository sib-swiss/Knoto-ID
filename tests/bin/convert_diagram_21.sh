#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.643691,0.22286,0.732117 --closure-method=direct --output=convert_diagram_21.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_21.txt "${sourcepath}"/output/convert_diagram_21.txt || exit $?
