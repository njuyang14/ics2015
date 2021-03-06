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
        strcpy(p->str,ex);
        p->val=content;
        free_=free_->next;
        p->next=head;
        head=p;
       //p=free_;
        //return head;
}


void free_wp(WP *wp){
        if(wp==head){
              WP*p=head;
              if(head!=NULL){
                     head=p->next;
                     p->next=free_;
                     free_=p;
              }
        }
        else if(wp->next==NULL){
              WP*p=head;
              for(;p!=NULL;p=p->next){
                     if(p->next->NO==wp->NO){
                            p->next=NULL;
                            wp->next=free_;
                            free_=wp;
                            break;
                     }
              }
   
        }
        else{
              WP*p=head;
              for(;p->next!=NULL;p=p->next){
                     if(p->next->NO==wp->NO){
                            //wp=p->next;
                            p->next=wp->next;
                            wp->next=free_;
                            free_=wp;
                            break;
                     }
              }
        } 
}

void free_del(int n){
        WP*p=head;
        for(;p!=NULL;p=p->next){
              if(n==p->NO)break;
        }
        free_wp(p);
}

void printwatch(){
        if(head!=NULL)
        {
              WP *temp;
              for(temp=head;temp!=NULL;temp=temp->next){
                  printf("%d\t\t%s\t\t%x\n",temp->NO,temp->str,temp->val);
              }
        }
        else
             printf("There is no watchpoint.\n");
}

int wp_check(){
         int flag=0;
         bool success=true;
         WP*p=head;
         if(head!=NULL){
                for(;p!=NULL;p=p->next){
                      uint32_t new_val=expr(p->str,&success);
                      if(p->val!=new_val){
                      flag=1;
                      p->val=new_val;
                      }
                }
         }
         return flag;
}












