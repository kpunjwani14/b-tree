#include <iostream>
#include <fstream>
#include "ArgumentManager.h"  
#include <string>
#include <sstream>
using namespace std;

//I collaborated with mentors Shehzad and Chanta along with a few other students in the mentor room.
//Additionally, I referenced the class' textbook "Data Structures using C++ by DS Malik"

struct bTreeNode{
	int size;
	int* data;
	bTreeNode** children;

	bTreeNode(int numDegrees){
		size = 0;
		data = new int[numDegrees-1];
		children = new bTreeNode*[numDegrees];
		for (int i = 0; i<numDegrees; i++){
			children[i] = 0;
		}
	}
};

class bTree{
    private:
        bTreeNode* root;
        bTreeNode* right;
        bool isTaller;
        int degree;

    public:
		bTree();
        bool searchNode(bTreeNode* current, int item);
        void insert(int item);
        void insertBtree(bTreeNode* current, int& item);
        void insertNode(bTreeNode* current, int& item, int insertPosition);
        void splitNode(bTreeNode* current, int& item, int insertPosition);
        void inOrder();
        bTreeNode* getRoot();
        void inOrder(bTreeNode* current, ofstream& ofs);
        void printLevel(bTreeNode* cu, int level, bool &levelExists, int curLevel, ofstream& ofs);
        void printL(int levelNum, ofstream& ofs);
		void setDegree(int d);
};

bTree::bTree(){
	root = 0;
	right = 0;
	isTaller = 1;
}

void bTree::printL(int levelNum, ofstream& ofs){
    bool levelExists=0;
    printLevel(root,levelNum,levelExists, 1, ofs);
    if(!levelExists)
        ofs<<"empty";
	ofs<<endl;
}

void bTree::printLevel(bTreeNode* cu, int level, bool &levelExists, int curLevel, ofstream& ofs){
    if(cu==0)
        return;
    else if(curLevel==level){
        for(int x=0;x<cu->size;x++){
            ofs<<cu->data[x]<<" ";
        }
        levelExists=1;
    }
    else{
        for(int x=0;x<cu->size+1;x++){
            printLevel(cu->children[x],level,levelExists,curLevel+1, ofs);
        }
    }
}

bool bTree::searchNode(bTreeNode* current, int item){
	int location = 0;
	while (location < current->size && item > current->data[location])
       location++;
	if (location < current->size && item == current->data[location])
		return 1;
	else
		return 0;
}

void bTree::insert(int item){
	isTaller = 0;
	bTreeNode* rightChild;
	right = 0;
	insertBtree(root, item);

	if(isTaller){
		bTreeNode* tempRoot = new bTreeNode(degree);
		tempRoot->size = 1;
		tempRoot->data[0] = item;
		tempRoot->children[0] = root;
		tempRoot->children[1] = right;
		root = tempRoot;
	}
}

void bTree::insertBtree(bTreeNode* current, int& item){
	int i = 0;
	if(!current){
		right = 0;
		isTaller = 1;
	}
	else{
		if (!searchNode(current, item)){
			if (item < current->data[i])
				insertBtree(current->children[i], item);
			else if (item > current->data[current->size-1]){
				i = current->size;
				insertBtree(current->children[i], item);
			}
			else{
				for (i = 0; i < current->size - 1; i++){
					if (current->data[i]< item  && item < current->data[i + 1]){
						insertBtree(current->children[i+1], item);
						i++;
                        break;
                    }
				}
			}
			if (isTaller){
				if (current->size < degree-1){
					insertNode(current, item, i);
					isTaller = 0;
				}
				else
					splitNode(current, item, i);
			}
		}
	}
}

void bTree::insertNode(bTreeNode* current, int& item, int insertPosition){
	int index;
	for (index=current->size; index>insertPosition; index--){
		current->data[index] = current->data[index - 1];
		current->children[index + 1] = current->children[index];
	}
	current->data[index] = item;
	current->children[index + 1] = right;
	current->size++;
}

void bTree::splitNode(bTreeNode* current, int& item, int insertPosition){
	bTreeNode* rightNode = new bTreeNode(degree);
	int mid = (degree - 1) / 2;
	if (insertPosition <= mid){
		int index = 0;
		int i = mid;
		while (i < degree - 1){
			rightNode->data[index] = current->data[i];
			rightNode->children[index + 1] = current->children[i + 1];
			index++;
			i++;
		}
		current->size = mid;
		insertNode(current, item, insertPosition);
		(current->size)--;
		item = current->data[current->size];
		rightNode->size = index;
		rightNode->children[0] = current->children[current->size + 1];
	}
	else{
		int i = mid + 1;
		int index = 0;
		while (i < degree - 1){
			rightNode->data[index] = current->data[i];
			rightNode->children[index + 1] = current->children[i + 1];
			index++;
			i++;
		}
		current->size = mid;
		rightNode->size = index;
		insertNode(rightNode, item, insertPosition - mid - 1);
		item = current->data[mid];
		rightNode->children[0] = current->children[current->size + 1];

	}
	right = rightNode;
}

void bTree::inOrder(bTreeNode* current, ofstream& ofs)
{
	if (current != NULL){
		inOrder(current->children[0], ofs);
		for (int i=0; i<current->size; i++){
			ofs << current->data[i] << " ";
			inOrder(current->children[i + 1], ofs);
		}
	}
}

bTreeNode* bTree::getRoot(){
    return root;
}

void bTree::setDegree(int d){
	degree = d;
}


int main(int argc, char* argv[]){
    ArgumentManager am(argc, argv);
    const string in = am.get("input");
    const string out = am.get("output");
    const string com = am.get("command");
    ifstream i(in);
    ifstream c(com);
    ofstream o(out);

    string line = "";
    bTree t;
    string stringDegree;
    int degree;

    while (getline(c, line)){
        if (line != "\r"){
            if (line[6] == '='){
                stringDegree = line.substr(7, line.length() - 7);
                degree = stoi(stringDegree);
                t.setDegree(degree);
                string newLine = "";
                while (getline(i, newLine)){
                    if (newLine != ""){
                        stringstream newString(newLine);
                        int temp;
                        while (newString >> temp)
                            t.insert(temp); 
                    }
                }
            }
            else if (line[0] == 'I'){
                t.inOrder(t.getRoot(), o);
                o << endl;
            }
            else if (line[0] == 'L'){
                stringstream newLevel(line);
                int level;
                string temp;
                newLevel >> temp >> level;

                t.printL(level, o);
            }
        }
    }
    i.close();
    c.close();
    o.close();
}