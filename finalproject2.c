#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define STOPNUM 876
#define PRONUM 2068
#define NEGNUM 4951
#define PUNCTNUM 26
#define REVIEWFILENUM 263
#define NOTNUM 21

void ReadFile(char*);
char(*Stop_dictionary(void))[20];
char(*Positive_dictionary(void))[20];
char(*Negative_dictionary(void))[20];
char(*Punct_dictionary(void))[2];
char(*Not_Words(void))[20];
char(*Token_Review(FILE* fp1))[150][100];
int Token_line(FILE* fp1);
//char(*Token_Review(FILE* fp1))[100];
//int Token_Review_Num(FILE* fp1);
char TokenToLower(char**);
char(*NoPunctToken(char* []))[100];
void Review_Point(int, int);

/*���� �� �ܾ��*/
int count_array[50] = { 0, };

/*���� �迭�� ����*/
int star_array[11] = { 0, };

int main() {
	int choice;
	printf("************************************\n");
	printf("\n1. Ư�� ���� ������ ���� Ȯ��\n");
	printf("2. ��� ���� ������ ���� Ȯ��\n");
	printf("\n�Է�: ");
	scanf("%d", &choice);
	printf("\n************************************\n");

	switch (choice) {
		//���� ���� �ϳ���
	case 1:
	{
		int file_num;
		char file_name[10];
		printf("\n���� ���� ��ȣ �Է�: ");
		scanf("%d", &file_num);
		sprintf(file_name, "%d", file_num);
		strcat(file_name, ".txt");
		ReadFile(file_name);
		printf("\n\n");
		break;
	}

	//��� ���� ���� ����
	case 2:
	{
		printf("��� ���� ���� ����\n");
		int file_num;
		int Total_ReviewPoint = 0;
		for (file_num = 0; file_num <= REVIEWFILENUM; file_num++) {
			char file_name[10];
			sprintf(file_name, "%d", file_num);
			strcat(file_name, ".txt");
			ReadFile(file_name);
			//printf("%s\n", file_name);
		}
		printf("\n----------------------------------------------------\n");

		for (int i = 0; i <= 10; i++) {
			printf("\n���� %d�� ����: %d\n", i, star_array[i]);
			Total_ReviewPoint += (i * star_array[i]);
		}
		printf("\n----------------------------------------------------\n");
		int aver_TotalStar = round((double)Total_ReviewPoint / (double)REVIEWFILENUM);
		printf("\n�� ���� ���� ���: %d  ", aver_TotalStar);
		for (int i = 0; i < aver_TotalStar; i++)
			printf("��");
		for (int i = 0; i < aver_TotalStar; i++)
			printf("��");
		printf("\n\n");

	}

	}
}

