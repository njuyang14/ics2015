#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,NUM,PLU,MIN,MULT,DIVI,LP,RP,REG,HEX,NEGA,POINT,NOT,AND,OR,NOTEQ,
        
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
         
	{"\\+", PLU},					// plus
        {"\\-", MIN}, 
        {"\\*", MULT}, 
        {"\\/", DIVI}, 
        {"\\(", LP}, 
        {"\\)", RP},
        {"\\$[a-zA-Z]{2,3}", REG},  
        {"0[xX][0-9a-fA-F]+", HEX},
        {"[0-9]+",  NUM},
        {"\\!\\=",  NOTEQ},
        {"\\!",  NOT},
        {"\\&&",  AND},
        {"\\|\\|",  OR}, 
	{"\\=\\=", EQ}						// equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex(){
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);

			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);        //regcomp():compile the regex.

			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);

		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				/*Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);*/
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
                                        case(NOTYPE):break;
                                        case(NUM):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len); 
                                                  tokens[nr_token].type=NUM;
                                                    
                                                  break;}
                                        case(PLU):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=PLU;
                                                    
                                                  break;}
                                        case(MIN):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=MIN;
                                                   
                                                  break;}
                                        case(MULT):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=MULT;
                                                   
                                                  break;}
                                        case(DIVI):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=DIVI;
                                                   
                                                  break;}
                                        case(LP):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=LP;
                                                   
                                                  break;}
                                        case(RP):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=RP;
                                                   
                                                  break;}
                                        case(REG):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  int sz;
                                                  for(sz=0;sz<substr_len-1;sz++){
                                                           tokens[nr_token].str[sz]=tokens[nr_token].str[sz+1];
                                                  } 
                                                  tokens[nr_token].str[substr_len-1]='\0';
                                                  //printf("reg=%s\n",tokens[nr_token].str);
                                                  tokens[nr_token].type=REG;
                                                  break;}
                                        case(HEX):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=HEX;
                                                  break;}
                                        case(NOT):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=NOT;
                                                  break;}
                                        case(AND):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=AND;
                                                  break;}
                                        case(NOTEQ):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=NOTEQ;
                                                  break;}
                                        case(EQ):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=EQ;
                                                  break;}
                                        case(OR):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                                                  tokens[nr_token].type=OR;
                                                  break;}
					default: panic("please implement me");
				}
                                nr_token++;
                                break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

typedef struct{
        int type;
        int add;
        char str[32]; 
}temp_op;


int level(int type){
        if(type==PLU||type==MIN)return 5;
        else if(type==EQ||type==NOTEQ)return 3;//  ==
        else if(type==OR)return 1;//  ||
        else if(type==AND)return 2;//  &&
        else if(type==NOT)return 7;//  !
        else if(type==MULT||type==DIVI)return 6;
        else if(type==NEGA||type==POINT)return 7;//  - *
        else return 10;
}

int dominant(int p,int q){
        //return 1;
        temp_op operator[32]; 
        int i;
        int nr_op=0;
        for(i=p;i<=q;i++){
               int count=0;
               if(tokens[i].type==LP){
                         count++;
                         int j;
                         for(j=i;i<=q;j++){
                                 if(tokens[j].type==RP)count--;
                                 if(count==0)break;
                         }
                         i=j;
               }
               else if(tokens[i].type!=NUM){
                         strcpy(operator[nr_op].str,tokens[i].str);
                         operator[nr_op].add=i;
                         operator[nr_op].type=tokens[i].type;
                         nr_op++;    
               }
        }

        /*int t;
        for(t=0;t<nr_op;t++){
               printf("%s ",operator[t].str);
        }*/
        //test the dominant function.


        int dominant_position=operator[0].add;
        int k;
        for(k=0;k<nr_op-1;k++){
        if(level(operator[k].type)>level(operator[k+1].type))
               dominant_position=operator[k+1].add;
        }
        
        return dominant_position;
}

