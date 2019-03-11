#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.595092,0.497739,0.630969 --closure-method=direct --output=convert_diagram_54.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_54.txt "${sourcepath}"/output/convert_diagram_54.txt || exit $?