void ReadFile(char *file_name) {

	char(*StopWords)[20] = Stop_dictionary();
	char(*PositiveWords)[20] = Positive_dictionary();
	char(*NegativeWords)[20] = Negative_dictionary();
	char(*PunctWords)[2] = Punct_dictionary();
	char(*NotWords)[20] = Not_Words();

	/*���� ���� ����*/
	FILE* fp1;
	fp1 = fopen(file_name, "rt");
	if (fp1 == NULL) {
		puts("���� ���� ����!");
		return -1;
	}
	printf("\n-------------------------------\n");
	printf("\n<%s>", file_name);


	/*���� ���� �ܾ� ������ ���ڿ� �迭�� ����*/
	char(*Tokens)[150][100] = Token_Review(fp1);

	/*���尳�� ����*/
	fseek(fp1, 0, SEEK_SET);
	int lineNum = Token_line(fp1);
	
		
	/*�빮�ڸ� �ҹ��ڷ� ��ȯ*/
	int c = 0;
	for (int a = 0; a < lineNum; a++) {
		for (int b = 0; b < count_array[a]; b++) {
			c = 0;
			while (Tokens[a][b][c] != NULL) {
				if (isupper(Tokens[a][b][c])) {
					Tokens[a][b][c] = tolower(Tokens[a][b][c]);
				}
				c++;
			}
		}
	}

	/*not�� ���Ե� ������ ó��*/
	int tokenword;
	int notindex;
	for (int l = 0; l < lineNum; l++) {
		for (tokenword = 0; tokenword < count_array[l]; tokenword++) {
			for (notindex = 0; notindex < 18; notindex++) {
				if (!strcmp(Tokens[l][tokenword], NotWords[notindex])) {
					strcpy(Tokens[l][tokenword], "not");
				}
			}
		}
	}

	/*���� ��ȣ ����*/
	int tokenIndex1 ;
	int tokenIndex2 ;
	int tokenIndex3;
	int pIndex = 0;

	for (tokenIndex1 = 0; tokenIndex1 < lineNum; tokenIndex1++) {
		for (tokenIndex2 = 0; tokenIndex2 < count_array[tokenIndex1]; tokenIndex2++)
		{
			tokenIndex3 = 0;
			while (Tokens[tokenIndex1][tokenIndex2][tokenIndex3] != NULL)
			{
				for (pIndex = 0; pIndex < PUNCTNUM; pIndex++) {
					if (Tokens[tokenIndex1][tokenIndex2][tokenIndex3] == PunctWords[pIndex][2]) {
						Tokens[tokenIndex1][tokenIndex2][tokenIndex3] = NULL;
					}
				}
				tokenIndex3++;
			}
		}
	}


	/*�ҿ�� ����*/
	//int tokenword;
	int spIndex;

	for (int l = 0; l < lineNum; l++) {
		for (tokenword = 0; tokenword < count_array[l]; tokenword++) {
			for (spIndex = 0; spIndex < STOPNUM; spIndex++) {
				if (!strcmp(Tokens[l][tokenword], StopWords[spIndex])) {
					Tokens[l][tokenword][0] = NULL;

				}
			}
		}
	}

	int checkIndex;
	int blankIndex;
	
	for (int i = 0; i < 3; i++) {
		for (int l = 0; l < lineNum; l++) {
			for (checkIndex = 0; checkIndex < count_array[l]; checkIndex++) {
				if (Tokens[l][checkIndex][0] == NULL) {

					for (blankIndex = checkIndex; blankIndex < count_array[l]; blankIndex++) {
						strcpy(Tokens[l][blankIndex], Tokens[l][blankIndex + 1]);
					}
					count_array[l]--;
				}
			}
		}
	}
	

	printf("\n\n");
	printf("lineNum: %d  ", lineNum);
	for (int i = 0; i < lineNum; i++) {
		printf("%d��°-%d ", i,count_array[i]);
	}
	printf("\n\n");


	//�ҿ�� �������� Ȯ�� ���� 
	printf("\n\n");
	for (int l = 0; l < lineNum; l++) {
		for (int q = 0; q < count_array[l]; q++) {
			printf("%s ", Tokens[l][q]);
		}
		printf("\n");
	}



	/*����, ���� �ܾ� ����*/
	int positiveTokenNum = 0;
	int negativeTokenNum = 0;
	int linestop = 0;
	int wordstop = 0;
	int originposnum = 0;
    int originnegnum = 0;

	/*not�� ����� Ȯ���ϱ�. �� ������ ī��Ʈ �ϰ� ����?*/
	for (int l = 0; l < lineNum; l++) {
		
		for (tokenword = 0; tokenword < count_array[l];tokenword++ )
		{
			originposnum = positiveTokenNum;
			originnegnum = negativeTokenNum;
			if (strcmp(Tokens[l][tokenword], "not")) {
				wordstop = tokenword+1;
				linestop = count_array[l];
				for (wordstop; wordstop < linestop; wordstop++) {
					for (spIndex = 0; spIndex < PRONUM; spIndex++) {
						if ((strcmp(Tokens[l][wordstop], PositiveWords[spIndex])) == 0) {
							negativeTokenNum++;
							Tokens[l][wordstop][0] = NULL;
							Tokens[l][tokenword][0] = NULL;
							break;
							//������ ������������ 
						}
					}
					for (spIndex = 0; spIndex < NEGNUM; spIndex++) {
						if ((strcmp(Tokens[l][wordstop], NegativeWords[spIndex])) == 0) {
							positiveTokenNum++;
							Tokens[l][wordstop][0] = NULL;
							Tokens[l][tokenword][0] = NULL;
							break;
						}
					}
					if (originposnum != positiveTokenNum || originnegnum != negativeTokenNum) break;
					
				}				
			}
		}
			
	}
	for (int l = 0; l < lineNum; l++) {
		for (tokenword = 0; tokenword < count_array[l]; tokenword++) {
			for (spIndex = 0; spIndex < PRONUM; spIndex++) {
				if ((strcmp(Tokens[l][tokenword], PositiveWords[spIndex])) == 0) {
					positiveTokenNum++;
				}
			}
			for (spIndex = 0; spIndex < NEGNUM; spIndex++) {
				if ((strcmp(Tokens[l][tokenword], NegativeWords[spIndex])) == 0) {
					negativeTokenNum++;
				}
			}
		}
	}
			

	printf("\n���� �ܾ� ����: %d\n", positiveTokenNum);
	printf("���� �ܾ� ����: %d\n", negativeTokenNum);

	/*���� ����(10�� ����)*/
	Review_Point(positiveTokenNum, negativeTokenNum);
	printf("\n");


	fclose(fp1);
}


/*�ҿ�� �ܾ �ҿ�� �迭�� ����ִ� �Լ�*/
char(*Stop_dictionary(void))[20] {
	FILE* stopfp;
	int i = 0;
	static char StopWord[STOPNUM][20]; //�ҿ�� ���� �迭 �����

	stopfp = fopen("stop_words_english.txt", "r");
	if (stopfp == NULL) {
		printf("���� ���� ����\n");
		exit(1);
	}

	while (!feof(stopfp)) {
		fscanf(stopfp, "%s", StopWord[i]);
		i++;
	}

	fclose(stopfp);
	return StopWord;
}

