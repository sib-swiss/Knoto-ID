#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=gauss --planar --output-format=pd --output=convert_diagram_217.txt "${sourcepath}"/input/input_diagram_gauss_open_plane_2.txt || exit $? 
diff convert_diagram_217.txt "${sourcepath}"/output/convert_diagram_217.txt || exit $?
