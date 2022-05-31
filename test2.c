#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lbr/jrb.h"
#include "lbr/dllist.h"
int main(int argc, char const *argv[])
{
    JRB tree, root;
    tree = make_jrb();
    jrb_insert_int(tree, 1, new_jval_i(1));
    jrb_insert_int(tree, 1, new_jval_i(1));
    jrb_insert_int(tree, 2, new_jval_i(1));
    jrb_traverse(root, tree)
    {
        printf("%d\n", jval_i(root->key));
    }
    return 0;
}