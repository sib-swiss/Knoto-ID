#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.662163,0.169991,-0.729824 --closure-method=direct --output=convert_diagram_118.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_118.txt "${sourcepath}"/output/convert_diagram_118.txt || exit $?
