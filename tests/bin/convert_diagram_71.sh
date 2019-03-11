#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.390527,-0.652707,-0.649201 --closure-method=direct --output=convert_diagram_71.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_71.txt "${sourcepath}"/output/convert_diagram_71.txt || exit $?
