/*Name: James Taylor
  Class: COP 3402
  Assigment#1
  Due Date 9/20/13
 */


#include <stdio.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVEL 3

typedef struct Instruction{
	int op;
	int l;
	int m;
	char *str;//used to store inter. instruction for printing.
} Instruction;

int SP, BP, PC, IR, numInst;//global ints
int stack[MAX_STACK_HEIGHT];//stack
Instruction code[MAX_CODE_LENGTH];//array of instructions
int ar_index[MAX_CODE_LENGTH]; //break Array
int ar_i;//Index for break array


//helper methods
void print_code();
void print_stack();
char* decode(int op);

void lit(Instruction ins);
void opr(Instruction ins);
void lod(Instruction ins);
void sto(Instruction ins);
void cal(Instruction ins);
void inc(Instruction ins);
void jmp(Instruction ins);
void jpc(Instruction ins);
void sio(Instruction ins);

void run_code();

int main()
{
	FILE *in = fopen("input.in", "r");
    FILE *out = fopen("output.out", "w");
	//init 
	ar_i=0;//Index for break array
	SP=0;
	BP=1;
	PC=0;
	IR=0;
	numInst=0;

	int i =0, op=0, l=0, m=0;
	for(i=0;i<MAX_STACK_HEIGHT;i++)
	{
		stack[i]=0;
	}
	for(i=0;i<MAX_CODE_LENGTH;i++)
	{
		ar_index[i]=-1;
	}
	i=0;
	//read input

	//fetch
	while(fscanf(in,"%d",&op)!=EOF)
	{

		fscanf(in,"%d",&l);
		fscanf(in,"%d",&m);

		Instruction temp;
		temp.op = op;
		temp.l = l;
		temp.m = m;
		temp.str  = decode(op);
		code[i++] = temp;
		
		numInst++;
		
	}
	print_code();//print fetch

	run_code();//run and print program



	system("PAUSE");//pause on exit, for running in IDE only
	return 0;
}

void run_code()
{
	printf("\t\t\t\t\tPC\tBP\tSP\tStack\nInital Values\t\t\t\t0\t1\t0\n");//output header
	while(1)
	{
		if(PC>=numInst||BP==0)
		{
			break;
		}
		//current instruction is code[PC]

		printf("%d\t%s\t%d\t%d", PC, code[PC].str, code[PC].l, code[PC].m);//Initial values
		printf("\t\t");

		switch(code[PC].op)
		{
			case 1:lit(code[PC]);PC++;break;
			case 2:opr(code[PC]);PC++;break;
			case 3:lod(code[PC]);PC++;break;
			case 4:sto(code[PC]);PC++;break;
			case 5:cal(code[PC]);break;//jumps do not inc pc
			case 6:inc(code[PC]);PC++;break;
			case 7:jmp(code[PC]);break;//jumps do not inc pc
			case 8:jpc(code[PC]);break;//jumps do not inc pc
			case 9:sio(code[PC]);PC++;break;
			default:break;
		}
		printf("%d\t%d\t%d\t",PC,BP,SP);
		print_stack();//print the stack at each level
		//system("PAUSE");



		//print current line
		//PC++;//increment PC

		
	}
}




int base(l, base) // l stand for L in the instruction format
{  
  int b1; //find base L levels down
  b1 = base; 
  while (l > 0)
  {
    b1 = stack[b1 + 1]-1;//-1 implemented for proper use of BP in implementation
    l--;
  }
  return b1;
}
void print_code()
{
	int i;
	printf("Line\tOP\tL\tM\n");
	for(i=0;i<numInst;i++)
	{
		printf("%d\t%s\t%d\t%d\n",i,code[i].str,code[i].l,code[i].m);
	}
}
void print_stack()
{
	int i;
	int num_index=0;
	for(i=0;(BP>SP?i<SP+4:i<SP);i++)
	{
		if(i == ar_index[num_index])
		{
			printf("| ");
			num_index++;
		}
		printf("%d ", stack[i]);
	}
	printf("\n");
}


char* decode(int op)
{
	char *str;
	if(op==1)
		str = "LIT";
	else if(op==2)
		str = "OPR";
	else if(op==3)
		str = "LOD";
	else if(op==4)
		str = "STO";
	else if(op==5)
		str = "CAL";
	else if(op==6)
		str = "INC";
	else if(op==7)
		str = "JMP";
	else if(op==8)
		str = "JPC";
	else if(op==9)
		str = "SIO";
	else if(op==10)
		str = "SIO";
	else
		str=NULL;
	return str;
}

void lit(Instruction ins)
{
	stack[SP++]=ins.m;
}
void opr(Instruction ins)
{
	switch(ins.m)
	{
		case 0:
			SP = BP-1;
			PC = (stack[SP+3])-1;
			BP = stack[SP+2];
			break;
		case 1:
			stack[SP] = stack[SP]*-1;
			break;
		case 2:
			SP--;
			stack[SP] = stack[SP] + stack[SP+1];
			break;
		case 3:
			SP--;
			stack[SP] = stack[SP] - stack[SP+1];
			break;
		case 4:
			SP--;
			stack[SP] = stack[SP] * stack[SP+1];
			break;
		case 5:
			SP--;
			stack[SP] = stack[SP] / stack[SP+1];
			break;
		case 6:
			stack[SP] = stack[SP] %2;
			break;
		case 7:
			SP--;
			stack[SP] = stack[SP] % stack[SP+1];
			break;
		case 8:
			SP--;
			stack[SP] = stack[SP] == stack[SP+1];
			break; 
		case 9:
			SP--;
			stack[SP] = stack[SP] != stack[SP+1];
			break;
		case 10:
			SP--;
			stack[SP] = stack[SP] < stack[SP+1];
			break;
		case 11:
			SP--;
			stack[SP] = stack[SP] <= stack[SP+1];
			break;
		case 12:
			SP--;
			stack[SP] = stack[SP] > stack[SP+1];
			break;
		case 13:
			SP--;
			stack[SP] = stack[SP] >= stack[SP+1];
			break;
		default:break;
	}
}
void lod(Instruction ins)
{
	stack[SP++] = stack[base(ins.l,(BP-1))+ins.m];
}
void sto(Instruction ins)
{
	//printf("BASE =%d + %d ", base(ins.l,BP),ins.m);
	//printf("Stack at %d is %d\n",SP,stack[SP-1]);
	
	stack[base(ins.l,(BP-1))+ins.m] = stack[SP-1];
	SP--;
	
}
void cal(Instruction ins)
{
	stack[SP+0] = 0;
	stack[SP+1] = base(ins.l,(BP));
	stack[SP+2] = BP;
	stack[SP+3] = PC+1;
	BP = SP+1;
	PC = ins.m;
	ar_index[ar_i++] = SP;//store Frame location for printing
}
void inc(Instruction ins)
{
	SP+=ins.m;
}
void jmp(Instruction ins)
{
	PC = ins.m;
}
void jpc(Instruction ins)
{

	if(stack[SP--]==0)
	{
		PC = ins.m;
	}
}
void sio(Instruction ins)
{
	if(ins.m == 1)
	{
		//Pop
		//print(stack[SP])
		printf("%d", stack[SP]);
		SP--;
	}
	else if(ins.m == 2)
	{
		//push
		SP++;
		scanf("%d", &stack[SP]);
	}
}