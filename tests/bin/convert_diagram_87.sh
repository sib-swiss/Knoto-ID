#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.201942,0.906465,-0.370864 --closure-method=direct --output=convert_diagram_87.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_87.txt "${sourcepath}"/output/convert_diagram_87.txt || exit $?
