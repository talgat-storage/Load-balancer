cd ./src/worker

make

mv worker ../../bin

cd ../..

cd ./src/handler

make

mv handler ../../bin

cd ../..

cd ./src/load_balancer

make

mv load_balancer ../../bin

cd ../..

cd ./src/client

make

mv client ../../bin

cd ../..

