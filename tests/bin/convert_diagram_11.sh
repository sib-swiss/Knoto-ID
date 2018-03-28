#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.870262,0.471207,0.143555 --closure-method=open --output=convert_diagram_11.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_11.txt "${sourcepath}"/output/convert_diagram_11.txt || exit $?
