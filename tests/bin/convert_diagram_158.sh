#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.268362,-0.00236355,0.963315 --closure-method=open --output=convert_diagram_158.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_158.txt "${sourcepath}"/output/convert_diagram_158.txt || exit $?
