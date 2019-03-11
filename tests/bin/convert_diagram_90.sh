#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.694553,-0.545793,0.468728 --closure-method=open --output=convert_diagram_90.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_90.txt "${sourcepath}"/output/convert_diagram_90.txt || exit $?
