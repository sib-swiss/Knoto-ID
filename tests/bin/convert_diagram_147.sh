#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.720975,-0.144123,0.677808 --closure-method=direct --output=convert_diagram_147.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_147.txt "${sourcepath}"/output/convert_diagram_147.txt || exit $?
