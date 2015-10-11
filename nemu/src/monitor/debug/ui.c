#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "common.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
//#include <cstring>

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
        char *tkp=strtok(args,"");
        char s[5];
        if(tkp!=NULL){
        strcpy(s,tkp);
        //printf("%s\n", s);
        tkp=strtok(NULL,"");
        }

        int step=atoi(s);
        if(step==0)
                cpu_exec(1);
        else
                cpu_exec(step);
        return 0;
}

void printwatch();
static int cmd_info(char *args){
        if(strcmp(args,"r")==0){
                printf("eax\t%p\t%x\n",&cpu.eax,cpu.eax);
                printf("ecx\t%p\t%x\n",&cpu.ecx,cpu.ecx); 
                printf("edx\t%p\t%x\n",&cpu.edx,cpu.edx); 
                printf("ebx\t%p\t%x\n",&cpu.ebx,cpu.ebx); 
                printf("esp\t%p\t%x\n",&cpu.esp,cpu.esp); 
                printf("ebp\t%p\t%x\n",&cpu.ebp,cpu.ebp); 
                printf("esi\t%p\t%x\n",&cpu.esi,cpu.esi); 
                printf("edi\t%p\t%x\n",&cpu.edi,cpu.edi); 
                printf("eip\t%p\t%x\n",&cpu.eip,cpu.eip);
                printf("ax\t%p\t%x\n",&cpu.gpr[0]._16,cpu.gpr[0]._16);
                printf("dx\t%p\t%x\n",&cpu.gpr[1]._16,cpu.gpr[1]._16);
                printf("cx\t%p\t%x\n",&cpu.gpr[2]._16,cpu.gpr[2]._16);
                printf("bx\t%p\t%x\n",&cpu.gpr[3]._16,cpu.gpr[3]._16);
                printf("bp\t%p\t%x\n",&cpu.gpr[4]._16,cpu.gpr[4]._16);
                printf("si\t%p\t%x\n",&cpu.gpr[5]._16,cpu.gpr[6]._16);
                printf("di\t%p\t%x\n",&cpu.gpr[6]._16,cpu.gpr[6]._16);
                printf("sp\t%p\t%x\n",&cpu.gpr[7]._16,cpu.gpr[7]._16);
                printf("al\t%p\t%x\n",&cpu.gpr[0]._8[0],cpu.gpr[0]._8[0]);
                printf("ah\t%p\t%x\n",&cpu.gpr[0]._8[1],cpu.gpr[0]._8[1]);
                printf("dl\t%p\t%x\n",&cpu.gpr[1]._8[0],cpu.gpr[1]._8[0]);
                printf("dh\t%p\t%x\n",&cpu.gpr[1]._8[1],cpu.gpr[1]._8[1]);
                printf("cl\t%p\t%x\n",&cpu.gpr[2]._8[0],cpu.gpr[2]._8[0]);
                printf("ch\t%p\t%x\n",&cpu.gpr[2]._8[1],cpu.gpr[2]._8[1]);
                printf("bl\t%p\t%x\n",&cpu.gpr[3]._8[0],cpu.gpr[3]._8[0]);
                printf("bh\t%p\t%x\n",&cpu.gpr[3]._8[1],cpu.gpr[3]._8[1]);
        }//finish the 32bit register.
        else if(strcmp(args,"w")==0){
               printf("NO\t\tWhat\t\tvalue\n");
               printwatch();
        }
		else if(strcmp(args,"flag")==0){
		       printf("CF\t%d\n",cpu.EFLAGS.CF);
			   printf("PF\t%d\n",cpu.EFLAGS.PF);
			   printf("AF\t%d\n",cpu.EFLAGS.AF);
			   printf("ZF\t%d\n",cpu.EFLAGS.ZF);
			   printf("SF\t%d\n",cpu.EFLAGS.SF);
			   printf("TF\t%d\n",cpu.EFLAGS.TF);
			   printf("IF\t%d\n",cpu.EFLAGS.IF);
			   printf("OF\t%d\n",cpu.EFLAGS.OF);
			   printf("CF\t%d\n",cpu.EFLAGS.CF);
		}
        return 0;     
}

static int cmd_x(char *args){   
        char *s=strtok(NULL," ");
        
        int memory_number=atoi(s);    
//get N
        
        char *expr=strtok(NULL," ");
//get expression

        swaddr_t addr;
        sscanf(expr,"%x",&addr);
        //printf("%s %x\n",expr,addr);
        int i;
        for(i=0;i<memory_number;i++){
        if(i%5==0) 
             printf("%x : ",addr);

        uint32_t memory_value=swaddr_read(addr,1);

        printf("%02x ",memory_value);
        addr+=1;
        if((i+1)%5==0||i==memory_number-1)
             printf("\n"); 
}       

        return 0;
}

static int cmd_p(char *args){
       bool make=1;
       printf("Result is %d.\n",expr(args,&make));
       return 0;
}

void new_wp(char *ex,uint32_t content);
static int cmd_w(char *args){
        bool success=1;
        uint32_t pointdata=expr(args,&success);
        new_wp(args,pointdata);                 
        return 0;
}

void free_del(int n);
static int cmd_d(char *args){
        int no=atoi(args);
        printf("no=%d\n",no);
        free_del(no);
        return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
        { "si", "single excecution: si [N]", cmd_si},
        { "info", "print register/watchpoint: info r/w", cmd_info}, 
        { "x", "examine memory: x [N] expr", cmd_x},
        { "p", "evaluate expr: p expr", cmd_p},
        { "w", "watchpoint: w expr", cmd_w},  
        { "d", "delete watchpoint: d [N]", cmd_d},
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);
	/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
