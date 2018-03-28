#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.75895,0.642247,0.107305 --closure-method=open --output=convert_diagram_110.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_110.txt "${sourcepath}"/output/convert_diagram_110.txt || exit $?
