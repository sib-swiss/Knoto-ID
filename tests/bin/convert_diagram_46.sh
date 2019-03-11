#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.025873,-0.912006,0.409359 --closure-method=open --output=convert_diagram_46.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_46.txt "${sourcepath}"/output/convert_diagram_46.txt || exit $?
