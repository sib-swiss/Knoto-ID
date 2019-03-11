#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.431699,0.117367,0.894349 --closure-method=open --output=convert_diagram_78.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_78.txt "${sourcepath}"/output/convert_diagram_78.txt || exit $?
