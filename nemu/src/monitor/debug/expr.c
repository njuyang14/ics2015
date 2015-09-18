#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,NUM,PLU,MIN,MULT,DIVI,LP,RP
        
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
        {"[0-9]+",  NUM}, 
	{"\\+", PLU},					// plus
        {"\\-", MIN}, 
        {"\\*", MULT}, 
        {"\\/", DIVI}, 
        {"\\(", LP}, 
        {"\\)", RP}, 
	{"==", EQ}						// equal
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

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
                                        case(NOTYPE):break;
                                        case(NUM):{
                                                  strncpy(tokens[nr_token].str,substr_start,substr_len); 
                                                  tokens[i].type=NUM;  
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

temp_op operator[32];
int nr_op=0;

/*int level(int type){
        if(type==PLU||type==MIN)return 1;
        else if(type==MULT||type==DIVI)return 2;
}*/

int dominant(int p,int q){
        return 1;
        /*int i;
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

        int t;
        for(t=0;t<nr_op;t++){
               printf("%s ",operator[t].str);
        }
        int dominant_position=operator[0].add;
        int k;
        for(k=0;k<nr_op-1;k++){
        if(level(operator[k].type)>level(operator[k+1].type))
               dominant_position=operator[k+1].add;
        }
        
        return dominant_position;*/
}

bool check_parenthese(int p,int q){
        int i;
        int count=0;
        if(tokens[p].type!=LP||tokens[p].type!=RP)return false;
        for(i=p;i<q;i++){
               if(tokens[i].type==LP)
                         count++;
               else if(tokens[i].type==RP)
                         count--;
               if(count<=0)
                         return false;                      
        }
        return true;
}

uint32_t eval(int p,int q){
	uint32_t v=0;
        if(p>q){
               printf("bad expression");
               return 0;
               }
        else if(p==q){
               uint32_t val;
               sscanf(tokens[p].str,"%x",&val);
               return val;
        }
        else if(check_parenthese(p,q)==true){
        
               return eval(p+1,q-1);
        }
        else{
               int op=dominant(p,q);
               printf("op=%d\n",op);
               uint32_t val1 = eval(p, op - 1);
               uint32_t val2 = eval(op + 1, q);
                
               //printf("%s\n",tokens[op].str);
               if(tokens[op].type==PLU)v=val1+val2;
               else if(tokens[op].type==MIN)v=val1-val2;
               else if(tokens[op].type==MULT)v=val1*val2;
               else if(tokens[op].type==DIVI)v=val1/val2;  
        }
        return v;
}


uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
        //test tokens[32].
        int j;
        for(j=0;j<=nr_token;j++)
                printf("%s ",tokens[j].str);
        printf("\n");
        
        eval(0,nr_token-1);
	/* TODO: Insert codes to evaluate the expression. */
	panic("please implement me");
	return 0;
}

