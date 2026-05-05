#include <fstream>
#include <unordered_map>
#include <cstdint>
#include "list_node.h"
#include "write_file.h"

bool writeFile(ListNode *head, const std::string &fileName)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open())
        return false;

    std::unordered_map<ListNode *, int32_t> mapIdx;

    ListNode *current = head;
    int32_t i = 0;

    while (current != nullptr)
    {
        ListNode *nextNode = current->next;
        mapIdx[current] = i;
        i++;
        current = nextNode;
    }

    int32_t count = i;
    file.write(reinterpret_cast<char *>(&count), sizeof(count));

    current = head;

    while (current != nullptr)
    {
        ListNode *nextNode = current->next;

        uint32_t len = current->data.size();
        file.write(reinterpret_cast<const char *>(&len), sizeof(len));
        file.write(current->data.c_str(), len);

        int32_t r_idx = (current->rand) ? mapIdx[current->rand] : -1;
        file.write(reinterpret_cast<const char *>(&r_idx), sizeof(r_idx));

        current = nextNode;
    }

    file.close();

    return true;
}