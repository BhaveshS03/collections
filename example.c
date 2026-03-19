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
    append(&root, make_string("what "));

    Node *x = get(&root, 1);
    Node *y = get(x->list,1);
    char* str = y->str;
    str[0] = "qsa";
    print(get(x->list,1));
    free_list(&root);
    return 0;
}
