#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.21214,-0.381661,0.899628 --closure-method=direct --output=convert_diagram_33.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_33.txt "${sourcepath}"/output/convert_diagram_33.txt || exit $?
