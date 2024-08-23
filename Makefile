build:
	gcc voxel.c -Iinclude/ -lraylib -lm -lpthread -ldl -lX11 -o voxel

run: 
	./voxel

clean:
	rm voxel