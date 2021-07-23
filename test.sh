#!/bin/bash

TEST_COUNT=`echo tests/*.x2017 | wc -w`
TESTS_DONE=0
PASSED=0

echo "###########################"
echo "##### Assembler Tests #####"
echo "###########################"
for FILE in tests/*.x2017
do
    TEST_NAME=`basename -s .x2017 $FILE`
    ((TESTS_DONE++))
    echo '-------------------------------------'
    echo "##### Test ($TESTS_DONE/$TEST_COUNT): $TEST_NAME #####"
    ./objdump_x2017 tests/$TEST_NAME.x2017 2>&1 | diff - tests/$TEST_NAME.asm
 
    if [[ $? == 0 ]]
    then
        echo $'\nTest passed!'
        ((PASSED++))
    else
        echo $'\nTest Failed.'
    fi
done

echo '-------------------------------------'

if [[ $PASSED == $TESTS_DONE ]]
then
    echo "⭐ All $TESTS_DONE Assembly tests passed successfully! ⭐"
else
    echo "☹ ($PASSED/$TESTS_DONE) Assembly tests passed successfully. ☹"
fi

echo ""

TEST_COUNT=`echo tests/*.x2017 | wc -w`
TESTS_DONE=0
PASSED=0

echo "###########################"
echo "######### VM Tests ########"
echo "###########################"
for FILE in tests/*.x2017
do
    TEST_NAME=`basename -s .x2017 $FILE`
    ((TESTS_DONE++))
    echo '-------------------------------------'
    echo "##### Test ($TESTS_DONE/$TEST_COUNT): $TEST_NAME #####"
    ./vm_x2017 tests/$TEST_NAME.x2017 2>&1 | diff - tests/$TEST_NAME.out

    if [[ $? == 0 ]]
    then
        echo $'\nTest passed!'
        ((PASSED++))
    else
        echo $'\nTest Failed.'
    fi
done

echo '-------------------------------------'

if [[ $PASSED == $TESTS_DONE ]]
then
    echo "⭐ All $TESTS_DONE VM tests passed successfully! ⭐"
else
    echo "☹ ($PASSED/$TESTS_DONE) VM tests passed successfully. ☹"
fi

echo ""

TEST_COUNT=`echo tests/errors/*.args | wc -w`
TESTS_DONE=0
PASSED=0

echo "###########################"
echo "####### Error Tests #######"
echo "###########################"
for FILE in tests/errors/*.args
do
    TEST_NAME=`basename -s .args $FILE`
    ((TESTS_DONE++))
    echo '-------------------------------------'
    echo "##### Test ($TESTS_DONE/$TEST_COUNT): $TEST_NAME #####"
    
    if [[ -f $TEST_NAME.asm ]]
    then    
        xargs -a tests/errors/$TEST_NAME.args ./objdump_x2017 | \
                                            diff - tests/errors/$TEST_NAME.asm
        if [[ $? !=  0 ]]
        then
            echo $'\nTest Failed (Assembly ✖)'
            continue
        fi
    fi

    xargs -a tests/errors/$TEST_NAME.args ./vm_x2017 >/dev/null 2>&1

    if [[ $? == 123 ]]
    then
        xargs -a tests/errors/$TEST_NAME.args ./vm_x2017 2>&1 | \
                                            diff - tests/errors/$TEST_NAME.out
        if [[ $? == 0 ]]
        then
            echo $'\nTest passed (Return Code ✓ Output ✓)!'
            ((PASSED++))
        else
            echo $'\nTest Failed (Return Code ✓ Output ✖)'
        fi
    else
        echo $'\nTest Failed (Return Code ✖ Output ✖)'
    fi
done

echo '-------------------------------------'

if [[ $PASSED == $TESTS_DONE ]]
then
    echo "⭐ All $TESTS_DONE Error tests passed successfully! ⭐"
else
    echo "☹ ($PASSED/$TESTS_DONE) Error tests passed successfully. ☹"
fi
