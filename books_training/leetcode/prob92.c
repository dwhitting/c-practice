#include <stdio.h>
#include <stdlib.h>

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* reverseBetween(struct ListNode* head, int left, int right) {
    struct ListNode *prev = NULL, *curr = head, *next = NULL, \
        *left_node = NULL, *right_node = NULL, *rev_list, *rev_head = NULL, \
        *rev_tail = NULL;

    int i = 0;
    while (curr != NULL) {
        i++;
        if (i == left - 1) {
            left_node = curr;
        }  
        while (i >= left && i <= right){
            if (i == left)
                rev_head = curr;
            next = curr->next;
            if (i > left)
                curr->next = prev;
            prev = curr;
            if (i == right)
                rev_tail = curr;
            curr = next;
            i++;
        }
        if (i == right +1) {
            left_node = rev_tail;
            right_node = rev_head;         
        }
  
        prev = curr;
        curr = curr->next;         
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

    head = reverseBetween(head, 2, 4);


    return 0;
}