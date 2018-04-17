//scarble helper 
//input 7 letters
//* - bimbo
//@denied6 Denis Redko


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
//#include <vld.h>
using namespace std;
int N = 7;
const int LETTERS = 31;
const char WHOLELETTERS[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','-','\'','?','*'};
//########################################################################################//
//------------------------------------STRUCT TRIE-----------------------------------------//
struct node {//prefics trie
	struct node *child[LETTERS];//26 letterrs + symb(4)[31 ptr to next node]
	bool leaf;
	string word;
	string about;
};
struct node *newnode()//new node for trie
{
	struct node *ptr = new struct node;//ptr to next node
	ptr->about = "";
	ptr->word = "";
	ptr->leaf = false;
	for (int i = 0; i < LETTERS; i++) {
		ptr->child[i] = NULL;
	}
	return ptr;
}
//-------------------------STRUCT WORD----------------------------------------------------//
struct strWord {//final word[word  about  point]
	string Word;
	string about;
	int point;
} word;
//########################################################################################//



//########################################################################################//
char * deleteElement(char *letters, char c);//delete char from array *char
int GetIndex(char c);//index of letter in arrey
int getPointWord(string Word);//points for word
bool inarrey(char* letters, char h);//находитьс€ ли буква в масиве
//-------------------------struct Word----------------------------------------------------//
strWord* addStrWord(const int amount, strWord word[]);//добавить слово в структуру 
void setStrWord(strWord Obj[], const int amount, string word, string about);
void showStrWord(strWord Obj[], int amount);
void showStrPoint(strWord Obj[], int amount);
//--------------------------SORT----------------------------------------------------------//
void sortByFirstLetterDW(strWord Obj[], int amount);//сортирока от a-z
void sortByFirstLetterUP(strWord Obj[], int amount);//сортирока от z-a
void sortByPointUP(strWord Obj[], const int amount);
void sortByPointDW(strWord Obj[], const int amount);
void showArreyChar(char* arrey);
//-------------------------struct Trie----------------------------------------------------//
void input(struct node *root, string word, string about);//input word from lib to trie
void search(struct node *node, char *letters, int &K, strWord*  &word);//search words
void deleteTrie(struct node *node);
void openFiles(char *plyerLetters, struct node *root);//open files
//########################################################################################//




int main() {
	struct node *root = newnode();
	char *plyerLetters = new char[N + 1];//
	char * ptr = new char[26];

//----------------------------------------------------------
	for (int i = 0; i < N; i++) {
			cin >> plyerLetters[i];
	}
//----------------------------------------------------------
	plyerLetters[N] = '\0';
	struct strWord* ptrword = &word;//Pointer to struct word
	openFiles(plyerLetters, root);
	system("cls");
	int k = 0;
	showArreyChar(plyerLetters);
//----------------------------------------------------------
	search(root, plyerLetters, k, ptrword);
	sortByPointUP(ptrword, k);
	showStrPoint(ptrword, k);
//----------------------------------------------------------
//-------------------USER MENU-----------------------------
	int ind;
	int indAddChar;
	do {
		do {
			
			cout << "\n[1] sort By First Letter DW \n";
			cout << "[2] sort By First Letter UP \n";
			cout << "[3] sort By Point DW \n";
			cout << "[4] sort By Point UP \n";
			cout << "[0] Exit \n";
			cin >> ind;
			switch (ind)
			{
			case 1: {
				sortByFirstLetterDW(ptrword, k);
				system("cls");
				showStrWord(ptrword, k);
			}break;
			case 2: {
				sortByFirstLetterUP(ptrword, k);
				system("cls");
				showStrWord(ptrword, k);
			}break;
			case 3: {
				sortByPointDW(ptrword, k);
				system("cls");
				showStrPoint(ptrword, k);
			}break;
			case 4: {
				sortByPointUP(ptrword, k);
				system("cls");
				showStrPoint(ptrword, k);
			}break;
			case 0: {
			}break;
			default:
				system("cls");
				cout << "WRONG!";
				break;
			}
		} while (ind);
//----------------------------------------------------------
//----------------NEW CHAR----------------------------------
		cout << "Add New char? : ";
		cin >> indAddChar;
		if (indAddChar) {
			char newCharS[30];
			char FinalCharS[45];
			cin >> newCharS;
			openFiles(newCharS, root);//OPEN FILES FOR NEW CHARS                                    ||дл€ старых букв файлы уже занесены в дерево
			strcpy_s(FinalCharS, plyerLetters);//final chars = old chars + new chars				
			strcat_s(FinalCharS, newCharS);//ALL CHARS
			plyerLetters = FinalCharS;
			system("cls");
			showArreyChar(plyerLetters);
			showArreyChar(newCharS);
			search(root, plyerLetters, k, ptrword);//search with new chars							//ищем слова которые можно составить из всех букв
			sortByPointUP(ptrword, k);
			showStrPoint(ptrword, k);
		}
//----------------NEW CHAR----------------------------------
	} while (indAddChar);
	deleteTrie(root);
	delete[] ptrword;
	system("pause");

}


