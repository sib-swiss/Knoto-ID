#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.782524,0.162093,0.601151 --closure-method=direct --output=convert_diagram_35.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_35.txt "${sourcepath}"/output/convert_diagram_35.txt || exit $?
