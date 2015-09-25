#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */

void new_wp(char *ex,uint32_t content){
        WP *p=free_;
        free_=free_->next;
        p->next=head;
        head=p;
        p=free_;
        //return head;
}


void free_wp(WP *wp){
        WP*p=head;
        head=p->next;
        p->next=free_;
        free_=p;
}


void printwatch(){
        if(head!=NULL)
        {
              WP *temp;
              for(temp=head;temp!=NULL;temp=temp->next){
                  printf("%d        %s        %x\n",temp->NO,temp->str,temp->val);
              }
        }

 
}

int wp_check(){
         int flag=0;
         bool success=true;
         WP*p=head;
         uint32_t v=expr(p->str,&success);
         for(;p->next!=NULL;p=p->next)
         {
               if(p->val!=v){
                    flag=1; 
                    break;
               }
         }
         return flag;
}












