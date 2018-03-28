#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.974742,-0.0891993,0.204746 --closure-method=direct --output=convert_diagram_103.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_103.txt "${sourcepath}"/output/convert_diagram_103.txt || exit $?
