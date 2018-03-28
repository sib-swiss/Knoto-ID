#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=gauss --closure-method=direct --output-format=pd --output=convert_diagram_225.txt "${sourcepath}"/input/input_diagram_gauss_closed_1.txt || exit $? 
diff convert_diagram_225.txt "${sourcepath}"/output/convert_diagram_225.txt || exit $?
