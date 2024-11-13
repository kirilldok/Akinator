#include "BinTree.h"
#include "FileFunc.h"

const char *INPUT_FILE_NAME  = "../run/data.txt";
const char *OUTPUT_FILE_NAME = "../run/data.txt";


size_t TreeCtor(Tree_t* tree)
{
    assert(tree);

    tree->root = (Node_t*)calloc(1, sizeof(Node_t));
    fprintf(stderr, "root = %p\n", tree->root);
    if(tree->root == nullptr)
    {
        fprintf(stderr, "Can't allocate memory for root\n");
        return ALLOCATED_FAILURE;
    }

    tree->root->data = (Code_t*)calloc(STR_MAX_SIZE, sizeof(Code_t));
    if(tree->root->data == nullptr)
    {
        fprintf(stderr, "Can't allocate memory for root data\n");
        return ALLOCATED_FAILURE;
    }

    tree->root->num = 1;
    tree->size = 0;

    return NO_ERROR;
}


Node_t* TreeInsert(Tree_t* tree, Node_t* node, char* str, const NodeChild child)
{
    assert(tree != nullptr);
    assert(node != nullptr);
    assert(str != nullptr);

    Node_t *newNode = nullptr;

    #define TREE_INSERT_(branch)                                            \
        {                                                                   \
        node->branch = (Node_t*)calloc(1, sizeof(Node_t));                  \
        if (node->branch == nullptr)                                        \
        {                                                                   \
            return nullptr;                                                 \
        }                                                                   \
        node->branch->data = (Code_t*)calloc(STR_MAX_SIZE, sizeof(Code_t)); \
        if (node->branch->data == nullptr)                                  \
        {                                                                   \
            return nullptr;                                                 \
        }                                                                   \
        strcpy(node->branch->data, str);                                    \
        newNode = node->branch;                                             \
        }

    if (child == LEFT_CHILD)
    {
        TREE_INSERT_(left);
    }
    else
    {
        TREE_INSERT_(right);
    }

    #undef TREE_INSERT_

    if (tree->size == 0)
    {
        Node_t *newBeginNode = tree->root->left;
        free(tree->root->data);
        free(tree->root);
        tree->root = newBeginNode;
    }
    tree->size = tree->size + 1;
    newNode->num = tree->size + 1;

    return newNode;
}


static char* TreeReadData()
{
    FILE *finput = fopen(INPUT_FILE_NAME, "r");

    char* str = nullptr;
    int NumberBytesFile = GetFileSize(finput);
    if(NumberBytesFile > 0)
    {
        str = (char*)calloc((size_t)NumberBytesFile, sizeof(char));
        str = (char*)ReadFile(finput, str, NumberBytesFile);
    }

    fclose(finput);

    return str;
}


