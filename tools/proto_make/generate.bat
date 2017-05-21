protoc -I=./protocol/ --cpp_out=../../lib/comm/proto ./protocol/client_server.proto

protoc -I=./protocol/ --cpp_out=../../lib/comm/proto ./protocol/server_server.proto

protoc -I=./protocol/ --cpp_out=../../lib/comm/proto ./protocol/base_type.proto

pause