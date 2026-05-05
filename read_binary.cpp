#include <vector>
#include <fstream>
#include <iostream>
#include <cstdint>
#include "list_node.h"

ListNode *readBinary(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::binary);

    if (!file.is_open())
        return nullptr;

    int32_t count = 0;
    file.read(reinterpret_cast<char *>(&count), sizeof(count));

    std::vector<ListNode *> nodes;
    std::vector<int32_t> randIndices;

    for (int32_t i = 0; i < count; ++i)
    {
        uint32_t len;
        file.read(reinterpret_cast<char *>(&len), sizeof(len));

        std::string data(len, '\0');
        file.read(&data[0], len);

        int32_t r_idx;
        file.read(reinterpret_cast<char *>(&r_idx), sizeof(r_idx));

        ListNode *node = new ListNode();
        node->data = data;

        if (!nodes.empty())
        {
            node->prev = nodes.back();
            nodes.back()->next = node;
        }

        nodes.push_back(node);
        randIndices.push_back(r_idx);
    }

    for (uint32_t i = 0; i < nodes.size(); ++i)
    {
        int32_t r_idx = randIndices[i];
        if (r_idx != -1 && r_idx < nodes.size())
        {
            nodes[i]->rand = nodes[r_idx];
        }
    }

    return nodes.empty() ? nullptr : nodes[0];
}