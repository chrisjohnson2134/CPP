#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "lines.cu"
#include <string.h>
#include <stdio.h>

/*
 * C / CUDA Implementation BST.
 * Modified to discard any duplicate value.
*/


struct pair{
    int a;
    int b;

    __host__ __device__ 
    friend bool operator<= (const pair& l,const pair& r)
    {return (l.a + l.b) <= (r.a + r.b);}

    __host__ __device__ 
    friend bool operator>= (const pair& l,const pair& r)
    {return (l.a + l.b) >= (r.a + r.b);}

    __host__ __device__ 
    friend bool operator !=(const pair& l,const pair& r)
    {return !((l.a == r.a) && (l.b == r.b));}

};


struct node 
{ 
    pair key; 
    struct node *left, *right; 
}; 

__host__ __device__
pair makePair(int l1,int l2)
{
  return {l1,l2};
};

__host__ __device__
struct node *newNode(pair pass) 
{ 
    struct node *temp =  (struct node *)malloc(sizeof(struct node)); 
    temp->key = pass;
    temp->left = temp->right = NULL; 
    return temp; 
} 


   
__host__ __device__ 
void inorder(struct node *root) 
{ 
    if (root != NULL) 
    { 
        inorder(root->left); 
        printf("%i %i\n", root->key.a,root->key.b); 
        inorder(root->right); 
    } 
} 
   
__host__ __device__ 
struct node* insert(struct node* node, pair key) 
{ 
    if (node == NULL) return newNode(key); 
    
    if(key <= node->key && node->key != key)
        node->left  = insert(node->left, key); 
    else if(key >= node->key && node->key != key)
        node->right = insert(node->right, key);    
        
    return node; 
} 

// Driver code 
__global__
void mainDevice() 
{ 
	struct node *root = NULL; 
    root = insert(root, makePair(1,1)); 
    insert(root, makePair(1,2)); 
    insert(root, makePair(1,2)); 
    insert(root, makePair(1,3)); 
    insert(root, makePair(2,1)); 
    insert(root, makePair(2,2)); 
    insert(root, makePair(2,2)); 
    insert(root, makePair(2,3)); 
    insert(root, makePair(3,1)); 
    insert(root, makePair(3,2)); 
   
    // print inoder traversal of the BST 
    inorder(root); 
} 

__host__
int main()
{
     mainDevice<<<1,1>>>();

     cudaDeviceSynchronize();
    return 0;
}



// This code is contributed by pkthapa 
