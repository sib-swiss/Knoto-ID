#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.673617,-0.705745,-0.219462 --closure-method=direct --output=convert_diagram_6.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_6.txt "${sourcepath}"/output/convert_diagram_6.txt || exit $?
