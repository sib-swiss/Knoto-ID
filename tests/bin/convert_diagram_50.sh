#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.496274,0.619972,0.607739 --closure-method=direct --output=convert_diagram_50.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_50.txt "${sourcepath}"/output/convert_diagram_50.txt || exit $?
