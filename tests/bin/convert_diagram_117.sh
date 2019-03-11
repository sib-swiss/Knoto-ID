#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.527125,-0.84052,-0.125164 --closure-method=direct --output=convert_diagram_117.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_117.txt "${sourcepath}"/output/convert_diagram_117.txt || exit $?
