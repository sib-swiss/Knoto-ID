#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.0946382,-0.782953,-0.61484 --closure-method=open --output=convert_diagram_155.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_155.txt "${sourcepath}"/output/convert_diagram_155.txt || exit $?
