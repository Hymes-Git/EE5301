mkdir -p results

PRG_NAME=./main
TEST_LIB_PATH="./test/NLDM_lib_max2Inp"
TEST_CKT_PATH="./test/cleaned_iscas89_99_circuits"
TEST_ARGS_PATH="./test/"

echo "==========================================="
echo "Testing c17"
echo ""
time $PRG_NAME $TEST_LIB_PATH "${TEST_CKT_PATH}/c17.isc" $(< ${TEST_ARGS_PATH}/c17_args.txt) | tee ./results/c17_out.txt

echo "==========================================="
echo "Testing c1908"
echo ""
time $PRG_NAME $TEST_LIB_PATH "${TEST_CKT_PATH}/c1908_.isc" $(< ${TEST_ARGS_PATH}/c1908_args.txt) | tee ./results/c1908_out.txt

echo "==========================================="
echo "Testing c2670"
echo ""
time $PRG_NAME $TEST_LIB_PATH "${TEST_CKT_PATH}/c2670.isc" $(< ${TEST_ARGS_PATH}/c2670_args.txt) | tee ./results/c2670_out.txt

echo "==========================================="
echo "Testing c3540"
echo ""
time $PRG_NAME $TEST_LIB_PATH "${TEST_CKT_PATH}/c3540.isc" $(< ${TEST_ARGS_PATH}/c3540_args.txt) | tee ./results/c3540_out.txt

echo "==========================================="
echo "Testing c5315"
echo ""
time $PRG_NAME $TEST_LIB_PATH "${TEST_CKT_PATH}/c5315.isc" $(< ${TEST_ARGS_PATH}/c5315_args.txt) | tee ./results/c5315_out.txt

echo "==========================================="
echo "Testing c7552"
echo ""
time $PRG_NAME $TEST_LIB_PATH "${TEST_CKT_PATH}/c7552.isc" $(< ${TEST_ARGS_PATH}/c7552_args.txt) | tee ./results/c7552_out.txt

echo "==========================================="
echo "Testing b15"
echo ""
time $PRG_NAME $TEST_LIB_PATH "${TEST_CKT_PATH}/b15_1.isc" $(< ${TEST_ARGS_PATH}/b15_1_args.txt) | tee ./results/b15_1_out.txt

echo "==========================================="
echo "Testing b18"
echo ""
time $PRG_NAME $TEST_LIB_PATH "${TEST_CKT_PATH}/b18_1.isc" $(< ${TEST_ARGS_PATH}/b18_1_args.txt) | tee ./results/b18_1_out.txt

echo "==========================================="
echo "Testing b19"
echo ""
time $PRG_NAME $TEST_LIB_PATH "${TEST_CKT_PATH}/b19_1.isc" $(< ${TEST_ARGS_PATH}/b19_1_args.txt) | tee ./results/b19_1_out.txt

echo "==========================================="
echo "Testing b22"
echo ""
time $PRG_NAME $TEST_LIB_PATH "${TEST_CKT_PATH}/b22.isc" $(< ${TEST_ARGS_PATH}/b22_args.txt) | tee ./results/b22_out.txt
