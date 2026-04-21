#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode *prev = NULL, *curr = head, *next = NULL;
    while (curr != NULL) {
        if (curr == head) {
            prev = curr;
            curr = curr->next;
        } else
            curr = next;
        if (curr != NULL && curr->next != NULL) {
            next = curr->next;
            curr->next = prev;
            prev = curr;  
        } else {
            if (curr != NULL)
                curr->next = prev;
            break;
        }
    }

    if (head != NULL && head->next != NULL) {
        head->next = NULL;
        head = curr;
    }

    return head;
    
}

struct ListNode *add_node(struct ListNode *head, int n) {
    struct ListNode *new_node = (struct ListNode*) malloc(sizeof(struct ListNode));
    new_node->val = n;
    new_node->next = head;
}

int main(void)
{
    struct ListNode *head = (struct ListNode*) malloc(sizeof(struct ListNode));
    head->val = 5;
    head->next = NULL;

    for (int i = 4; i > 0; i--) {
        head = add_node(head, i);
    }

    printf("head val: %d\n", head->val);

    head = reverseList(head);


    return 0;
}