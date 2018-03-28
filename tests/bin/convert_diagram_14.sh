#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.461361,0.867457,0.186183 --closure-method=open --output=convert_diagram_14.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_14.txt "${sourcepath}"/output/convert_diagram_14.txt || exit $?
