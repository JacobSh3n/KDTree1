#include <iostream>    
#include <algorithm>    
#include <stack>    
#include <math.h>    
using namespace std;    
 
const int k=2;  

struct data    
{    
    double number[k];    
};    
    
struct Tnode    
{    
    struct data da;    
    int split;    
    struct Tnode * left;    
    struct Tnode * right;    
};
struct Tnode* newnode(double arr[])
{
    struct Tnode* temp = new Tnode;
    for(int i=0;i<k;i++)
       temp->da.number[i]=arr[i];
    temp->left=temp->right=NULL;
    temp->split=0;
    return temp;
}
void sorting(data set[], int size, int &split){
    data tmp;
    for (int i=0;i<size-1;i++)
    {
       for(int j=size-1;j>i;j--){
          if(set[j].number[split]<set[j-1].number[split])
          {
          tmp=set[j];
          set[j]=set[j-1];
          set[j-1]=tmp;
          }
        }
    }

} //sort the data set in an increasing order, consider the given dimension    
         
bool equal(data a, data b){    
    for(int i=0;i<k;i++){
        if(a.number[i]!=b.number[i])
        {
            return false;
        }
    }
    return true;
}    
    
void ChooseSplit(data set[], int size, int &split, data &SplitChoice){    

    double tmp1,tmp2;
    double v1,v2;   
    tmp1 = tmp2 = 0;
    v1 = v2 = 0;
    split=0; 
    for (int j=0;j<k;j++){    
    for (int i=0;i<size;i++)    
    {    
        tmp1 += 1.0 / (double)size * set[i].number[j] * set[i].number[j];    
        tmp2 += 1.0 / (double)size * set[i].number[j];    
    }    
    v1 = tmp1 - tmp2 * tmp2;  //compute variance on the each dimension        
    tmp1 = tmp2 = 0;
    if(v1>v2)
    {
    split=j;
    v2=v1;
    }
    
    }    //choose dimension with the largest variance        
    sorting(set,size, split);    

    //set the split point value
    for(int l=0;l<k;l++)
    {
         SplitChoice.number[l] = set[size / 2].number[l];
    }    
}       
Tnode* build_kdtree(data set[], int size, Tnode* T)
{    
    //call ChooseSplit to choose the dimension and split point    
    if (size == 0){    
        return NULL;    
    }    
    else{    
        int split;    
        data da;    
        ChooseSplit(set, size, split, da);    
        data set_right [100];    
        data set_left [100];    
        int sizeleft ,sizeright;    
        sizeleft = sizeright = 0;        
            for (int i = 0; i < size; ++i)    
            {    
                    
                if (!equal(set[i],da) && set[i].number[split] <= da.number[split])    
                {   for(int l=0;l<k;l++) 
                    {
                      set_left[sizeleft].number[l] = set[i].number[l]; 
                    } 
                    sizeleft++;    
                }    
                else if (!equal(set[i],da) && set[i].number[split]> da.number[split])    
                {   for(int l=0;l<k;l++) 
                    {
                      set_right[sizeright].number[l] = set[i].number[l]; 
                    }  
                    sizeright++;    
                }    
            }    
            
        
        T = new Tnode;
        for(int m=0;m<k;m++) {
          T->da.number[m] = da.number[m];     
        }    
        T->split = split;    
        T->left = build_kdtree(set_left, sizeleft, T->left);    
        T->right = build_kdtree(set_right, sizeright, T->right);    
        return T;    
            
    }  
}
//build KD tree
/////////////////////////////////////////////////////////////////////////////////////////////
//Search
bool arePointsSame(double point1[], double point2[])
{
    for (int i = 0; i < k; ++i)
        if (point1[i] != point2[i])
            return false;
  
    return true;
}
bool searchRec(Tnode* root, double point[])
{
    if (root == NULL)
        return false;
    if (arePointsSame(root->da.number, point))
        return true;
    int cd = root->split;
  
    if (point[cd] < root->da.number[cd])
        return searchRec(root->left, point);
  
    return searchRec(root->right, point);
} 
bool search(Tnode* root, double point[]){
    return searchRec(root, point);
}
//Search
/////////////////////////////////////////////////////////////////////////////////////////
//insert
Tnode *insertRec(Tnode* root, double point[])
{
    if (root == NULL)
    {
        return newnode(point);
    }
    
    int cd = root->split;

    if (point[cd] < root->da.number[cd])
        root->left = insertRec(root->left, point);
    else
        root->right= insertRec(root->right,point);
    return root;
} 
Tnode* insert(Tnode* root, double point[],data set[],int id){
    for(int i=0;i<k;i++){
        set[id].number[i]=point[i];
        }
    return insertRec(root, point);
}
int insertnew(Tnode* root, double point[],data set[],int id){
    insert(root,point,set,id);
    return id+1;
}
//insert
//////////////////////////////////////////////////////////////////////////////////////
//delete
Tnode *minNode(Tnode *t1, Tnode *t2, Tnode *t3, int d)
{
    Tnode *tmp = t1;
    if (t2 != NULL && t2->da.number[d] < tmp->da.number[d])
       tmp = t2;
    if (t3 != NULL && t3->da.number[d] < tmp->da.number[d])
       tmp = t3;
    return tmp;
}
Tnode *findMinRec(Tnode* root, int d)//find minimun consider one dimension
{

    if (root == NULL)
        return NULL;
  
    int cd = root->split;
  
    if (cd == d)
    {
        if (root->left == NULL)
            return root;
        return findMinRec(root->left, d);
    }

    return minNode(root,findMinRec(root->left, d),findMinRec(root->right, d), d);
}
Tnode *findMin(Tnode* root, int d)
{
    return findMinRec(root, d);
}//find minimum in d th dimension
void copyPoint(double p1[], double p2[])
{
   for (int i=0; i<k; i++)
       p1[i] = p2[i];
}
Tnode *deleteNodeRec(Tnode *root, double point[])
{
    // if point is not present
    if (root == NULL)
        return NULL;
  
    // Find dimension of current node
    int cd = root->split;
  
    // if the point  is present at root
    if (arePointsSame(root->da.number, point))
    {
        
        if (root->right != NULL)
        {
            // Find minimum of root's dimension in right subtree
            Tnode *min = findMin(root->right, cd);
  
            // copy the minimum to root
            copyPoint(root->da.number, min->da.number);
  
            // recursively delete the minimum
            root->right = deleteNodeRec(root->right, min->da.number);
        }
        else if (root->left != NULL) // same as above
        {
            Tnode *min = findMin(root->left, cd);
            copyPoint(root->da.number, min->da.number);
            root->right = deleteNodeRec(root->left, min->da.number);
        }
        else // if node to be deleted is leaf node
        {
            delete root;
            return NULL;
        }
        return root;
    }
    if (point[cd] < root->da.number[cd])
        root->left = deleteNodeRec(root->left, point);
    else
        root->right = deleteNodeRec(root->right, point);
    return root;
}
 Tnode* deleteNode(Tnode *root, double point[])//delete node
{
   return deleteNodeRec(root, point);
}
int deletedata(data set[],double point1[],int id){
    for(int i=0;i<id;i++){
        if(arePointsSame(set[i].number,point1))//find the set and delete the set
           {
               for(int j=i;j<id-1;j++){
               set[i]=set[i+1];
               }
             break;
           }
    }
    return id-1;//update id
}
//delete
////////////////////////////////////////////////////////////////////////////////////
//update
Tnode* update(data set[],int id,Tnode* root){
    root=NULL;
    return build_kdtree(set, id, root);
}
//update
       
