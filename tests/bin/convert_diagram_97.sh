#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.259448,-0.864756,0.429982 --closure-method=direct --output=convert_diagram_97.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_97.txt "${sourcepath}"/output/convert_diagram_97.txt || exit $?
