#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.779262,0.440407,-0.445861 --closure-method=direct --output=convert_diagram_36.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_36.txt "${sourcepath}"/output/convert_diagram_36.txt || exit $?
