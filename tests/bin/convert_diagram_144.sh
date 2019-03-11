#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.954728,-0.197661,0.222318 --closure-method=direct --output=convert_diagram_144.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_144.txt "${sourcepath}"/output/convert_diagram_144.txt || exit $?
