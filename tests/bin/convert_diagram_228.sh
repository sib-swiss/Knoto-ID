#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --close-diagram --input-format=gauss --closure-method=direct --output-format=pd --output=convert_diagram_228.txt "${sourcepath}"/input/input_diagram_gauss_closed_2.txt || exit $? 
diff convert_diagram_228.txt "${sourcepath}"/output/convert_diagram_228.txt || exit $?
