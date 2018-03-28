#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.804631,0.21189,-0.554682 --closure-method=direct --output=convert_diagram_98.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_98.txt "${sourcepath}"/output/convert_diagram_98.txt || exit $?
