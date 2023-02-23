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

/*문장 당 단어수*/
int count_array[50] = { 0, };

/*별점 배열에 저장*/
int star_array[11] = { 0, };

int main() {
	int choice;
	printf("************************************\n");
	printf("\n1. 특정 리뷰 파일의 별점 확인\n");
	printf("2. 모든 리뷰 파일의 별점 확인\n");
	printf("\n입력: ");
	scanf("%d", &choice);
	printf("\n************************************\n");

	switch (choice) {
		//리뷰 파일 하나만
	case 1:
	{
		int file_num;
		char file_name[10];
		printf("\n리뷰 파일 번호 입력: ");
		scanf("%d", &file_num);
		sprintf(file_name, "%d", file_num);
		strcat(file_name, ".txt");
		ReadFile(file_name);
		printf("\n\n");
		break;
	}

	//모든 리뷰 파일 실행
	case 2:
	{
		printf("모든 리뷰 파일 실행\n");
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
			printf("\n별점 %d개 리뷰: %d\n", i, star_array[i]);
			Total_ReviewPoint += (i * star_array[i]);
		}
		printf("\n----------------------------------------------------\n");
		int aver_TotalStar = round((double)Total_ReviewPoint / (double)REVIEWFILENUM);
		printf("\n총 리뷰 별점 평균: %d  ", aver_TotalStar);
		for (int i = 0; i < aver_TotalStar; i++)
			printf("★");
		for (int i = 0; i < aver_TotalStar; i++)
			printf("☆");
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

	/*리뷰 파일 열기*/
	FILE* fp1;
	fp1 = fopen(file_name, "rt");
	if (fp1 == NULL) {
		puts("파일 오픈 실패!");
		return -1;
	}
	printf("\n-------------------------------\n");
	printf("\n<%s>", file_name);


	/*리뷰 파일 단어 단위로 문자열 배열에 저장*/
	char(*Tokens)[150][100] = Token_Review(fp1);

	/*문장개수 세기*/
	fseek(fp1, 0, SEEK_SET);
	int lineNum = Token_line(fp1);
	
		
	/*대문자를 소문자로 변환*/
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

	/*not이 포함된 조동사 처리*/
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

	/*문장 기호 삭제*/
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


	/*불용어 삭제*/
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
		printf("%d번째-%d ", i,count_array[i]);
	}
	printf("\n\n");


	//불용어 지워진거 확인 가능 
	printf("\n\n");
	for (int l = 0; l < lineNum; l++) {
		for (int q = 0; q < count_array[l]; q++) {
			printf("%s ", Tokens[l][q]);
		}
		printf("\n");
	}



	/*긍정, 부정 단어 세기*/
	int positiveTokenNum = 0;
	int negativeTokenNum = 0;
	int linestop = 0;
	int wordstop = 0;
	int originposnum = 0;
    int originnegnum = 0;

	/*not뒤 긍부정 확인하기. 점 없으면 카운트 하고 삭제?*/
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
							//루프를 빠져나가야함 
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
			

	printf("\n긍정 단어 개수: %d\n", positiveTokenNum);
	printf("부정 단어 개수: %d\n", negativeTokenNum);

	/*긍정 별점(10점 만점)*/
	Review_Point(positiveTokenNum, negativeTokenNum);
	printf("\n");


	fclose(fp1);
}


/*불용어 단어를 불용어 배열에 집어넣는 함수*/
char(*Stop_dictionary(void))[20] {
	FILE* stopfp;
	int i = 0;
	static char StopWord[STOPNUM][20]; //불용어 넣을 배열 만들기

	stopfp = fopen("stop_words_english.txt", "r");
	if (stopfp == NULL) {
		printf("파일 열기 실패\n");
		exit(1);
	}

	while (!feof(stopfp)) {
		fscanf(stopfp, "%s", StopWord[i]);
		i++;
	}

	fclose(stopfp);
	return StopWord;
}

/*Not 포함된 단어를 배열에 넣는 함수*/
char(*Not_Words(void))[20] {
	FILE* notfp;
	int n = 0;
	static char NotWord[NOTNUM][20]; 
	notfp = fopen("notword.txt", "r");
	if (notfp == NULL) {
		printf("파일 열기 실패\n");
		exit(1);
	}
	while (!feof(notfp))
		fscanf(notfp, "%s", NotWord[n++]);
	fclose(notfp);
	return NotWord;
}


/*긍정어 단어를 긍정어 배열에 집어넣는 함수*/
char(*Positive_dictionary(void))[20] {
	FILE* posfp;
	int p = 0;
	static char PositiveWord[PRONUM][20]; //긍정어 넣을 배열 만들기
	posfp = fopen("positive.txt", "r");
	if (posfp == NULL) {
		printf("파일 열기 실패\n");
		exit(1);
	}
	while (!feof(posfp))
		fscanf(posfp, "%s", PositiveWord[p++]);
	fclose(posfp);
	return PositiveWord;
}

/*부정어 단어를 부정어 배열에 집어넣는 함수*/
char(*Negative_dictionary(void))[20] {
	FILE* negfp;
	int n = 0;
	static char NegativeWord[NEGNUM][20];
	negfp = fopen("negativeword.txt", "r");
	if (negfp == NULL) {
		printf("파일 열기 실패\n");
		exit(1);
	}
	while (!feof(negfp))
		fscanf(negfp, "%s", NegativeWord[n++]);
	fclose(negfp);
	return NegativeWord;
}

/*문장부호를 문장부호 배열에 집어넣는 함수*/
char(*Punct_dictionary(void))[2] {
	FILE* punctfp;
	int q = 0;
	static char PunctWord[PUNCTNUM][2]; 

	punctfp = fopen("punct.txt", "r");
	if (punctfp == NULL) {
		printf("파일 열기 실패\n");
		exit(1);
	}

	while (!feof(punctfp)) {
		fscanf(punctfp, "%s", PunctWord[q++]);
	}

	fclose(punctfp);
	return PunctWord;
}

/*리뷰 파일을 문장 별로 문자열 배열에 저장하는 함수*/
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
			printf("\n\n%d번째 문장", i);
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

/*리뷰 내 문장 개수*/
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


/*리뷰 토큰의 총 단어 수 계산 함수*/
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


/*리뷰 포인트 출력*/
void Review_Point(int posTokNum, int negTokNum) {
	//일의 자리까지 반올림. ex) (5/(13+5))*5 -> round -> 2점
	double totalnum = (double)posTokNum + (double)negTokNum;

	int review_Point = round(10 * ((double)posTokNum / totalnum));
	printf("리뷰의 긍정 별점: %d점", review_Point);

	//별 출력
	for (int i = 0; i < review_Point; i++)
		printf("★");
	for (int i = 0; i < (10 - review_Point); i++)
		printf("☆");
	printf("\n");

	star_array[review_Point]++;

}