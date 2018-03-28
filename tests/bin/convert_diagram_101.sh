#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.536607,-0.800015,0.268381 --closure-method=direct --output=convert_diagram_101.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_101.txt "${sourcepath}"/output/convert_diagram_101.txt || exit $?
