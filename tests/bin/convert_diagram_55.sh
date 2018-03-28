#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.81122,-0.526214,0.254994 --closure-method=direct --output=convert_diagram_55.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_55.txt "${sourcepath}"/output/convert_diagram_55.txt || exit $?
