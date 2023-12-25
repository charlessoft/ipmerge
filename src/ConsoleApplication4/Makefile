run:
	g++ -finput-charset=UTF-8 \
	-DGIT_COMMIT_ID=\"$(git rev-parse HEAD)\" -DBUILD_TIME=\"$(date)\" \
	-o ipmerge \
	-std=c++11 main.cpp logger.cc

clean:
	rm -fr ipmerge