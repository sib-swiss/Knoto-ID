#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.769377,0.596516,-0.228533 --closure-method=direct --output=convert_diagram_150.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_150.txt "${sourcepath}"/output/convert_diagram_150.txt || exit $?
