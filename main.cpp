#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>

struct ListNode // ListNode модифицировать нельзя
{
    ListNode *prev = nullptr; // указатель на предыдущий элемент или nullptr
    ListNode *next = nullptr;
    ListNode *rand = nullptr; // указатель на произвольный элемент данного списка, либо `nullptr`
    std::string data;         // произвольные пользовательские данные
};

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

        size_t pos = line.find_last_of(';');
        std::string data = line.substr(0, pos);
        int r_idx = std::stoi(line.substr(pos + 1));

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

    for (size_t i = 0; i < nodes.size(); ++i)
    {
        int r_idx = randIndices[i];
        if (r_idx != -1 && r_idx < nodes.size())
        {
            nodes[i]->rand = nodes[r_idx];
        }
    }

    return nodes.empty() ? nullptr : nodes[0];
}

bool writeFile(ListNode *head, const std::string &fileName)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open())
        return false;

    std::unordered_map<ListNode *, int> mapIdx;

    ListNode *current = head;
    int i = 0;

    while (current != nullptr)
    {
        ListNode *nextNode = current->next;
        mapIdx[current] = i;
        i++;
        current = nextNode;
    }

    int count = i;
    file.write(reinterpret_cast<char *>(&count), sizeof(count));

    current = head;

    while (current != nullptr)
    {
        ListNode *nextNode = current->next;

        size_t len = current->data.size();
        file.write(reinterpret_cast<const char *>(&len), sizeof(len));
        file.write(current->data.c_str(), len);

        int r_idx = (current->rand) ? mapIdx[current->rand] : -1;
        file.write(reinterpret_cast<const char *>(&r_idx), sizeof(r_idx));

        current = nextNode;
    }

    file.close();

    return true;
}

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

ListNode *readBinary(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open())
        return nullptr;

    int count = 0;
    file.read(reinterpret_cast<char *>(&count), sizeof(count));

    std::vector<ListNode *> nodes;
    std::vector<int> randIndices;

    for (int i = 0; i < count; ++i)
    {
        size_t len;
        file.read(reinterpret_cast<char *>(&len), sizeof(len));

        std::string data(len, '\0');
        file.read(&data[0], len);

        int r_idx;
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

    for (size_t i = 0; i < nodes.size(); ++i)
    {
        int r_idx = randIndices[i];
        if (r_idx != -1 && r_idx < nodes.size())
        {
            nodes[i]->rand = nodes[r_idx];
        }
    }

    return nodes.empty() ? nullptr : nodes[0];
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
            std::cout << "| Rand points to:" << newCurr->rand->data;
        }
        else
        {
            std::cout << "| Rand points to: nullptr";
        }
        std::cout << std::endl;

        newCurr = newCurr->next;
    }

    clearNode(newHead);
    newHead = nullptr;

    return 0;
}