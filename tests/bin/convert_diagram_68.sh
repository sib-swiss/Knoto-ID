#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.0946507,-0.384778,0.918143 --closure-method=direct --output=convert_diagram_68.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_68.txt "${sourcepath}"/output/convert_diagram_68.txt || exit $?
