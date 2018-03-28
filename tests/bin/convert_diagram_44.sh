#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.0754123,0.828349,-0.555114 --closure-method=open --output=convert_diagram_44.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_44.txt "${sourcepath}"/output/convert_diagram_44.txt || exit $?
