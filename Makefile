BUILD_DIR ?= build

.PHONY: all test install clean sync

all:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR) --parallel

test: all
	ctest --test-dir $(BUILD_DIR) --output-on-failure

install: all
	cmake --install $(BUILD_DIR)

clean:
	cmake -E remove_directory $(BUILD_DIR)

sync:
	./scripts/sync-upstream.sh
