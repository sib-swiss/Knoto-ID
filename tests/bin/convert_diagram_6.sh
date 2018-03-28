#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.43204,0.442363,0.785911 --closure-method=direct --output=convert_diagram_6.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_6.txt "${sourcepath}"/output/convert_diagram_6.txt || exit $?
