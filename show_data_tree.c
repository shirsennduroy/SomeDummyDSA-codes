#include <stdio.h>
#include <stdlib.h>

#define MAX 128 // Make it always 2's integer exponent value such as 2^(1,2,3,...., n integer)

typedef struct tree
{
    int info;
    struct tree *left_child;
    struct tree *right_child;
} NODE;

NODE *Root;

NODE *stack[MAX];
static int Top = -1;
NODE dummy = {0, NULL, NULL};
int IsEmpty()
{
    if (Top < 0)
        return 1;

    return 0;
}

int IsFull()
{
    if (Top >= MAX - 1)
        return 1;
    return 0;
}

void Stack_Push(NODE *node)
{
    if (IsFull())
    {
        printf("Stack OverFlow!\n");
        return;
    }

    if (Top >= 0)
    {
        if (stack[Top] == node)
            return;
    }
    stack[++Top] = node;
}

void stack_pop()
{
    if (IsEmpty())
    {
        printf("Stack is empty!\n");
        return;
    }
    Top--;
}

NODE *stack_top()
{
    if (IsEmpty())
    {
        printf("Stack is empty!\n");

        return &dummy;
    }
    return stack[Top];
}

void free_all_allocation(NODE *root)
{
    if (root != NULL)
    {
        free_all_allocation(root->left_child);
        free_all_allocation(root->right_child);
        free(root);
    }
    else
    {
        return;
    }
}

NODE *insert(int value, NODE *tree_ptr)
{
    if (tree_ptr == NULL)
    {
        tree_ptr = malloc(sizeof(NODE));

        if (tree_ptr == NULL)
        {
            fprintf(stderr, "allocation failed!\n");
            free_all_allocation(Root);
            exit(EXIT_FAILURE);
        }

        tree_ptr->info = value;
        tree_ptr->left_child = tree_ptr->right_child = NULL;
    }
    else if (tree_ptr->info >= value)
    {
        tree_ptr->left_child = insert(value, tree_ptr->left_child);
    }
    else
    {
        tree_ptr->right_child = insert(value, tree_ptr->right_child);
    }

    return tree_ptr;
}

void show_tree_recursively(int hight, NODE *node)
{
    if (node == NULL)
    {
        return;
    }

    show_tree_recursively(hight + 1, node->right_child);
    printf("\n  ");
    for (int i = 1; i < hight; i++)
    {
        printf("\033[33m-----\033[37m");
    }
    printf("\033[37m\033[31m%i\033[37m\n", node->info);
    show_tree_recursively(hight + 1, node->left_child);
}

void search_node_value(int value, NODE *root)
{
    if (root->info == value)
    {
        printf("\033[34m%i found at tree!\033[37m\n", value);
        return;
    }

    while (root->info != value)
    {
        if (!root)
        {
            printf("\033[31mNo %d value found! YET!\033[37m\n", value);
            return;
        }
        else if (root->info == value)
        {
            printf("\033[34m%i found at tree!\033[37m\n", value);
            return;
        }
        else if (root->info > value)
            root = root->left_child;
        else
            root = root->right_child;
    }
}

NODE *delete_node(NODE *root, int value)
{
    if (root == NULL)
    {
        printf("\n\tValue not found in tree!\n");
        return NULL;
    }

    if (value < root->info)
    {
        root->left_child = delete_node(root->left_child, value);
    }
    else if (value > root->info)
    {
        root->right_child = delete_node(root->right_child, value);
    }
    else
    {
        // Node to be deleted found
        if (root->left_child == NULL)
        {
            NODE *temp = root->right_child;
            free(root);
            return temp;
        }
        else if (root->right_child == NULL)
        {
            NODE *temp = root->left_child;
            free(root);
            return temp;
        }

        // Node with two children
        // Get the inorder successor (smallest in the right subtree)
        NODE *successor = root->right_child;
        while (successor->left_child != NULL)
        {
            successor = successor->left_child;
        }

        // Copy the successor's value
        root->info = successor->info;

        // Delete the inorder successor
        root->right_child = delete_node(root->right_child, successor->info);
    }

    return root;
}

void PreOrder(NODE *root)
{
    while (root)
    {
        printf("%d ", root->info);

        if (root->left_child)
        {
            Stack_Push(root);
            root = root->left_child;
        }
        else if (root->right_child)
            root = root->right_child;
        else if (!IsEmpty())
        {
            while (!root->right_child)
            {
                root = stack_top();
                stack_pop();
                if (IsEmpty())
                    break;
            }
            root = root->right_child;
        }
        else
        {
            printf("  PreOrder_Done\n");
            root = root->right_child;
        }
    }
}

void PostOrder(NODE *root)
{
    if (!root)
        return;
    PostOrder(root->left_child);
    PostOrder(root->right_child);
    printf("\033[34m %d \033[37m", root->info);
}

