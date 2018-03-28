#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.249139,0.514686,-0.820383 --closure-method=direct --output=convert_diagram_114.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_114.txt "${sourcepath}"/output/convert_diagram_114.txt || exit $?