//########################################################################################//
int GetIndex(char c) {//индекс буквы в масиве дерева. Input - char. Output - int;
//-------------ќ—ќЅџ≈ —Ћ”„ј» »Ќƒ≈ —ќ¬--------------------
	if (c == '-') {
		return 26;
	}
	if (c == '\'') {
		return 27;
	}
	if (c == '?') {
		return 28;
	}
	if (c == '\\') {
		return 29;
	}
//----------------------------------------------------------
	return tolower(c) - 'a';//ASCI(нижний регистр буквы) - 66
}


//########################################################################################// o

void openFiles(char *plyerLetters, struct node *root) {
	//структура файлов [word]  [about]
	//a.txt b.txt .... z.txt

	string filename;
	string txt = ".txt";
	string word;
	string about;
	vector <string> v2;
	//--------------------------------OPEN FILE/ INPUT IN TRIE---------------------------//
	char tmp;
	int w = 0;
	int ind = 0;
	//убираем одинаковые буквы в  конец масива и сдвигаем границу на +1, чтобы не заполн€ть дерево одинаковыми словами
	for (int i = 0; i < strlen(plyerLetters) - w; i++) {
		if (plyerLetters[i] == '*') {
			ind = 1;
		}
		for (int j = i + 1; j < strlen(plyerLetters) - w; j++) {
			if (plyerLetters[i] == plyerLetters[j] || plyerLetters[i] == '*' ) {
				tmp = plyerLetters[strlen(plyerLetters) - 1 - w];
				plyerLetters[strlen(plyerLetters) - 1 - w] = plyerLetters[j];
				plyerLetters[j] = tmp;
				w ++;
			}
		}
	}
	//если нету пустышки открываем файлы дл€ определенных букв
	if (!ind) {
		for (int i = 0; i < strlen(plyerLetters) - w; i++) {

			filename = plyerLetters[i] + txt;//cmp file neme = letter + '.txt'
			ifstream letter_file(filename);//open file for each letter
			if (letter_file.is_open()) {
				while (letter_file >> word)//записываем в ворд первое слово
				{
					getline(letter_file, about, '\n');//все что после запись в about
					letter_file.ignore(255, '\n');//игнорируем любой мусор до \n
					input(root, word, about);//запись слова и описание в дерево
				}
			}
			else
			{
				cout << "PHHH - PHHHH. Error 404." << filename << "Not found\n";
			}
			letter_file.close();
		}
	}
	//если есть пустышка - открываем все файлы дл€ записи в дерево(т.к. люба€ буква может быть в начале)
	else {
		for (int i = 0; i < 26; i++) {

			filename = WHOLELETTERS[i] + txt;//cmp file neme = letter + '.txt'
			ifstream letter_file(filename);//open file for each letter
			if (letter_file.is_open()) {
				while (letter_file >> word)
				{
					getline(letter_file, about, '\n');
					letter_file.ignore(255, '\n');
					input(root, word, about);
				}
			}
			else
			{
				cout << "PHHH - PHHHH. Error 404." << filename << "Not found\n";
			}
			letter_file.close();
		}
	}
}

//########################################################################################//
strWord* addStrWord(const int amount, strWord word[]) {//добавление слова в структуру слов (очень медлено .. нужно заменить на св€зный список)
	if (!amount) {
		word = new strWord[amount + 1];
	}
	else {
		strWord* tempObj = new strWord[amount + 1];

		for (int i = 0; i < amount; i++)
		{
			tempObj[i] = word[i]; // копируем во временный объект
		}
		delete[] word;

		word = tempObj;
		//delete[] tempObj;
	}
	return word;
}

int getPointWord(string Word) {
	int len = Word.length();
	int count = 0;
	int pointArr[] = { 1,3,3,4,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10 };//очки за слова 
	for (int i = 0; i < len; i++) {
		if (GetIndex(Word[i]) < 26)
			count += pointArr[GetIndex(Word[i])];

	}
	return count;
}
void setStrWord(strWord Obj[], const int amount, string word, string about)
{

	Obj[amount].about = about;
	Obj[amount].Word = word;
	Obj[amount].point = getPointWord(Obj[amount].Word);

}
void showStrWord(strWord Obj[], int amount)
{
	for (int i = 0; i < amount; i++) {
		cout << Obj[i].Word << "\t";
		cout << Obj[i].about << "\t";
		cout << Obj[i].point << endl;
	}

}

void showStrPoint(strWord Obj[], int amount)
{
	for (int i = 0; i < amount; i++) {
		cout << Obj[i].point << "\t";
		cout << Obj[i].Word << "\t";
		cout << Obj[i].about << endl;
	}

}
//########################################################################################//	




