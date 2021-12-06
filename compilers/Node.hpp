#include <iostream>
#include <string>

class Node
{
public:
    Node(std::string id, Node *node0, Node *node1, Node *node2, Node *node3)
    {
        this->id = id;
        this->children[0] = node0;
        this->children[1] = node1;
        this->children[2] = node2;
        this->children[3] = node3;
    }
    Node(std::string id)
    {
        this->id = id;
        this->children[0] = nullptr;
        this->children[1] = nullptr;
        this->children[2] = nullptr;
        this->children[3] = nullptr;
    }
    Node(std::string id, Node *node0)
    {
        this->id = id;
        this->children[0] = node0;
        this->children[1] = nullptr;
        this->children[2] = nullptr;
        this->children[3] = nullptr;
    }
    Node(std::string id, Node *node0, Node *node1)
    {
        this->id = id;
        this->children[0] = node0;
        this->children[1] = node1;
        this->children[2] = nullptr;
        this->children[3] = nullptr;
    }
    Node(std::string id, Node *node0, Node *node1, Node *node2)
    {
        this->id = id;
        this->children[0] = node0;
        this->children[1] = node1;
        this->children[2] = node2;
        this->children[3] = nullptr;
    }
    void printPreorder(std::string tab = "")
    {
        std::cout << tab << id << "\n";
        for (int i = 0; i < 4; i++)
        {
            if (children[i])
                children[i]->printPreorder(tab + "\t");
            else
                std::cout << tab << "\tempty " << i << "\n";
        }
    }

private:
    std::string id;
    Node *children[4];
};