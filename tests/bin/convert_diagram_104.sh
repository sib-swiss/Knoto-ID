#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.91297,0.0932733,0.397223 --closure-method=open --output=convert_diagram_104.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_104.txt "${sourcepath}"/output/convert_diagram_104.txt || exit $?
