#include <vector>
#include <fstream>
#include <iostream>
#include <cstdint>
#include "list_node.h"


ListNode *readFile(const std::string &fileName)
{
    std::ifstream inputFile(fileName);

    if (!inputFile.is_open())
        return nullptr;

    std::string line;
    std::vector<ListNode *> nodes;
    std::vector<int> randIndices;

    while (std::getline(inputFile, line))
    {
        if (line.empty())
            continue;

        uint64_t pos = line.find_last_of(';');

        if (pos == std::string::npos)
        {
            std::cerr << "Warning: Skipping invalid line: " << line << std::endl;
            continue;
        }

        std::string data = line.substr(0, pos);
        int32_t r_idx = std::stoi(line.substr(pos + 1));

        ListNode *node = new ListNode();
        node->data = std::move(data);

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