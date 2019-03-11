#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.26516,-0.165894,0.949826 --closure-method=open --output=convert_diagram_110.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_110.txt "${sourcepath}"/output/convert_diagram_110.txt || exit $?
