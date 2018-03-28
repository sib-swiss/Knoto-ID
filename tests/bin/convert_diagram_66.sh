#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.616858,-0.0409677,-0.786008 --closure-method=direct --output=convert_diagram_66.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_66.txt "${sourcepath}"/output/convert_diagram_66.txt || exit $?