/*Not ���Ե� �ܾ �迭�� �ִ� �Լ�*/
char(*Not_Words(void))[20] {
	FILE* notfp;
	int n = 0;
	static char NotWord[NOTNUM][20]; 
	notfp = fopen("notword.txt", "r");
	if (notfp == NULL) {
		printf("���� ���� ����\n");
		exit(1);
	}
	while (!feof(notfp))
		fscanf(notfp, "%s", NotWord[n++]);
	fclose(notfp);
	return NotWord;
}


/*������ �ܾ ������ �迭�� ����ִ� �Լ�*/
char(*Positive_dictionary(void))[20] {
	FILE* posfp;
	int p = 0;
	static char PositiveWord[PRONUM][20]; //������ ���� �迭 �����
	posfp = fopen("positive.txt", "r");
	if (posfp == NULL) {
		printf("���� ���� ����\n");
		exit(1);
	}
	while (!feof(posfp))
		fscanf(posfp, "%s", PositiveWord[p++]);
	fclose(posfp);
	return PositiveWord;
}

/*������ �ܾ ������ �迭�� ����ִ� �Լ�*/
char(*Negative_dictionary(void))[20] {
	FILE* negfp;
	int n = 0;
	static char NegativeWord[NEGNUM][20];
	negfp = fopen("negativeword.txt", "r");
	if (negfp == NULL) {
		printf("���� ���� ����\n");
		exit(1);
	}
	while (!feof(negfp))
		fscanf(negfp, "%s", NegativeWord[n++]);
	fclose(negfp);
	return NegativeWord;
}

/*�����ȣ�� �����ȣ �迭�� ����ִ� �Լ�*/
char(*Punct_dictionary(void))[2] {
	FILE* punctfp;
	int q = 0;
	static char PunctWord[PUNCTNUM][2]; 

	punctfp = fopen("punct.txt", "r");
	if (punctfp == NULL) {
		printf("���� ���� ����\n");
		exit(1);
	}

	while (!feof(punctfp)) {
		fscanf(punctfp, "%s", PunctWord[q++]);
	}

	fclose(punctfp);
	return PunctWord;
}

/*���� ������ ���� ���� ���ڿ� �迭�� �����ϴ� �Լ�*/
char(*Token_Review(FILE* fp1))[150][100]{
		char FileWords[100][1000] = { 0, };
		char buffer[1000];
		char* token;
		int line = 0;

		while (!feof(fp1)) {
			fgets(buffer, 1000, fp1);
			token = strtok(buffer, "\n!,");
			while (token != NULL) {
				strcpy(FileWords[line++], token);
				//printf("%s\n", token);
				token = strtok(NULL, "\n");
			}
		}

		//printf("\n%s\n", FileWords[0]);
		printf("%d", line);

		char realFileWords[100][150][100] = { 0, };
		char* finaltoken;
		int count;
		//int count_array[50] = { 0, };

		for (int i = 0; i < line; i++) {
			
			count = 0;
			printf("\n\n%d��° ����", i);
			printf("\n%s\n", FileWords[i]);
			finaltoken = strtok(FileWords[i], " ");
			//printf("\n%s\n", FileWords[i]);
			while (finaltoken != NULL) {
				strcpy(realFileWords[i][count], finaltoken);
				//printf("\n%s", finaltoken);
				printf("\n%s", realFileWords[i][count]);
				finaltoken = strtok(NULL, " ");
				count++;

			}
			count_array[i] = count;
			

		}
		printf("\n\n");
		for (int i = 0; i < line; i++) {
			printf("%d ", count_array[i]);
		}
		printf("\n\n");

		return realFileWords;
}

/*���� �� ���� ����*/
int Token_line(FILE* fp1) {
	char FileWords[100][1000] = { 0, };
	char buffer[1000];
	char* token;
	int line = 0;
	int count = 0;
	while (!feof(fp1)) {
		fgets(buffer, 1000, fp1);
		token = strtok(buffer, "\n!,");
		while (token != NULL) {
			strcpy(FileWords[line++], token);
			token = strtok(NULL, "\n");
		}
	}


	return line;
}


/*���� ��ū�� �� �ܾ� �� ��� �Լ�*/
//int Token_Review_Num(FILE* fp1) {
//	char FileWords[1500][100] = { 0, };
//	char buffer[100];
//	char* token;
//	int count = 0;
//	while (!feof(fp1)) {
//		fgets(buffer, 100, fp1);
//		token = strtok(buffer, " ");
//		while (token != NULL) {
//			strcpy(FileWords[count++], token);
//			token = strtok(NULL, " ");
//		}
//	}
//	return count;
//}


/*���� ����Ʈ ���*/
void Review_Point(int posTokNum, int negTokNum) {
	//���� �ڸ����� �ݿø�. ex) (5/(13+5))*5 -> round -> 2��
	double totalnum = (double)posTokNum + (double)negTokNum;

	int review_Point = round(10 * ((double)posTokNum / totalnum));
	printf("������ ���� ����: %d��", review_Point);

	//�� ���
	for (int i = 0; i < review_Point; i++)
		printf("��");
	for (int i = 0; i < (10 - review_Point); i++)
		printf("��");
	printf("\n");

	star_array[review_Point]++;

}