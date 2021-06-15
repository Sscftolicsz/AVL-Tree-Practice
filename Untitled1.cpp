#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct Node{
	char productName[100];
	char productQuality[15];
	int productPrice;
	int productStock;
	int height;
	Node* left;
	Node* right;
};

Node* createNode(char productName[100], char productQuality[15], int productPrice, int productStock){
	Node* newNode = (Node*)malloc(sizeof(Node));
	strcpy(newNode->productName,productName);
	strcpy(newNode->productQuality,productQuality);
	newNode->productPrice = productPrice;
	newNode->productStock = productStock;
	newNode->height = 1;
	newNode->left = newNode->right = NULL;
	return newNode;
}

void clear(){
	for(int i=0;i<26;i++){
		printf("\n");
	}
}

int max(int a, int b){
	// ternary
	// if (a > b) return a
	// else return b
	return a > b ? a : b;
}

int getHeight(Node* root){
	if(!root) return 0;
	return root->height;
	// 1. root nya ada -> langsung return root->height
	// 2. root nya NULL -> jangan return root->height, tapi 0;
}

int getBalanceFactor(Node* root){
	// height anak kiri - height anak kanan 
	return getHeight(root->left) - getHeight(root->right);
}

void updateHeight(Node* root){
	// height = max(height anak kiri, height anak kanan) + 1 -> diri sendiri
	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;		
}

Node* rotateRight(Node* root){
	Node* child = root->left;
	Node* gchild = child->right;
	
	child->right = root;
	root->left = gchild;
	
	updateHeight(root);
	updateHeight(child);
	
	return child;
}

Node* rotateLeft(Node* root){
	Node* child = root->right;
	Node* gchild = child->left;
	
	child->left = root;
	root->right = gchild;
	
	updateHeight(root);
	updateHeight(child);
	
	return child;
}

Node* rebalance(Node* root){
	// balance factor
	int bf = getBalanceFactor(root);
	
	if(bf < -1){
		// kanan lebih berat
		if(getBalanceFactor(root->right) > 0){
			// root -> kanan -> kiri 
			// rotate right si childnya
			root->right = rotateRight(root->right);
		}
		// rotate left si rootnya
		root = rotateLeft(root);
	}else if(bf > 1){
		// kiri lebih berat
		if(getBalanceFactor(root->left) < 0){
			// root -> kiri -> kanan
			// rotate left si childnya
			root->left = rotateLeft(root->left);
		}
		// rotate right si rootnya
		root = rotateRight(root);
	}
	return root;
}

Node* push(Node* root, char productName[100], char productQuality[15], int productPrice, int productStock){
	// ada root / tidak ?
	if(!root) return createNode(productName,productQuality,productPrice,productStock);
	
	if(strcmp(productName, root->productName) < 0){
		// kiri
		root->left = push(root->left, productName, productQuality, productPrice, productStock);
	}else if(strcmp(productName, root->productName) > 0){
		// kanan
		root->right = push(root->right, productName,productQuality,productPrice,productStock);
	}
	
	// update height
	updateHeight(root);
	
	// return root yang sudah di rebalance
	return rebalance(root);
}

Node* searchProduct(Node* root, char name[100]){
	if(!root) return NULL;
	
	if(strcmp(name, root->productName) < 0) // name < root
		return searchProduct(root->left, name);
	else if(strcmp(name, root->productName) > 0) // name > root
		return searchProduct(root->right, name);
	else return root;
}

bool nameIsValid(Node* root, char name[100]){
	// harus lebih dari 2 karakter
	if(strlen(name) < 2){
		printf("PRODUCT NAME MUST CONSIST OF 2 CHARACTERS OR MORE!\n\n");
		return false;
	}
	// tidak boleh ada spasi
	for(int i=0;i<strlen(name);i++){
		if(isspace(name[i])){
			printf("PRODUCT NAME CAN NOT CONSIST OF WHITE SPACE\n\n");
			return false;	
		} 
	}
	
	// tidak boleh sama (unique)
	if(searchProduct(root, name)) return false;
	
	return true;
}

Node* insertProduct(Node* root){
	char productName[100];
	char productQuality[15];
	int productPrice;
	int productStock;
	char confirmation;
	
	do{
		printf("Input Product's name [2 chars or more] : ");
		scanf("%[^\n]",productName);
		getchar();
	}while(!nameIsValid(root, productName));
	printf("\n");
	do{
		printf("Input Product's quality [ Super | Good | Ok ] : ");
		scanf("%[^\n]",productQuality);
		getchar();
		if(strcmpi(productQuality, "super") != 0 && strcmpi(productQuality, "good") != 0 && strcmpi(productQuality, "ok") != 0){
			printf("PRODUCT'S QUALITY CAN ONLY BE Super / Good / Ok\n\n");
		}
	}while(strcmpi(productQuality, "super") != 0 && strcmpi(productQuality, "good") != 0 && strcmpi(productQuality, "ok") != 0);
	printf("\n");
	do{
		printf("Insert product's price [Divisible by 100] : ");
		scanf("%d",&productPrice);
		getchar();
		if(productPrice%100 != 0){
			printf("PRODUCT'S PRICE MUST BE DIVISIBLE BY 100!\n\n");
		}
	}while(productPrice%100 != 0);
	printf("\n");
	do{
		printf("Insert product's stock [1-100] : ");
		scanf("%d",&productStock);
		getchar();
		if(productStock < 1 || productStock > 100){
			printf("PRODUCT'S MUST BE BETWEEN 1 AND 100!\n\n");
		}
	}while(productStock < 1 || productStock > 100);
	printf("\n");
	do{
		printf("ARE YOU SURE YOU WANT TO ADD %s PRODUCT? [Y|N] > ",productName);
		scanf("%c",&confirmation);
		getchar();
	}while(confirmation != 'Y' && confirmation != 'N');
	if(confirmation == 'Y'){
		printf("SUCCESS ADD PRODUCT!\n");
		root = push(root, productName, productQuality, productPrice, productStock);
	}
	return root;
}

