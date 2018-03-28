#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.858568,0.443741,0.256818 --closure-method=open --output=convert_diagram_30.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_30.txt "${sourcepath}"/output/convert_diagram_30.txt || exit $?
