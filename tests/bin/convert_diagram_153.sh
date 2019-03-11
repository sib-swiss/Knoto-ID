#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.355385,0.609658,0.708533 --closure-method=open --output=convert_diagram_153.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_153.txt "${sourcepath}"/output/convert_diagram_153.txt || exit $?
