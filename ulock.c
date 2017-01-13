#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define PASS_MAX 64
#define READ_MAX 1024

void write_locked_output(int output_file, char* input_buff, int input_size, char* pass_buff, int pass_size) {
	char write_buff[input_size];
	int i;
	for(i = 0;i < input_size;i++) {
		write_buff[i] = input_buff[i] ^ (pass_buff[i % pass_size] + pass_buff[i % pass_size] / 2);
	}

	write(output_file, write_buff, input_size);
}

int main(int argc, char* argv[]) {
	char pass_buff[PASS_MAX];
	char input_buff[READ_MAX];

	int pass_size = read(0, pass_buff, PASS_MAX);

	int input_file = open(argv[1], O_RDONLY | O_BINARY);
	int output_file = open(argv[1], O_CREAT | O_WRONLY | O_BINARY, 0777);	

	int input_size;
	while((input_size = read(input_file, input_buff, READ_MAX)) > 0) {
		write_locked_output(output_file, input_buff, input_size, pass_buff, pass_size);
	}

	close(input_file);
	close(output_file);

	printf("done");

	return 0;
}
