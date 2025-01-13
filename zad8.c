/* Napisati program koji omogućava rad s binarnim stablom pretraživanja. Treba
 omogućiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
 level order), brisanje i pronalaženje nekog elementa.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _binaryTree;
typedef struct _binaryTree* Position;
typedef struct _binaryTree {
    int el;
    Position left;
    Position right;
} BinaryTree;

Position insert(int el, Position root);
Position search(int el, Position root);
Position deleteElement(int el, Position root);
int inorder(Position root);
int preorder(Position root);
int postorder(Position root);
int levelOrder(Position root);

int main() {

    Position root = NULL;
    int el = 0;
    printf("Unesite vrijednost prvog clana binarnog stabla: ");
    scanf("%d", &el);
    root = insert(el, root);


    int command = 1;
    do {
        printf("\nMenu:\n");
        printf("1 - Unos novog elementa\n");
        printf("2 - Ispis binarnog stabla - inorder\n");
        printf("3 - Ispis binarnog stabla - preorder\n");
        printf("4 - Ispis binarnog stabla - postorder\n");
        printf("5 - Ispis binarnog stabla - level order\n");
        printf("6 - Pretrazivanje binarnog stabla\n");
        printf("7 - Brisanje nekog elementa\n");
        printf("8 - izlaz\n");
        printf("[----->");
        scanf("%d", &command);

        switch (command) {
        case 1:
            printf("Unesite vrijednost novog elementa: ");
            scanf("%d", &el);
            insert(el, root);
            break;
        case 2:
            inorder(root);
            break;
        case 3:
            preorder(root);
            break;
        case 4:
            postorder(root);
            break;
        case 5:
            levelOrder(root);
            break;
        case 6:
            printf("Unesite vrijednost elementa kojeg zelite traziti: ");
            scanf("%d", &el);
            Position result = search(el, root);
            if (result != NULL) {
                printf("Element postoji u stablu\n");
            }
            else {
                printf("Element ne postoji u stablu\n");
            }
            break;
        case 7:
            printf("Unesite vrijednost elementa kojeg zelite izbrisati: ");
            scanf("%d", &el);
            deleteElement(el, root);
            break;
        case 8:
            printf("Izlaz\n");
            break;
        default:
            printf("Ne postoji ta naredba\n");
        }
    } while (command != 8);

    return 0;
}

Position insert(int el, Position root) {
    if (root == NULL) {
        Position newElement = (Position)malloc(sizeof(BinaryTree));
        if (newElement == NULL) {
            printf("Malloc error\n");
            return -1;
        }
        else {
            newElement->el = el;
            newElement->left = NULL;
            newElement->right = NULL;
        }
        root = newElement;
    }
    else if (el < root->el) {
        root->left = insert(el, root->left);
    }
    else if (el > root->el) {
        root->right = insert(el, root->right);
    }
    return root;
}

Position search(int el, Position root) {
    if (root == NULL) {
        return NULL;
    }
    else if (el < root->el) {
        root->left = search(el, root->left);
    }
    else if (el > root->el) {
        root->right = search(el, root->right);
    }
    else {
        return root;
    }
}

Position deleteElement(int el, Position root) {
    if (root == NULL) {
        printf("Nema ga");
    }

    // find a node to delete
    else if (el < root->el) {
        root->left = deleteElement(el, root->left);
    }
    else if (el > root->el) {
        root->right = deleteElement(el, root->right);
    }
    //found

    //case with left and right child
    else if (root->left != NULL && root->right != NULL) {
        //find the minimum right or maximum left
        Position temp = root->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }
       
        root->el = temp->el;

        //delete
        root->right = deleteElement(root->el, root->right);
    }
    //case with one child
    else {
        Position temp = root;
        if (root->left == NULL) {
            root = root->right;
        }
        else {
            root = root->left;
        }
        free(temp);
    }

    return root;
}

int inorder(Position root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->el);
        inorder(root->right);
    }
    return 0;
}

int preorder(Position root) {
    if (root != NULL) {
        printf("%d ", root->el);
        preorder(root->left);
        preorder(root->right);
    }
    return 0;
}

int postorder(Position root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->el);
    }
    return 0;
}

int levelOrder(Position root) {
    if (root == NULL) {
        return 0;
    }

    Position element[20];
    int front = 0, rear = 0;

    element[rear++] = root;

    while (front < rear) {
        Position current = element[front++];

        printf("%d ", current->el);

        if (current->left != NULL) {
            element[rear++] = current->left;
        }

        if (current->right != NULL) {
            element[rear++] = current->right;
        }
    }
    return 0;
}