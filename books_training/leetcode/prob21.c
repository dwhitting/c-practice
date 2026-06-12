#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode *addNode(struct ListNode *res, int in_val) {
    if (res == NULL) {
        res = (struct ListNode *)malloc(sizeof(struct ListNode));
        res->val = in_val;
    }
    else {
        struct ListNode *curr = res;
        while (curr->next != NULL)
            curr = curr->next;

        struct ListNode *temp = (struct ListNode *)malloc(sizeof(struct ListNode));
        temp->val = in_val;
        temp->next = NULL;
        curr->next = temp;
    }
    return res;
}
struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {

    struct ListNode *temp = NULL, *ret_list = NULL;
    while (list1 != NULL && list2 != NULL) {
            if (list1->val < list2->val) {
                ret_list = addNode(ret_list, list1->val);
                list1 = list1->next;
                //break;
            } else {
                ret_list = addNode(ret_list, list2->val);
                list2 = list2->next;
                //break;
            }
    }
    while (list1 != NULL) {
        ret_list = addNode(ret_list, list1->val);
        list1 = list1->next;
    }
    while (list2 != NULL) {
        ret_list = addNode(ret_list, list2->val);
        list2 = list2->next;
    }
    return ret_list;
}

int main(void)
{
    struct ListNode *list1 = NULL;
    for (int i = 1; i <= 15; i++) {
        list1 = addNode(list1, i);
    }

    struct ListNode *list2 = NULL;
    for (int i = 2; i <= 20; i+= 2) {
        list2 = addNode(list2, i);
    }

    // while (list1 != NULL) {
    //     printf("l1: %d, l2: %d\n", list1->val, list2->val);
    //     list1 = list1->next;
    //     list2 = list2->next;
    // }
        
    struct ListNode *final = mergeTwoLists(list1, list2);
    
    while (final != NULL) {
        printf("f: %d\n", final->val);
        final = final->next;
    }


    return 0;
}