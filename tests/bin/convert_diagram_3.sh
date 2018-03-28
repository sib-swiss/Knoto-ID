#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.237356,-0.012821,0.971338 --closure-method=direct --output=convert_diagram_3.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_3.txt "${sourcepath}"/output/convert_diagram_3.txt || exit $?