int main(){    
    data set[100]; //build empty set   
    double x,y;  
    int id=0;  
    set[0].number[0]=3;
    set[0].number[1]=6;
    set[1].number[0]=17;
    set[1].number[1]=15;
    set[2].number[0]=13;
    set[2].number[1]=15;
    set[3].number[0]=6;
    set[3].number[1]=12;
    set[4].number[0]=9;
    set[4].number[1]=1;
    set[5].number[0]=2;
    set[5].number[1]=7; 
    set[6].number[0]=10;
    set[6].number[1]=19; //input set value
    id=7;    //total number of set
    
    //cout<<id;  
    struct Tnode * root = NULL;    
    root = build_kdtree(set, id, root); 

    double point1[] = {0, 6};//a new set 

    insertnew(root,point1,set,id);
    //test
    cout<< set[7].number[0]<<set[7].number[1]<<"\n";
    (search(root, point1))? cout << "Found\n": cout << "Not Found\n";

    root=deleteNode(root,point1);
    id=deletedata(set,point1,id);//delete

    root=update(set, id, root);//update
    
    //test
    (search(root, point1))? cout << "Found\n": cout << "Not Found\n";
    cout<< root->left->left->da.number[0]<<root->left->left->da.number[1]<<"\n";
    cout<< root->split<<"\n";  
    cout<<id;
}  