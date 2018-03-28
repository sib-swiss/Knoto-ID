#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.616542,-0.205821,0.759944 --closure-method=direct --output=convert_diagram_134.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_134.txt "${sourcepath}"/output/convert_diagram_134.txt || exit $?
