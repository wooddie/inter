#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdint>
#include <unordered_map>
#include "list_node.h"
#include "read_file.h"
#include "read_binary.h"
#include "write_file.h"

void clearNode(ListNode *head)
{
    ListNode *current = head;

    while (current != nullptr)
    {
        ListNode *nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

int main()
{
    ListNode *head = readFile("inlet.in");
    ListNode *curr = head;
    while (curr)
    {
        std::cout << "Data: " << curr->data
                  << " | Rand points to: "
                  << (curr->rand ? curr->rand->data : "nullptr")
                  << std::endl;
        curr = curr->next;
    }

    std::cout << std::boolalpha << writeFile(head, "outlet.out") << std::endl;

    clearNode(head);
    head = nullptr;

    ListNode *newHead = readBinary("outlet.out");
    ListNode *newCurr = newHead;
    while (newCurr != nullptr)
    {
        std::cout << "Node Data: " << newCurr->data;

        if (newCurr->rand)
        {
            std::cout << " | Rand points to: " << newCurr->rand->data;
        }
        else
        {
            std::cout << " | Rand points to: nullptr";
        }
        std::cout << std::endl;

        newCurr = newCurr->next;
    }

    clearNode(newHead);
    newHead = nullptr;

    return 0;
}