void sortByFirstLetterDW(strWord Obj[],const int amount) {//быстра€ сортировка(по всем буквам)
	struct strWord * temp = new strWord [amount];
	for (int i = 1; i < amount; i++) {
		for (int i = 1; i < amount; i++) {
			if (strcmp(Obj[i - 1].Word.c_str(), Obj[i].Word.c_str()) >= 0 ) {
				*temp = Obj[i - 1];
				Obj[i - 1] = Obj[i];
				Obj[i] = *temp;


				
				//showStrWord(Obj, amount);
			}
		}
	}
	delete[]temp;
	//showStrWord(Obj, amount);
}

void sortByFirstLetterUP(strWord Obj[], const int amount) {//быстра€ сортировка(по всем буквам)
	struct strWord * temp = new strWord[amount];
	for (int i = 1; i < amount; i++) {
		for (int i = 1; i < amount; i++) {
			if (strcmp(Obj[i - 1].Word.c_str(), Obj[i].Word.c_str()) < 0) {
				*temp = Obj[i - 1];
				Obj[i - 1] = Obj[i];
				Obj[i] = *temp;
				//showStrWord(Obj, amount);
				
			}
		}
	}
	delete []temp;
	//showStrWord(Obj, amount);
	
}

void sortByPointUP(strWord Obj[], const int amount) {
	struct strWord * temp = new strWord[amount];
	for (int i = 1; i < amount; i++) {
		for (int i = 1; i < amount; i++) {
			if (Obj[i - 1].point < Obj[i].point) {
				*temp = Obj[i - 1];
				Obj[i - 1] = Obj[i];
				Obj[i] = *temp;
				//showStrWord(Obj, amount);
			}
		}
	}
}


void sortByPointDW(strWord Obj[], const int amount) {
	struct strWord * temp = new strWord[amount];
	for (int i = 1; i < amount; i++) {
		for (int i = 1; i < amount; i++) {
			if (Obj[i - 1].point > Obj[i].point) {
				*temp = Obj[i - 1];
				Obj[i - 1] = Obj[i];
				Obj[i] = *temp;
				//showStrWord(Obj, amount);
			}
		}
	}
}

void showArreyChar(char * arrey)
{
	for (int i = 0; i < strlen(arrey); i++) {
		cout << "| " << arrey[i] << " ";
	}
	cout <<"|"<< endl;
}










//########################################################################################//
void input(struct node *root, string word, string about) {
	struct node *crawl = root;
	int len = word.length();
	for (int i = 0; i < len; i++)
	{
		int ind = GetIndex(word[i]);
		if (!crawl->child[ind])
		{
			crawl->child[ind] = newnode();
		}
		crawl = crawl->child[ind];

	}
	crawl->about = about;
	crawl->word = word;
}
//########################################################################################//




//########################################################################################//
char * deleteElement(char *letters, char c) {
	char * newarr = new char[strlen(letters) - 1];
	unsigned int i;
	for (i = 0; i < strlen(letters); i++) {
		if (letters[i] == c) break;
	}
	for (unsigned int j = 0; j < i; j++) {
		newarr[j] = letters[j];
	}
	for (unsigned int j = i + 1; j < strlen(letters); j++) {
		newarr[j - 1] = letters[j];
	}

	newarr[strlen(letters) - 1] = '\0';
	//delete letters;
	return newarr;
}
//########################################################################################//




//########################################################################################//
//if char exist in plyer Letters
bool inarrey(char* letters, char h) {
	
for (unsigned int i = 0; i < strlen(letters); i++) {
		if (letters[i] == '*') {
			return true;
		}
		if (letters[i] == h) {
			return true;
		}
	}
	return false;
}
//########################################################################################//




//########################################################################################//
//search in trie
//рекурсивна€ формула дл€ поиска, провеки, и поиска следу€ по ссылке
//	        
//	проход по масиву	
//	| 0x | 0x | 0x | . | . | . |  |  |  |...31
//	   |	
//      ->это слово?
//      ->есть ли индекс буквфы в масиве данном пользывателем
//		->переходим по этой ссылке на след. масив
//		loop
//
//
//
//
void search(struct node *node, char *letters, int &K, strWord*  &word) {
	for (int i = 0; i < LETTERS; i++) {
		if (node->word != "" && !node->leaf) {
			word = addStrWord(K, word);
			setStrWord(word, K, node->word, node->about);
			K++;
			//cout << node->word << node->about << "\n";
			node->leaf = true;
		}
		if (node->child[i] && inarrey(letters, '*')) {
			search(node->child[i], deleteElement(letters, '*'), K, word);
		}
		if (node->child[i] && inarrey(letters, WHOLELETTERS[i]) ) {//|| node->child[i] && inarrey(letters, '*')
			search(node->child[i], deleteElement(letters, WHOLELETTERS[i]), K, word);
		}
		
	}
}
//########################################################################################//





//########################################################################################//
void deleteTrie(struct node *node) {
	for (int i = 0; i < LETTERS; i++) {
		if (node->child[i]) {
			deleteTrie(node->child[i]);
			node->child[i] = NULL;
			node->about = "";
			node->word = "";
		}
	}
}
//########################################################################################//



