#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.30121,0.576058,-0.759888 --closure-method=direct --output=convert_diagram_34.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_34.txt "${sourcepath}"/output/convert_diagram_34.txt || exit $?
