#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.888155,0.452492,0.0801971 --closure-method=direct --output=convert_diagram_4.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_4.txt "${sourcepath}"/output/convert_diagram_4.txt || exit $?
