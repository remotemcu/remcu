PATH2TEST="$1"

echo "starting mock gdb server..."
python ./test/mock_server/pyproxy.py  -s 127.0.0.1:3333 --play -v -f ./test/mock_server/gdb.txt &
RUNNING_PID=$!
echo "PID of server : $RUNNING_PID"
sleep 2
echo "starting test gdb client..."
LD_LIBRARY_PATH="$PATH2TEST/" $PATH2TEST/test_shared localhost 0 3333 0x20000000 1 || exit -1
echo "finish mock server $RUNNING_PID"
kill $RUNNING_PID
echo "======================================================================================================"

sleep 4

echo "starting mock OpenOCD v0.10 TCL server..."
python ./test/mock_server/pyproxy.py  -s 127.0.0.1:6666 --play -v -f ./test/mock_server/openocd_0.10.txt &
RUNNING_PID=$!
echo "PID of server : $RUNNING_PID"
sleep 2
echo "starting test openocd v0.10 client..."
LD_LIBRARY_PATH="$PATH2TEST/" $PATH2TEST/test_shared localhost 6666 0 0x20000000 1 || exit -1
echo "finish mock server $RUNNING_PID"
kill $RUNNING_PID
echo "======================================================================================================"

sleep 4

echo "starting mock OpenOCD v0.12 TCL server..."
python ./test/mock_server/pyproxy.py  -s 127.0.0.1:6667 --play -v -f ./test/mock_server/openocd_0.12.txt &
RUNNING_PID=$!
echo "PID of server : $RUNNING_PID"
sleep 2
echo "starting test openocd v0.12 client..."
LD_LIBRARY_PATH="$PATH2TEST/" $PATH2TEST/test_shared localhost 6667 0 0x20000000 1 || exit -1
echo "finish mock server $RUNNING_PID"
kill $RUNNING_PID
echo "======================================================================================================"