int check_parenthese(int p,int q){
        int i;
        int count=0;
        if(tokens[p].type!=LP||tokens[q].type!=RP)return 0;
        for(i=p;i<q;i++){
               if(tokens[i].type==LP)
                         count++;
               else if(tokens[i].type==RP)
                         count--;
               if(count<=0)
                         return 0;                      
        }
        return 1;
}

uint32_t eval(int p,int q){
       //uint32_t v=0;
        if(p>q){
               printf("bad expression in eval()");
               return 0;
               }
        else if(p==q){
               if(tokens[p].type==NUM){
                    /*if(tokens[p-1].type==NEGA){
                          uint32_t val;
                          sscanf(tokens[p].str,"%d",&val);
                          return -val;
                    }
                    else{*/
                          uint32_t val;
                          sscanf(tokens[p].str,"%d",&val);
                          return val;
                   // }
               }
               else if(tokens[p].type==REG){
                          if(strcmp(tokens[p].str,"eax")==0){
                                      return cpu.eax;
                          }
                          else if(strcmp(tokens[p].str,"edx")==0){
                                      return cpu.edx;
                          }
                          else if(strcmp(tokens[p].str,"ecx")==0){
                                      return cpu.ecx;
                          }
                          else if(strcmp(tokens[p].str,"ebx")==0){
                                      return cpu.ebx;
                          }
                          else if(strcmp(tokens[p].str,"ebp")==0){
                                      return cpu.ebp;
                          }
                          else if(strcmp(tokens[p].str,"esi")==0){
                                      return cpu.esi;
                          }
                          else if(strcmp(tokens[p].str,"edi")==0){
                                      return cpu.edi;
                          }
                          else if(strcmp(tokens[p].str,"esp")==0){
                                      return cpu.esp;
                          }
                          else if(strcmp(tokens[p].str,"eip")==0){
                                      return cpu.eip;
                          }
                          else if(strcmp(tokens[p].str, "ax")==0){
				      return cpu.gpr[0]._16;
                          }
                          else if(strcmp(tokens[p].str, "dx")==0){
                                      return cpu.gpr[1]._16;
                          }
                          else if(strcmp(tokens[p].str, "cx")==0){
                                      return cpu.gpr[2]._16;
                          }
                          else if(strcmp(tokens[p].str, "bx")==0){
                                      return cpu.gpr[3]._16;
                          }
                          else if(strcmp(tokens[p].str,"bp")==0){
                                      return cpu.gpr[4]._16;
                          }
                          else if(strcmp(tokens[p].str,"si")==0){
                                      return cpu.gpr[5]._16;
                          }
                          else if(strcmp(tokens[p].str,"di")==0){
                                      return cpu.gpr[6]._16;
                          }
                          else if(strcmp(tokens[p].str,"sp")==0){
                                      return cpu.gpr[7]._16;
                          }
                          else if(strcmp(tokens[p].str,"al")==0){
                                      return cpu.gpr[0]._8[0];
                          }
                          else if(strcmp(tokens[p].str,"ah")==0){
                                      return cpu.gpr[0]._8[1];
                          }
                          else if(strcmp(tokens[p].str,"dl")==0){
                                      return cpu.gpr[1]._8[0];
                          }
                          else if(strcmp(tokens[p].str,"dh")==0){
                                      return cpu.gpr[1]._8[1];
                          }
                          else if(strcmp(tokens[p].str,"cl")==0){
                                      return cpu.gpr[2]._8[0];
                          }
                          else if(strcmp(tokens[p].str,"ch")==0){
                                      return cpu.gpr[2]._8[1];
                          }
                          else if(strcmp(tokens[p].str,"bl")==0){
                                      return cpu.gpr[3]._8[0];
                          }
                          else if(strcmp(tokens[p].str,"bh")==0){
                                      return cpu.gpr[3]._8[1];
                          }
                          else 
                                      return 0;
              }
              else if(tokens[p].type==HEX){
                          uint32_t val;
                          //swaddr_t addr;
                          sscanf(tokens[p].str,"%x",&val);
                          return val;
              }
/*              else if((tokens[p].type==HEX||tokens[p].type==REG)&&tokens[p-1].type==POINT){
              swaddr_t addr;
              if(tokens[p].type==HEX){
                          //uint32_t val;
                          sscanf(tokens[p].str,"%x",&addr);
                              
              }
              else if(tokens[p].type==REG){
                          if(strcmp(tokens[p].str,"eax")==0){
                                       sscanf(tokens[p].str,"%x",&addr);
                          }
                          else if(strcmp(tokens[p].str,"edx")==0){
                                       sscanf(tokens[p].str,"%x",&addr);
                          }
                          
                          else if(strcmp(tokens[p].str,"ecx")==0){
                                       sscanf(tokens[p].str,"%x",&addr);
                          }
                          
                          else if(strcmp(tokens[p].str,"ebx")==0){
                                       sscanf(tokens[p].str,"%x",&addr);
                          }
                          
                          else if(strcmp(tokens[p].str,"ebp")==0){
                                       sscanf(tokens[p].str,"%x",&addr);
                          }
                          
                          else if(strcmp(tokens[p].str,"esi")==0){
                                       sscanf(tokens[p].str,"%x",&addr);
                          }
                         
                          else if(strcmp(tokens[p].str,"edi")==0){
                                       sscanf(tokens[p].str,"%x",&addr);
                          }
                          
                          else if(strcmp(tokens[p].str,"esp")==0){
                                       sscanf(tokens[p].str,"%x",&addr);
                          }
                          
                          else if(strcmp(tokens[p].str,"eip")==0){
                                       sscanf(tokens[p].str,"%x",&addr);
                          }
                          else addr=0;

              }
                          return swaddr_read(addr,4);

              }*/
              else return 0;
        }
	else if(check_parenthese(p,q)==1){
        
               return eval(p+1,q-1);
        }
        else{
                         int op=dominant(p,q);
                         //printf("op=%d\n",op);
               if(op!=p){
                         uint32_t val1 = eval(p, op - 1);
                         uint32_t val2 = eval(op + 1, q);
                         int op_type=tokens[op].type;
  
                         //printf("tokens[op]=%s\n",tokens[op].str);
                         if(op_type==PLU)return val1+val2;
                         else if(op_type==MIN)return val1-val2;
                         else if(op_type==MULT)return val1*val2;
                         else if(op_type==DIVI)return val1/val2; 
                         else if(op_type==EQ){
                                  if(val1==val2)return 1;
                                  else return 0;
                         }
                         else if(op_type==NOTEQ){
                                  if(val1==val2)return 0;
                                  else return 1;
                         }
                         else if(op_type==AND){
                                  if(val1==1&&val2==1)return 1;
                                  else return 0;
                         }
                         else if(op_type==OR){
                                  if(val1==1||val2==1)return 1;
                                  else return 0;
                         }
                         else assert(0);  
              }
              if(tokens[p].type==NEGA){
                         return -eval(p+1,q);
              }
              else if(tokens[p].type==POINT){
                         swaddr_t temp=eval(p+1,q);
                         return swaddr_read(temp,4);
              }
              else if(tokens[p].type==NOT){
                         return (eval(p+1,q)==0)?1:0;
              }

        }
      return 0;
     
}


uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

        //test tokens[32].
        int i;
        for(i=0;i<=nr_token;i++){
                if(tokens[i].type == MIN && (i == 0 || tokens[i - 1].type == PLU||tokens[i - 1].type ==MIN||tokens[i - 1].type ==MULT||tokens[i - 1].type ==DIVI||tokens[i - 1].type ==LP )){
		tokens[i].type = NEGA;
                }
                //printf("%s ",tokens[i].str);
        }
        //printf("\n");

        for(i=0;i<=nr_token;i++){
                if(tokens[i].type == MULT && (i == 0 || tokens[i - 1].type == PLU||tokens[i - 1].type ==MIN||tokens[i - 1].type ==MULT||tokens[i - 1].type ==DIVI||tokens[i - 1].type ==LP )){
                tokens[i].type =POINT ;
                }
        }
        
        uint32_t result=eval(0,nr_token-1);
	/* TODO: Insert codes to evaluate the expression. */
	//panic("please implement me");
	return result;
}

