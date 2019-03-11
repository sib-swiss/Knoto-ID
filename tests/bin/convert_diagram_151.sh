#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.78792,-0.487979,-0.375577 --closure-method=direct --output=convert_diagram_151.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_151.txt "${sourcepath}"/output/convert_diagram_151.txt || exit $?