void inorder(Node* root){
	if(!root) return;
	inorder(root->left);
	printf("\n");
	printf("========================\n");
	printf("Product's Name  : %s\n",root->productName);
	printf("Product's Quality : %s\n",root->productQuality);
	printf("Product's Price : IDR %d\n",root->productPrice);
	printf("Product's Stock : %d pcs\n",root->productStock);
	printf("========================\n");
	printf("\n");
	inorder(root->right);
}

void preorder(Node* root){
	if(!root) return;
	printf("\n");
	printf("========================\n");
	printf("Product's Name  : %s\n",root->productName);
	printf("Product's Quality : %s\n",root->productQuality);
	printf("Product's Price : IDR %d\n",root->productPrice);
	printf("Product's Stock : %d pcs\n",root->productStock);
	printf("========================\n");
	printf("\n");
	preorder(root->left);
	preorder(root->right);
}

void postorder(Node* root){
	if(!root) return;
	postorder(root->left);
	postorder(root->right);
	printf("\n");
	printf("========================\n");
	printf("Product's Name  : %s\n",root->productName);
	printf("Product's Quality : %s\n",root->productQuality);
	printf("Product's Price : IDR %d\n",root->productPrice);
	printf("Product's Stock : %d pcs\n",root->productStock);
	printf("========================\n");
	printf("\n");
}

void seeAllProduct(Node* root){
	if(!root){
		puts("YOU DON'T HAVE ANY PRODUCT");
		getchar();
		return;
	}
	int choose = 0;
	do{
		puts("How do you want to see all product?");
		puts("1. In order");
		puts("2. Pre order");
		puts("3. Post order");
		printf(">> ");
		scanf("%d",&choose);
		getchar();
		switch(choose){
			case 1:
				inorder(root);
				break;
			case 2:
				preorder(root);
				break;
			case 3:
				postorder(root);
				break;
		}
		getchar();
	}while(choose < 1 || choose > 3);
}

Node* findPredecessor(Node* root){
	root = root->left; // ke kiri 1x
	while(root->right){ // ke kanan sampe mentok
		root = root->right;
	}
	return root;
}

Node* pop(Node* root, char productName[100]){
	if(!root) return NULL;
	
	if(strcmp(productName, root->productName) < 0){
		root->left = pop(root->left, productName);
	}else if(strcmp(productName, root->productName) > 0){
		root->right = pop(root->right, productName);
	}else{
		// ketemu
		if(root->left && root->right){
			root = NULL;
			free(root);
			return NULL;
		}else if(!root->left){
			// anak kanan
			Node* child = root->right;
			root = NULL;
			free(root);
			return child;
		}else if(!root->right){
			// anak kiri
			Node* child = root->left;
			root = NULL;
			free(root);
			return child;
		}else{
			// 2 anak
			Node* pre = findPredecessor(root);
			strcpy(root->productName, pre->productName);
			strcpy(root->productQuality, pre->productQuality);
			root->productPrice = pre->productPrice;
			root->productStock = pre->productStock;
			
			root->left = pop(root->left, pre->productName);
		}
	}
	updateHeight(root);
	
	return rebalance(root);
}

Node* removeProduct(Node* root){
	if(!root){
		puts("YOU DON'T HAVE ANY PRODUCT");
		return NULL;
	}
	char productName[100];
	Node* found = NULL;
	
	seeAllProduct(root);
	
	do{
		printf("ENTER PRODUCT NAME TO BE REMOVED! > ");
		scanf("%[^\n]",productName);
		found = searchProduct(root, productName);
		if(!found){
			printf("PRODUCT DOES NOT EXIST!\n");
			getchar();
		}else{
			printf("SUCCESS REMOVE PRODUCT");
		getchar();
		pop(root, productName);
		}
	}while(!found);
}

Node* removeAllProduct(Node* root){
	if(!root){
		puts("YOU DON'T HAVE ANY PRODUCT");
		return NULL;
	}
	char confirmation;
	do{
		printf("ARE YOU SURE WANT TO REMOVE ALL PRODUCT? [Y|N] > ");
		scanf("%c",&confirmation);
		getchar();
	}while(confirmation != 'Y' && confirmation != 'N');
	if(confirmation == 'Y'){
		while(root){
			root = pop(root, root->productName);
			printf("SUCCESS REMOVE ALL PRODUCT!\n");
		}
	}
	return root;
}

int main(){
	int choose = 0;
	Node* root = 0;
	do{ 
		clear();
		puts("My Product\n");
		puts("1. Insert New Product");
		puts("2. Remove a Product");
		puts("3. See all product");
		puts("4. Remove all product");
		puts("5. Exit");
		printf(">> ");
		scanf("%d",&choose);
		getchar();
		
		switch(choose){
			case 1 :
				root = insertProduct(root);
				getchar();
				break;
			case 2 :
				root = removeProduct(root);
				getchar();
				break;
			case 3 :
				seeAllProduct(root);
				break;
			case 4 :
				root = removeAllProduct(root);
				getchar();
				break;
		}
	}while(choose != 5);
	return 0;
}
