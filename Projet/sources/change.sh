#! /bin/bash


find . -name '*pp' -exec sed -i \
     -e 's/é/e/g' \
     -e 's/è/e/g' \
     -e 's/à/a/g' \
     -e 's/ù/u/g' \
     -e 's/â/a/g' \
     -e 's/β/beta/g' \
     -e 's/α/alpha/g' \
     -e 's/θ/theta/g' \
     -e 's/μ/mu/g' \
     '{}' \;
