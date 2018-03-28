#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.401893,-0.157443,0.90205 --closure-method=open --output=convert_diagram_159.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_159.txt "${sourcepath}"/output/convert_diagram_159.txt || exit $?
