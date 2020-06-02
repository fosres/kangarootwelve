#include <stdio.h>
#include <string.h>

unsigned char *** keccak_p(unsigned char *b,unsigned long long int n_r);

unsigned char * F(unsigned char * b, unsigned long long int n_r)	{
	
	unsigned long long int offset = 0;

	unsigned char state[201]; 

	unsigned char input_slot[201];
	
	memset(state,0x0,sizeof(unsigned char)*256);

	memset(input_slot,0x0,sizeof(unsigned char)*256);
	while ( offset < input_len - 168 )	{

		strncat(input_slot,input,sizeof(unsigned char)*168);

		memset(&input_slot[offset+168],0x0,sizeof(unsigned char)*32);

		unsigned i = 0;

		while ( i < 200 )
		{
			state[i] ^= input_slot[i];

			i++;
		}

		keccak_p(state,12);	
		
		offset += 168;

		memset(input_slot,0x0,201);		
	}

	// Absorb last block and treatment of padding

	unsigned long long last_block_length = input_len - offset;

	strncat(input_slot,&input[offset],last_block_length);

	memset(&input_slot[last_bock_length],0x0,200-last_block_length);

	unsigned i = 0;

	while ( i < 167 )	{
		
		state[i] ^= 0x00;

		i++;

	}

	state[i++] = 0x80;

	while ( i < 200 )	{
		
		state[i] ^= 0x00;

		i++;
	}
	
	keccak_p(state);

	// Squeeze 

	unsigned char * output = (unsigned char*)calloc(output_byte_len,sizeof(unsigned char));

	unsigned long long oblc = output_byte_len;

	memset(output,0x0,output_byte_len*sizeof(unsigned char));

	while ( oblc > 168 )	{
		
		strncat(output,&state[0],168*sizeof(unsigned char));

		keccak_p(state);
		
		oblc -= 168;
	}
		


strncat(output,&state[0],output_byte_len*sizeof(unsigned char));

return output;

}
