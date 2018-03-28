#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.750657,-0.121495,0.649425 --closure-method=direct --output=convert_diagram_21.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_21.txt "${sourcepath}"/output/convert_diagram_21.txt || exit $?
