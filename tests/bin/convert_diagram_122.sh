#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.301814,-0.170111,-0.938067 --closure-method=open --output=convert_diagram_122.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_122.txt "${sourcepath}"/output/convert_diagram_122.txt || exit $?
