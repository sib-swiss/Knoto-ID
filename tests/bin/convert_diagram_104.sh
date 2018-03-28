#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.92716,-0.106332,-0.359261 --closure-method=open --output=convert_diagram_104.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_104.txt "${sourcepath}"/output/convert_diagram_104.txt || exit $?
