#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.755579,-0.359769,0.547418 --closure-method=direct --output=convert_diagram_83.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_83.txt "${sourcepath}"/output/convert_diagram_83.txt || exit $?
