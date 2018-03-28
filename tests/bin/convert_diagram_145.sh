#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.532094,0.471793,-0.703056 --closure-method=direct --output=convert_diagram_145.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_145.txt "${sourcepath}"/output/convert_diagram_145.txt || exit $?
