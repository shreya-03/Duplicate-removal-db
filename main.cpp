#include <bits/stdc++.h>
using namespace std;
map<unsigned int,string>  map_for_hashing;
class BTreeNode
{
    int *keys;  // An array of keys
    int t;      // Minimum degree (defines the range for number of keys)
    BTreeNode **C; // An array of child pointers
    int n;     // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
public:
    BTreeNode(int _t, bool _leaf);   // Constructor
    void insertNonFull(int k);
    void splitChild(int i, BTreeNode *y);
    void traverse();
    BTreeNode *search(int k);   // returns NULL if k is not present.
 
// Make BTree friend of this so that we can access private members of this
// class in BTree functions
friend class BTree;
};
class BTree
{
    BTreeNode *root; // Pointer to root node
    int t;  // Minimum degree
public:
    BTree(int _t)
    {  root = NULL;  t = _t; }
    void traverse()
    {  
    	if (root != NULL) 
    		root->traverse(); 
    }
    BTreeNode* search(int k)
    {  
    	return (root == NULL)? NULL : root->search(k); 
    }
    void insert(int k);
};
 
// Constructor for BTreeNode class
BTreeNode::BTreeNode(int t1, bool leaf1)
{
    // Copy the given minimum degree and leaf property
    t = t1;
    leaf = leaf1;
    // Allocate memory for maximum number of possible keys
    // and child pointers
    keys = new int[2*t-1];
    C = new BTreeNode *[2*t];
    // Initialize the number of keys as 0
    n = 0;
}
 
// Function to traverse all nodes in a subtree rooted with this node
void BTreeNode::traverse()
{
    // There are n keys and n+1 children, travers through n keys
    // and first n children
    int i;
    for (i = 0; i < n; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (leaf == false)
            C[i]->traverse();
        cout << " " << keys[i];
    }
    // Print the subtree rooted with last child
    if (leaf == false)
        C[i]->traverse();
}
 
// Function to search key k in subtree rooted with this node
BTreeNode *BTreeNode::search(int k)
{
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < n && k > keys[i])
        i++;
    // If the found key is equal to k, return this node
    if (keys[i] == k)
        return this;
    // If key is not found here and this is a leaf node
    if (leaf == true)
        return NULL; 
    // Go to the appropriate child
    return C[i]->search(k);
}
 
// The main function that inserts a new key in this B-Tree
void BTree::insert(int k)
{
    // If tree is empty
    if (root == NULL)
    {
        // Allocate memory for root
        root = new BTreeNode(t, true);
        root->keys[0] = k;  // Insert key
        root->n = 1;  // Update number of keys in root
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root->n == 2*t-1)
        {
            // Allocate memory for new root
            BTreeNode *s = new BTreeNode(t, false);
            // Make old root as child of new root
            s->C[0] = root;
            // Split the old root and move 1 key to the new root
            s->splitChild(0, root);
            // New root has two children now.  Decide which of the
            // two children is going to have new key
            int i = 0;
            if (s->keys[0] < k)
                i++;
            s->C[i]->insertNonFull(k);
            // Change root
            root = s;
        }
        else  // If root is not full, call insertNonFull for root
            root->insertNonFull(k);
    }
}
 
// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
void BTreeNode::insertNonFull(int k)
{
    // Initialize index as index of rightmost element
    int i = n-1;
    // If this is a leaf node
    if (leaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && keys[i] > k)
        {
            keys[i+1] = keys[i];
            i--;
        }
        // Insert the new key at found location
        keys[i+1] = k;
        n = n+1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && keys[i] > k)
            i--;
        // See if the found child is full
        if (C[i+1]->n == 2*t-1)
        {
            // If the child is full, then split it
            splitChild(i+1, C[i+1]);
            // After split, the middle key of C[i] goes up and
            // C[i] is splitted into two.  See which of the two
            // is going to have the new key
            if (keys[i+1] < k)
                i++;
        }
        C[i+1]->insertNonFull(k);
    }
}
 
