#! /bin/bash

# Create a tarball for a list of JEC text files

CORRECTIONS="L1RC L1FastJet L2Relative L3Absolute L2L3Residual Uncertainty"

P=$1

if [ ! -d "$P" ]; then
    echo "$P does not exists"
    exit 1
fi

ERA=`basename $P`

echo "Creating tarball for ${ERA}..."

pushd $P &> /dev/null

TARBALL=${ERA}.tar.gz

CMD=""
for C in $CORRECTIONS; do
    CMD=${ERA}_${C}*" $CMD"
done

tar -hzcf ${TARBALL} ${CMD}

popd &> /dev/null

echo "Done. File saved as ${P}/${TARBALL}"
