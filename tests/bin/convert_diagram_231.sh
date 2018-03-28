#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=gauss --closure-method=direct --output-format=gauss --output=convert_diagram_231.txt "${sourcepath}"/input/input_diagram_gauss_closed_2.txt || exit $? 
diff convert_diagram_231.txt "${sourcepath}"/output/convert_diagram_231.txt || exit $?
