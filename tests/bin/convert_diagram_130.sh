#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.378279,-0.641999,0.666891 --closure-method=direct --output=convert_diagram_130.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_130.txt "${sourcepath}"/output/convert_diagram_130.txt || exit $?
