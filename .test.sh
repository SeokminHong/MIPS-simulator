TESTDIR="./examples/p2"
OUTPUTDIR="./output/p2"
PROGRAM="simulator"

echo "Testing $1..."

./make-test ${TESTDIR}/$1-setting.txt
COUNT=$?

if [ ${COUNT} -eq 255 ];  then
    exit -1
fi

if [ -f ${TESTDIR}/$1-reg.txt ]; then
    REG=${TESTDIR}/$1-reg.txt
fi

# Run mode 0
echo "./${PROGRAM} ${TESTDIR}/$1-code.txt ${COUNT} 0 ${REG} > ${OUTPUTDIR}/$1-out0.txt"
#./${PROGRAM} ${TESTDIR}/$1-code.txt ${COUNT} 0 ${REG} > ${OUTPUTDIR}/$1-out0.txt
echo diff ${TESTDIR}/$1-out0.txt ${OUTPUTDIR}/$1-out0.txt -B -u

# Run mode 1
echo "./${PROGRAM} ${TESTDIR}/$1-code.txt ${COUNT} 1 ${REG} > ${OUTPUTDIR}/$1-out1.txt"
#./${PROGRAM} ${TESTDIR}/$1-code.txt ${COUNT} 1 ${REG} > ${OUTPUTDIR}/$1-out1.txt
echo diff ${TESTDIR}/$1-out1.txt ${OUTPUTDIR}/$1-out1.txt -B -u