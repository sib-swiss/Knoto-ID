#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.0131896,0.635908,-0.771652 --closure-method=direct --output=convert_diagram_132.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_132.txt "${sourcepath}"/output/convert_diagram_132.txt || exit $?
