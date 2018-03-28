#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.995527,-0.00279816,0.0944352 --closure-method=direct --output=convert_diagram_22.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_22.txt "${sourcepath}"/output/convert_diagram_22.txt || exit $?
