#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.22269,-0.215666,-0.950735 --closure-method=direct --output=convert_diagram_19.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_19.txt "${sourcepath}"/output/convert_diagram_19.txt || exit $?
