
.PHONY: all

all:
	@cd ./openssl101 && cp ~/workspace/protocol/benchmark/openssl101/libssl.a . && cp ~/workspace/protocol/benchmark/openssl101/libcrypto.a . &&  cp -r ~/workspace/protocol/benchmark/openssl101/include . && ./build
	@cd ./openssl110 && cp ~/workspace/protocol/benchmark/openssl110/libssl.a . && cp ~/workspace/protocol/benchmark/openssl110/libcrypto.a . && cp -r ~/workspace/protocol/benchmark/openssl110/include . &&  ./build
	@cd ./openssl3 && cp ~/workspace/protocol/benchmark/openssl3/libssl.a . && cp ~/workspace/protocol/benchmark/openssl3/libcrypto.a . &&  cp -r ~/workspace/protocol/benchmark/openssl3/include . &&  ./build
clean:
	rm -rf openssl101/out/*/*
	rm -f openssl101/openssl101
	rm -rf openssl110/out/*/*
	rm -f openssl110/openssl110
	rm -rf openssl3/out/*/*
	rm -f openssl3/openssl3