// A utility function to split the child y of this node
// Note that y must be full when this function is called
void BTreeNode::splitChild(int i, BTreeNode *y)
{
    // Create a new node which is going to store (t-1) keys
    // of y
    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;
    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];
    // Copy the last t children of y to z
    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }
    // Reduce the number of keys in y
    y->n = t - 1;
    // Since this node is going to have a new child,
    // create space of new child
    for (int j = n; j >= i+1; j--)
        C[j+1] = C[j];
    // Link the new child to this node
    C[i+1] = z;
    // A key of y will move to this node. Find location of
    // new key and move all greater keys one space ahead
    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];
    // Copy the middle key of y to this node
    keys[i] = y->keys[t-1];
    // Increment count of keys in this node
    n = n + 1;
}
unsigned long create_hash_value(string temp_str)
{
	unsigned long hash_val = 5381;
	for(int i=0;i<temp_str.size();i++)
	{
		if(temp_str[i]!=',')
			hash_val=((hash_val << 5) + hash_val) + temp_str[i];
	}
	return hash_val;
}
void file_close(ifstream& ipfilename,ofstream& opfilename)
{
//	cout<<"entered close"<<endl;
	ipfilename.close();
	opfilename.close();
}
void Getnext_hash(ifstream& inputfile,ofstream& outputfile,int no_of_buffer,int no_of_attr,int buffer_size)
{
	vector<string> ipbuffer;
	vector<unsigned int> opbuffer;
	map<unsigned int,string>::iterator m;
	long int records_ip_buffer=((no_of_buffer-1)*buffer_size)/(no_of_attr*4);
	long int records_op_buffer=buffer_size/(no_of_attr*4);
	long int cnt_ip_buffer=0;
	long int cnt_op_buffer=0;
	string lines;
	unsigned int hash_val;
	vector<string>::iterator ip_it;
	vector<unsigned int>::iterator op_it;
	while(getline(inputfile,lines))
	{
		if(cnt_ip_buffer<records_ip_buffer-1)
		{
			ipbuffer.push_back(lines);
			cnt_ip_buffer++;
		}
		else
		{
			ipbuffer.push_back(lines);
			cnt_ip_buffer++;
			for(ip_it=ipbuffer.begin();ip_it!=ipbuffer.end();++ip_it)
			{
				hash_val=create_hash_value(*ip_it);
//				cout<<hash_val<<endl;
				m=map_for_hashing.find(hash_val);
				if(m==map_for_hashing.end())
				{
//					cout<<"entered"<<endl;
					opbuffer.push_back(hash_val);
//					cout<<*op_it<<endl;
					cnt_op_buffer++;
//					cout<<"before hashing"<<endl;
					map_for_hashing.insert(pair<unsigned int,string>(hash_val,*ip_it));
//					cout<<"after hashing"<<endl;
				}
				if(cnt_op_buffer>records_op_buffer)
				{
//					cout<<"entered 3rd case"<<endl;
					for(op_it=opbuffer.begin();op_it!=opbuffer.end();++op_it)
						outputfile<<map_for_hashing[*op_it]<<endl;
					opbuffer.clear();
					cnt_op_buffer=0;
				}
			}
			cnt_ip_buffer=0;
			ipbuffer.clear();
		}
	}
//	cout<<cnt_ip_buffer<<endl;
	if(cnt_op_buffer>0)
	{
		for(op_it=opbuffer.begin();op_it!=opbuffer.end();op_it++)
			outputfile<<map_for_hashing[*op_it]<<endl;
	}
//	cout<<map_for_hashing.size()<<endl;
//	for(m=map_for_hashing.begin();m!=map_for_hashing.end();m++)
//		cout<<m->first<<":"<<m->second<<endl;
//	for(op_it = opbuffer.begin(); op_it != opbuffer.end(); op_it++) {
// 		cout << *op_it <<endl;
//  	}
//	cout<<count<<endl;
	file_close(inputfile,outputfile);
	return;
}

