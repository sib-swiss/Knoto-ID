#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.763326,0.190352,-0.617333 --closure-method=direct --output=convert_diagram_18.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_18.txt "${sourcepath}"/output/convert_diagram_18.txt || exit $?
