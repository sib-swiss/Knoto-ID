#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.673891,0.715768,-0.183158 --closure-method=direct --output=convert_diagram_100.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_100.txt "${sourcepath}"/output/convert_diagram_100.txt || exit $?