void Getnext_btree(ifstream& inputfile,ofstream& outputfile,int no_of_buffer,int no_of_attr,int buffer_size )
{
//	cout<<"entered btree function"<<endl;
	vector<string> ipbuffer;
	vector<unsigned int> opbuffer;
	map<unsigned int,string>::iterator m;
	long int records_ip_buffer=((no_of_buffer-1)*buffer_size)/(no_of_attr*4);
	long int records_op_buffer=buffer_size/(no_of_attr*4);
	long int cnt_ip_buffer=0;
	long int cnt_op_buffer=0;
	string lines;
	unsigned int hash_val;
	vector<string>::iterator ip_it;
	vector<unsigned int>::iterator op_it;
//	long int count=0;
//	close(inputfile,outputfile);
//	cout<<"entered hash function"<<endl;
	int n;
    n=floor((buffer_size*1000000-8)/12);
	BTree t(n);
	while(getline(inputfile,lines))
	{
//		hash_val=create_hash_value(lines);
		if(cnt_ip_buffer<records_ip_buffer-1)
		{
//			cout<<"enetered"<<endl;
			ipbuffer.push_back(lines);
			cnt_ip_buffer++;
		}
		else
		{
			ipbuffer.push_back(lines);
			cnt_ip_buffer++;
			for(ip_it=ipbuffer.begin();ip_it!=ipbuffer.end();ip_it++)
			{
				hash_val=create_hash_value(*ip_it);
				if(t.search(hash_val)==NULL)
				{
//					cout<<"entered"<<endl;
					opbuffer.push_back(hash_val);
//					cout<<*op_it<<endl;
					cnt_op_buffer++;
					t.insert(hash_val);
//					cout<<"before hashing"<<endl;
					map_for_hashing.insert(pair<unsigned int,string>(hash_val,*ip_it));
//					cout<<"after hashing"<<endl;
				}
//				cout<<cnt_op_buffer<<endl;
				if(cnt_op_buffer>=records_op_buffer)
				{
//					cout<<"entered 3rd case"<<endl;
					for(op_it=opbuffer.begin();op_it!=opbuffer.end();op_it++)
						outputfile<<map_for_hashing[*op_it]<<endl;
					opbuffer.clear();
					cnt_op_buffer=0;
				}
			}
			cnt_ip_buffer=0;
			ipbuffer.clear();
		}
//	for(op_it = opbuffer.begin(); op_it != opbuffer.end(); op_it++) {
// 		cout << *op_it <<endl;
//  	}
	}
	if(cnt_op_buffer>0)
	{
		for(op_it=opbuffer.begin();op_it!=opbuffer.end();op_it++)
			outputfile<<map_for_hashing[*op_it]<<endl;
	}
	file_close(inputfile,outputfile);
	return;
}
void file_open(string ipfilename,string opfilename,int no_of_attr,int no_of_buffer,int buffer_size,int index_type)
{
//	cout<<"entered file func"<<endl;
	ifstream inputfile;
	inputfile.open(ipfilename.c_str());
	ofstream outputfile ;
	outputfile.open(opfilename.c_str());
	if(index_type==1)	
		Getnext_hash(inputfile,outputfile,no_of_buffer,no_of_attr,buffer_size);
	else
		Getnext_btree(inputfile,outputfile,no_of_buffer,no_of_attr,buffer_size);
	return;
}
int main(int argc,char * argv[])
{
	if(argc<4)
	{
		exit(0);
	}
	else
	{
		string inputfile=argv[2];
		int number_of_attr=10;
		int number_of_buffer=atoi(argv[1]);
		char *token=strtok(argv[3],"M");
		int buffer_size=atoi(token);
		int index_type=1;
		string outputfile="output.txt";
		string arguments;
		if(index_type==1)
		{
			//Hash
			file_open(inputfile,outputfile,number_of_attr,number_of_buffer,buffer_size,index_type);

		}
		else
		{
			//Btree
			file_open(inputfile,outputfile,number_of_attr,number_of_buffer,buffer_size,index_type);
		}
	}
	return 0;
}