static char* StrBufferFindEndPhrase(char *str)
{
    assert(str != nullptr);

    char *firstTypeSymbol = strchr(str, '?');
    char *secondTypeSymbol = strchr(str, '!');

    #define RETURN_END_PHRASE_(num)        \
        {                                  \
            *(num##TypeSymbol - 1) = '\0'; \
            return (num##TypeSymbol - 1);  \
        }

    if (firstTypeSymbol == nullptr)
    {
        RETURN_END_PHRASE_(second);
    }

    if (secondTypeSymbol == nullptr)
    {
        RETURN_END_PHRASE_(first);
    }

    if (firstTypeSymbol < secondTypeSymbol)
    {
        RETURN_END_PHRASE_(first);
    }
    else
    {
        RETURN_END_PHRASE_(second);
    }

    #undef RETURN_END_PHRASE_
}


static char* StrBufferFindNewNode(char *str)
{
    assert(str != nullptr);

    while (*str != '{' && *str != '\0')
    {
        str = str + 1;
    }

    return str;
}


static char* NodeBuild(Tree_t *tree, Node_t *node, char *str)
{
    assert(tree != nullptr);
    assert(node != nullptr);
    assert(str != nullptr);

    if (*str == '{')
    {
        str = str + 2;
        char number[1] = {};

        strncpy(number, str, 1);
        NodeChild child = (atoi(number) == 1) ? LEFT_CHILD : RIGHT_CHILD;
        str = str + 1;

        char *phraseEnd = StrBufferFindEndPhrase(str);
        Node_t *newNode = TreeInsert(tree, node, str, child);

        if (*(phraseEnd + 1) == '?')
        {
            str = phraseEnd + 3;
            str = NodeBuild(tree, newNode, str);
            str = NodeBuild(tree, newNode, str);
        }
        else
        {
            str = StrBufferFindNewNode(phraseEnd + 1);
        }
    }

    return str;
}


size_t TreeBuild(Tree_t *tree)
{
    assert(tree != nullptr);

    char *str = TreeReadData();
    if (str == nullptr)
    {
        return TREE_FILL_ERROR;
    }

    char *strCopy = str;
    NodeBuild(tree, tree->root, strCopy);

    TreeDump(tree);

    free(str);
    return NO_ERROR;
}


static void  DeleteNode(Node_t* node)
{
    fprintf(stderr, "node = %p\n", node);
    Node_t* left  = node->left;
    Node_t* right = node->right;
    //free(node->data);
    free(node);

    if( left != nullptr )
    {
        fprintf(stderr, "node left = %p\n", left);
        DeleteNode(left);
    }
    if( right != nullptr )
    {
        fprintf(stderr, "node right = %p\n", right);
        DeleteNode(right);
    }
}


size_t TreeDtor(Tree_t* tree)
{
    if(tree == nullptr)
    {
        return TREE_NULL_PTR;
    }
    fprintf(stderr, "dtor root = %p\n", tree->root);
    DeleteNode(tree->root);
    tree->size = 0;

    return NO_ERROR;
}


size_t PrintNode(Node_t* node, FILE* logfile, size_t rang, size_t nodecounter)
{
    if (logfile == nullptr)
    {
        fprintf(stderr, "Can't write in file pointed by NULL pointer\n");
        return FILE_NULL_PTR;
    }
    if( node == nullptr)
    {
        return NO_ERROR;
    }

    fprintf(logfile, "(");


    if( node->left )
    {
        nodecounter++;
        PrintNode(node->left, logfile, rang, nodecounter);
    }

    fprintf(logfile, " %s ", node->data);

    if( node->right )
    {
        nodecounter++;
        PrintNode(node->right, logfile, rang, nodecounter);
    }

    fprintf(logfile, ")");
    rang++;

    return NO_ERROR;
}


size_t PrintTree(Tree_t* tree, FILE* logfile)
{
    if (logfile == nullptr)
    {
        fprintf(stderr, "Can't write in file pointed by NULL pointer\n");
        return FILE_NULL_PTR;
    }

    PrintNode(tree->root, logfile, 0, 0);
    return NO_ERROR;
}


static void NodeSaveInFile(Node_t* node, FILE* foutput, NodeChild child)
{
    assert(node != nullptr);
    assert(foutput != nullptr);

    if (node->left != nullptr)
    {
        fprintf(foutput, "{ %d%s ? ", (int)child, node->data);

        if (node->left  != nullptr) NodeSaveInFile(node->left, foutput, LEFT_CHILD);
        if (node->right != nullptr) NodeSaveInFile(node->right, foutput, RIGHT_CHILD);

        fprintf(foutput, "} ");
    }
    else
    {
        fprintf(foutput, "{ %d%s ! } ", (int)child, node->data);
    }
}


size_t TreeSaveInFile(Tree_t* tree)
{
    assert(tree != nullptr);

    FILE *data = fopen(OUTPUT_FILE_NAME, "w");

    NodeSaveInFile(tree->root, data, LEFT_CHILD);

    TreeDump(tree);

    fclose(data);
    return NO_ERROR;
}
