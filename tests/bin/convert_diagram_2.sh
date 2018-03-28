#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.377752,0.599628,0.705514 --closure-method=direct --output=convert_diagram_2.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_2.txt "${sourcepath}"/output/convert_diagram_2.txt || exit $?
