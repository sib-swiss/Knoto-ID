#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.0336158,-0.177524,-0.983542 --closure-method=direct --output=convert_diagram_144.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_144.txt "${sourcepath}"/output/convert_diagram_144.txt || exit $?
