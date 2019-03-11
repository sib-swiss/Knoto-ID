#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.823073,-0.328428,-0.463343 --closure-method=direct --output=convert_diagram_17.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_17.txt "${sourcepath}"/output/convert_diagram_17.txt || exit $?
