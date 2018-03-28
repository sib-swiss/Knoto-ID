#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.440459,-0.550952,0.708835 --closure-method=direct --output=convert_diagram_19.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_19.txt "${sourcepath}"/output/convert_diagram_19.txt || exit $?
