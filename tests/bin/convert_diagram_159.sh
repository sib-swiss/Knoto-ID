#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.650551,0.636911,-0.413677 --closure-method=open --output=convert_diagram_159.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_159.txt "${sourcepath}"/output/convert_diagram_159.txt || exit $?
