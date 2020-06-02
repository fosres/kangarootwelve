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

void theta(unsigned char A[][][],unsigned char C[][],unsigned char D[][]);

void rho(unsigned char A[][][]);

void pi(unsigned char A[][][]);

void chi(unsigned char A[][][]);

void keccak_p(unsigned char *S,unsigned long long int n_r)	{

	unsigned x = 0, y = 0, z = 0;

	while ( y < 5 )			
	{

		while ( x < 5 )	
		{
			
			while ( z < 64 )
			{
				A[x][y][z] = (S[(64*(5*y+x)+z)/8] >> (7-(64*(5*y+x)+z)%8)) & 0b1;

				z++;

			}

			if ( z == 64 )
			{

				x++;

				z = 0;	
			}
		}

		if ( x == 5 )
		{
			y++;

			x = 0;
		}
	}

	unsigned char C[5][64];

	unsigned char D[5][64];

	unsigned i_r = 0;

	while ( i_r < n_r)
	{
		
		theta(A,C,D);

		rho(A);

		pi(A);

		chi(A);

		iota(A,i_r);

		i_r++;

	}

	// Redo large x,y,z while loop, switching left and right sides across "=" operation

}


void theta(unsigned char A[][][],unsigned char C[][],unsigned char D[][])
{
	
	unsigned x = 0, z = 0, y = 0;

	while ( x < 5 )
	{
		while ( z < 64 )
		{
			C[x][z] = A[x,0,z] ^ A[x,1,z] ^ A[x,2,z] ^ A[x,3,z] ^ A[x,4,z];

			D[x][z] = C[(x-1) % 5,z] ^ C[(x+1)%5,(z-1)%64];

			while ( y < 5 )
			{
				
				A[x][y][z] = A[x][y][z] ^ D[x][z];

				y++;

			}

			y = 0;

			z++;
		}

		z = 0; x++;
	}

}


void rho(unsigned char A[][][])
{
	unsigned x = 1, y = 0, z = 0, t = 0;

	while ( z < 64 )
	{
		x = 1; y = 0; t = 0;
	
		while ( t < 23 )
		{
			A[x][y][z] = A[x][y][(z-(t+1)(t+2)/2)%64];

			x = y; y = (2*x+3*y)%5;

			t++;
		}

		z++;
	}

}

void pi(unsigned char A[][][])
{
	unsigned x = 0, y = 0, z = 0;

	while ( y < 5 )
	{
		while ( x < 5 )
		{
			while ( z < 64 )
			{
				A[x][y][z] = A[(x+3*y)%5,x,z];
				z++;
			}

			z = 0; x++;
		}
	}

}

void chi(unsigned char A[][][])
{
	unsigned x = 0, y = 0, z = 0;

	while ( y < 5 )
	{
		while ( x < 5 )
		{
			
			while ( z < 64 )
			{
				A[x][y][z] = A[x][y][z] ^ (
	(
		A[(x+1)%5][y][z] ^ 1)

		*

		A[(x+2)%5][y][z]

	);

			}
			
			z = 0; x++;
		}

		x = 0; y++;

	}

}

unsigned char rc(unsigned long long t)
{
	unsigned char RC[9] = {0b1,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0};

	if ( t % 255 == 0 )
	{
		return 0b1;
	}
	
	unsigned i = 0, j = 0;

	while ( i < (t % 255) )
	{
		j = 8;

		while ( j >= 1)
		{
			
			RC[j] = RC[j-1];

			j--;

		}

		RC[0] = 0b0;

		RC[4] = RC[0] ^ RC[8];

		RC[5] = RC[4] ^ RC[8];

		RC[6] = RC[6] ^ RC[8];

		RC[8] = 0b0;
	}

	return RC[0];
}

void iota(unsigned char A[][][],unsigned i_r)
{
	unsigned char R[64];

	memset(RC,0x0,64*sizeof(unsigned char));

	unsigned j = 0, z = 0;

	while ( j < 6 )
	{
		RC[(unsigned)pow(2,j)-1] = rc(j+7*i-4);

		j++;
	}

	while ( z < 64 )
	{
		A[0,0,z] = A[0,0,z] ^ RC[z];

		z++;
	}

}

unsigned char * encode = (unsigned char*)calloc(257,sizeof(unsigned char));

unsigned char e_p = encode;

void length_encode(unsigned long long int x)
{

	if ( x == 0 )
	{
		*e_p = (e_p - enocde);

		e_p--;

		return;
	}

	e_p++;

	length_encode(x/256);

	*e_p-- = x % 256;
} 
