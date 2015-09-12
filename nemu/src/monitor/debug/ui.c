#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

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
        cpu_exec(step);
        return 0;
}

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
                printf("xxx\n");     
        }//finish the 32bit register.
        else if(strcmp(args,"w")==0){
        printf("waiting...");
        }
        return 0;     
}

static int cmd_x(char *args){
        char *tkp=strtok(args,"");
        char s[5];
        if(tkp!=NULL){
        strcpy(s,tkp);
        //n=atoi(s);
        tkp=strtok(NULL,"");
        }
        int memory_number=atoi(s);
        printf("%d\n",memory_number);    
//get N
        char expr[20];
        if(tkp!=NULL){
        strcpy(expr,tkp);
        tkp=strtok(NULL,"");
        }
//get expression
        long addr=atol(expr);
        //printf("%x/n",addr);
        int *addrp=(int*)addr;
        printf("%p/n",&addrp);
       // for(int i=0;i<memory_number;i++){
        
//}
                
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
        { "si", "single excecution", cmd_si},
        { "i", "print register/watchpoint", cmd_info}, 
        { "x", "examine memory", cmd_x}, 
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
