mkdir -p results

PRG_NAME=./placer
TEST_LIB_PATH="./test/NLDM_lib_max2Inp"
TEST_CKT_PATH="tests"

run_test() {
    local circuit_name=$1
    local circuit_file=$2
    local output_file="./results/${circuit_name}_out.txt"

    echo "==========================================="
    echo "Testing ${circuit_name}"
    echo ""

    time $PRG_NAME  "$circuit_file" | tee "$output_file"

}

run_test "toy01" "${TEST_CKT_PATH}/toy01/toy01"
run_test "toy02" "${TEST_CKT_PATH}/toy02/toy02"
run_test "ibm01" "${TEST_CKT_PATH}/ibm01/ibm01"
run_test "ibm03" "${TEST_CKT_PATH}/ibm03/ibm03"
