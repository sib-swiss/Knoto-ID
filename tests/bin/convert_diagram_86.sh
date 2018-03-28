#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.896127,-0.25119,0.365869 --closure-method=direct --output=convert_diagram_86.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_86.txt "${sourcepath}"/output/convert_diagram_86.txt || exit $?
