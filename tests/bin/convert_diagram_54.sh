#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.88223,0.470698,-0.0106237 --closure-method=direct --output=convert_diagram_54.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_54.txt "${sourcepath}"/output/convert_diagram_54.txt || exit $?
