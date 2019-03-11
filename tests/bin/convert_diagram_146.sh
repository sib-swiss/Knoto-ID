#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.413069,-0.518142,0.748934 --closure-method=direct --output=convert_diagram_146.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_146.txt "${sourcepath}"/output/convert_diagram_146.txt || exit $?
