#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.361343,-0.83774,-0.409417 --closure-method=open --output=convert_diagram_143.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_143.txt "${sourcepath}"/output/convert_diagram_143.txt || exit $?
