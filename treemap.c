#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

//Implemente la función createTreeMap en el archivo treemap.c. Esta función recibe la función de comparación de claves y crea un mapa (TreeMap) inicializando sus variables. El siguiente código muestra como inicializar la función de comparación. Reserve memoria, inicialice el resto de variables y retorne el mapa.
TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * map = (TreeMap *)malloc(sizeof(TreeMap));
    if (map == NULL)
    {
        return NULL;
    }
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;
    return map;
}

//Implemente la función void insertTreeMap(TreeMap * tree, void* key, void * value). Esta función inserta un nuevo dato (key,value) en el árbol y hace que el current apunte al nuevo nodo. Para insertar un dato, primero debe realizar una búsqueda para encontrar donde debería ubicarse. Luego crear el nuevo nodo y enlazarlo. Si la clave del dato ya existe retorne sin hacer nada (recuerde que el mapa no permite claves repetidas).
void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * aux = tree->root;
    TreeNode *aux_parent = NULL;
    while(aux != NULL)
    {
        if(is_equal(tree, key, aux->pair->key))
        {
            return;
        }
        else if (tree->lower_than(key, aux->pair->key))
        {
            aux_parent = aux;
            aux = aux->left; //subarbol izq :)
        }
        else
        {
            aux_parent = aux;
            aux = aux->right; //subarbol der
        }
    }
    //llegué al subarbol vacío donde debo insertar:
    TreeNode * tree_node_aux = createTreeNode(key, value);
    tree_node_aux->parent = aux_parent;
    if (aux_parent == NULL)
    {
        //entonces era la raiz
        tree->root = tree_node_aux;
    }
    else
    {
        //si no es la raiz hay que preguntar si insertamos el "hijo" en el subarbol/izq o der.
        if (tree->lower_than(key, aux_parent->pair->key))
        {
            aux_parent->left = tree_node_aux;
            tree->current = tree_node_aux;
        }
        else
        {
            aux_parent->right = tree_node_aux;
            tree->current = tree_node_aux;
        }
    }
}

//Implemente la función TreeNode * minimum(TreeNode * x). Esta función retorna el nodo con la mínima clave ubicado en el subárbol con raiz x. Para obtener el nodo tiene que, a partir del nodo x, irse por la rama izquierda hasta llegar al final del subárbol. Si x no tiene hijo izquierdo se retorna el mismo nodo.
TreeNode * minimum(TreeNode * x){
    //copia x;
    TreeNode * aux = x;
    while(aux != NULL)
    {
        if(aux->left == NULL)
        {
            return aux;
        }
        aux = aux->left;
    }
    return x;
}

//Implemente la función void removeNode(TreeMap * tree, TreeNode* node). Esta función elimina el nodo node del árbol tree. Recuerde que para eliminar un node existen 3 casos: Nodo sin hijos: Se anula el puntero del padre que apuntaba al nodo Nodo con un hijo: El padre del nodo pasa a ser padre de su hijo Nodo con dos hijos: Descienda al hijo derecho y obtenga el menor nodo del subárbol (con la función minimum). Reemplace los datos (key,value) de node con los del nodo "minimum". Elimine el nodo minimum (para hacerlo puede usar la misma función removeNode).
void removeNode(TreeMap * tree, TreeNode* node) {
    //caso 1:
    if (node->left == NULL && node->right == NULL)
    {
        if (node->parent == NULL)
        {
            //es la raiz:
            tree->root = NULL;
        }
        else
        {
            if (node->parent->left == node)
            {
                node->parent->left = NULL;
            }
            else
            {
                node->parent->right = NULL;
            }
        }
    }
    else if (node->right == NULL || node->left == NULL)
    {
        //caso 2:
        if (node->parent == NULL)
        {
            tree->root = NULL;
        }
        else
        {
            if (node->left != NULL)
            {
                if (node->parent->left == node)
                {
                    node->parent->left = node->left;
                }
                else
                {
                    node->parent->right = node->left;
                }
                node->left->parent = node->parent;
            }
            else
            {
                if (node->parent->right == node)
                {
                    node->parent->right = node->right;
                }
                else
                {
                    node->parent->left = node->right;
                }
                node->right->parent = node->parent;
            }
        }  
    }
    else
    {
        //caso 3:
        TreeNode * aux = minimum(node->right);
        //entonces se reemplazan los valores del nodo a eliminar por los del minimo:
        node->pair->key = aux->pair->key;
        node->pair->value = aux->pair->value;
        //el parent sigue siendo el mismo porque solo copiamos los valores!
        //luego se elimina el minimo:
        removeNode(tree, aux);  
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

//Implemente la función Pair* searchTreeMap(TreeMap* tree, void* key), la cual busca el nodo con clave igual a key y retorna el Pair asociado al nodo. Si no se encuentra la clave retorna NULL. Recuerde hacer que el current apunte al nodo encontrado.
Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root;
    while(aux != NULL)
    {
        if(is_equal(tree, key, aux->pair->key))
        {
            tree->current = aux;
            return aux->pair;
        }
        else if (tree->lower_than(key, aux->pair->key))
        {
            aux = aux->left; //subarbol izq :)
        }
        else
        {
            aux = aux->right; //subarbol der
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

//mplemente las funciones para recorrer la estructura: Pair* firstTreeMap(TreeMap* tree) retorna el primer Pair del mapa (el menor). Pair* nextTreeMap(TreeMap* tree) retornar el siguiente Pair del mapa a partir del puntero TreeNode* current. Recuerde actualizar este puntero.
Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->root;
    while (aux != NULL && aux->left != NULL)
    {
        aux = aux->left;
    }
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->current;
    if (aux == NULL)
    {
        return NULL;
    }
        
    else if (aux->right != NULL)
    {
        //tiene sub arbol derecho
        aux = minimum(aux->right);
        tree->current = aux;
        return aux->pair;
    }
    else
    {
        //no tiene sub arbol derecho, vamos al padre.
        while(aux->parent != NULL && aux->parent->right == aux)
        {
          aux = aux->parent;  
        }
        tree->current = aux;
        return aux->pair;
    }
}
