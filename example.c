#include "list.h"
#include <stdio.h>

void print(Node *n){
    if (n->type == STRING_NODE)
        printf("%s",n->str);
}

int main(){
    List root;
    init(&root);

    append(&root, make_string("Hello "));

    List *sub = malloc(sizeof(List));
    init(sub);
    append(sub, make_string("world "));
    append(sub, make_string("C List\n"));

    append(&root, make_list(sub));

    list_foreach(&root, print, VISIT_LEAF);


    free_list(&root);
    return 0;
}
