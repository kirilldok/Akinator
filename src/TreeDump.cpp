#include"BinTree.h"

const char* TREE_LOG_HTML   = "./run/treelog.html";
const char* TREE_GRAPHVIZ   = "./run/graphviz.gv";
const char *SYSTEM_CMD_LINE = "dot -Tpng run/graphviz.gv -o run/graphviz/graphviz";
const char *HTML_MAKE_IMG   = "<img src =\"run/graphviz/graphviz";
const int   LIST_CMD_SIZE   = 128;


static void DrawNodes(const Node_t* node, FILE* graffile)
{
    assert(node); assert(graffile);

    fprintf(graffile, "\t%lu[shape=record,  "
                      "style=\"filled\",    "
                      "fillcolor=\"yellow\","
                      "label=\"%s\"];\n",
                       node->num, node->data);

    if(node->left != nullptr)
        DrawNodes(node->left, graffile);
    if(node->right!= nullptr)
        DrawNodes(node->right, graffile);
}

static void DrawBranches(const Node_t* node, FILE* graffile)
{
    assert(node); assert(graffile);

    if (node->left != nullptr && node->right != nullptr)
    {
        fprintf(graffile,
                "\t%lu -> %lu[color=\"green\", label=\"NO\", "
                "fontsize=12]\n", node->num, node->left->num);
        fprintf(graffile,
                "\t%lu -> %lu[color=\"blue\", label=\"YES\", "
                "fontsize=12]\n", node->num, node->right->num);
    }

    if (node->left != nullptr)
        DrawBranches(node->left, graffile);
    if (node->right != nullptr)
        DrawBranches(node->right, graffile);
}



size_t TreeDump(Tree_t* tree)
{
    assert(tree);

    FILE* log = fopen("TREE_LOG_HTML", "a+"); assert(log);

    fprintf(log, "<pre>\n");
    fprintf(log, "##################TREEDUMP##################\n");
    fprintf(log, "TREE SIZE: %lu\n"
                 "TREE ROOT: %p\n", tree->size, tree->root);

    FILE* graffile = fopen("TREE_GRAPHVIZ", "w+"); assert(graffile);

    fprintf(graffile,
            "digraph TREE{\n"
            "\trankdir=UD;\n\n"
            "\tnode[color=\"red\",fontsize=14];\n\n");

    DrawNodes(tree->root, graffile);

    fprintf(graffile, "\n");

    DrawBranches(tree->root, graffile);

    fprintf(graffile, "\n");
    fprintf(graffile, "}");
    fclose(graffile);

    static int cntImg = 1;
    char *str = (char*)calloc(LIST_CMD_SIZE, sizeof(char));

    sprintf(str, "%s%d%s", SYSTEM_CMD_LINE, cntImg, ".png");
    system(str);
    sprintf(str, "%s%d%s", HTML_MAKE_IMG, cntImg, ".png\">");

    fprintf(log, "%s", str);
    cntImg = cntImg + 1;
    free(str);

    fprintf(log, "##################################################\n");
    fclose(log);

    return NO_ERROR;
}


