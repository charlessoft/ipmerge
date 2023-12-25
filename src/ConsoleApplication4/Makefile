GIT_COMMIT_ID := $(shell git rev-parse HEAD)
 BUILD_TIME := $(shell date "+%Y-%m-%d\ %H\:%M\:%S")
run:
	g++ -finput-charset=UTF-8 \
	-DGIT_COMMIT_ID=\"$(GIT_COMMIT_ID)\" -DBUILD_TIME=\"$(BUILD_TIME)\" \
	-o ipmerge -std=c++11 main.cpp logger.cc

clean:
	rm -fr ipmerge