void PostOrder_Itaretive_Way(NODE *root) // Without Recursion and inspired of Recursion!
{
    if (!root)
        return;
    Top = -1;
    NODE *Outted = NULL;
    int NeedToGoDown = 1;

    Stack_Push(root);

    while (!IsEmpty())
    {
        if (root->right_child && root->left_child)
        {
            if (root->right_child == Outted)
            {
                printf("\033[34m %d \033[37m", root->info);
                Outted = root;
                root = stack_top();
                stack_pop();
                if (IsEmpty())
                {
                    printf("\033[34m %d \033[37m", root->info);
                    break;
                }
                continue;
            }

            Stack_Push(root);
            Stack_Push(root->right_child);
            root = root->left_child;
            Outted = NULL;
        }
        else if (root->left_child)
        {
            if (root->left_child == Outted)
            {
                printf("\033[34m %d \033[37m", root->info);
                Outted = root;
                root = stack_top();
                stack_pop();
                if (IsEmpty())
                {
                    printf("\033[34m %d \033[37m", root->info);
                    break;
                }
                continue;
            }

            Stack_Push(root);
            root = root->left_child;
            Outted = NULL;
        }
        else if (root->right_child)
        {
            if (root->right_child == Outted)
            {
                printf("\033[34m %d \033[37m", root->info);
                Outted = root;
                root = stack_top();
                stack_pop();
                if (IsEmpty())
                {
                    printf("\033[34m %d \033[37m", root->info);
                    break;
                }
                continue;
            }

            Stack_Push(root);
            root = root->right_child;
            Outted = NULL;
            if (IsEmpty())
            {
                printf("\033[34m %d \033[37m", root->info);
                break;
            }
        }
        else
        {
            Outted = root;
            printf("\033[34m %d \033[37m", root->info);
            root = stack_top();
            stack_pop();
            if (IsEmpty())
            {
                printf("\033[34m %d \033[37m", root->info);
                break;
            }
        }
    }
}

NODE *Mirror_Tree(NODE *root) // recursive way
{
    if (root)
    {
        NODE *left = root->left_child, *right = root->right_child;
        if (left || right)
        {
            root->left_child = Mirror_Tree(right);
            root->right_child = Mirror_Tree(left);
        }
    }
    return root;
}

int Calculate_height(NODE *node)
{
    if (node)
    {
        int left_height = Calculate_height(node->left_child);
        int right_height = Calculate_height(node->right_child);
        return 1 + (left_height > right_height ? left_height : right_height);
    }
    return 0;
}

NODE *AVL(NODE *node)
{
    if (node)
    {
        int factor = Calculate_height(node->left_child) - Calculate_height(node->right_child);
        NODE *current = node, *NewRoot, *RightofNewRoot;

        if (factor > 1)
        {
            while ((Calculate_height(current->left_child) - Calculate_height(current->right_child)) > 1)
            {
                NewRoot = current->left_child;
                RightofNewRoot = NewRoot->right_child;
                current->left_child = RightofNewRoot;
                NewRoot->right_child = current;
                current = NewRoot;
            }
        }
        else if (factor < -1)
        {
            while ((Calculate_height(current->left_child) - Calculate_height(current->right_child)) < -1)
            {
                NewRoot = current->right_child;
                RightofNewRoot = NewRoot->left_child;
                current->right_child = RightofNewRoot;
                NewRoot->left_child = current;
                current = NewRoot;
            }
        }

        current->right_child = AVL(current->right_child);
        current->left_child = AVL(current->left_child);
        return current;
    }
    return NULL;
}

void Inorder(NODE *root)
{
    if (!root)
        return;

    Top = -1;
    int ShouldVisitLeft = 1;

    while (root)
    {
        if (root->left_child && ShouldVisitLeft)
        {
            Stack_Push(root);
            root = root->left_child;
        }
        else if (root->right_child)
        {
            printf("\033[34m %d \033[37m", root->info);
            root = root->right_child;
            ShouldVisitLeft = 1;
        }
        else if (!IsEmpty())
        {
            printf("\033[34m %d \033[37m", root->info);
            root = stack_top();
            stack_pop();
            ShouldVisitLeft = 0;
        }
        else
        {
            printf("\033[34m %d \033[37m", root->info);
            printf("   InOrder_Done!\n");
            root = root->right_child;
        }
    }
}

int main(int argc, char **argv)
{

    int value;

    while (1)
    {
        printf("\nHey let print the tree \n If want to terminarte press 1000 input those value.\n\n\n");
        scanf("%d", &value);
        if (value == 1000)
        {
            break;
        }
        Root = insert(value, Root);

        show_tree_recursively(1, Root);
        // printf("\n\nFind the value in tree\n");
        // scanf("%i", &value);

        // search_node_value(value, Root);
    }
    while (1)
    {
        printf("Do you want to delete node ? If yes put the value or 1000 for break\n");
        scanf("%d", &value);
        if (value == 1000)
            break;
        Root = delete_node(Root, value);
        show_tree_recursively(2, Root);
    }

    PreOrder(Root);
    printf("\n");
    Inorder(Root);
    printf("\n");
    PostOrder(Root);
    printf("  PostOrder_Done!\n");
    PostOrder_Itaretive_Way(Root);
    printf("Done Intaretive Way!\n\n");

    printf("\n     %d\n", Calculate_height(Root));
    Root = AVL(Root);
    printf("AVL! Tree!\nHeight is %d\n", Calculate_height(Root));
    show_tree_recursively(2, Root);
    Root = Mirror_Tree(Root);
    printf("MIrror!\n");
    show_tree_recursively(2, Root);
    printf("\n");

    free_all_allocation(Root);
    return 0;
}
