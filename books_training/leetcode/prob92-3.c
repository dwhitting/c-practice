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

    struct ListNode *curr = head, *left_node = head, *next = head, *prev = head, \
        *begin_node = head;
    for (int i = 1; i < left; i++) {
        prev = curr;
        curr = curr->next;
    }
    left_node = prev;
    begin_node = curr;
    if (left != right) {
        for (int i = left; i <= right; i ++) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        begin_node->next = curr;
        left_node->next = prev;
    } else head->next = NULL;


    
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

    for (int i = 4; i > 3; i--) {
        head = add_node(head, i);
    }

    head = reverseBetween(head, 1, 1);

    while (head != NULL) {
        printf("Num: %d\n", head->val);
        head = head->next;
    }


    return 0;
}