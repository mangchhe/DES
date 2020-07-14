/*

작성자 : 하주현
주제 : DES 알고리즘 구현 

*/

#include <stdio.h>

unsigned int table_IP[64]=	// IP 치환 테이블
	{58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7};

unsigned int table_PC_1[56]=	// PC-1 치환 테이블
	{57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4};
	
int roundShift[16]=		// 라운드당 Shift 횟수
	{1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
	
unsigned int table_PC_2[48]=	// PC-2 치환 테이블
	{14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32};
	
unsigned int table_E[48]=		// E 치환 테이블
	{32,1,2,3,4,5,
	4,5,6,7,8,9,
	8,9,10,11,12,13,
	12,13,14,15,16,17,
	16,17,18,19,20,21,
	20,21,22,23,24,25,
	24,25,26,27,28,29,
	28,29,30,31,32,1};
	
unsigned int table_P[32]=		// P 치환 테이블
	{16,7,20,21,
	29,12,28,17,
	1,15,23,26,
	5,18,31,10,
	2,8,24,14,
	32,27,3,9,
	19,13,30,6,
	22,11,4,25};
	
unsigned int table_S[8][4][16]=	// S 치환 테이블
	{{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
	{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
	{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
	{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
	{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
	{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
	{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
	{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
	{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
	{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
	{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
	{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
	{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
	{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
	{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
	{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
	{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
	{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
	{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
	{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
	{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
	{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
	{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
	{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
	{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
	{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
	{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
	{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
	{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
	{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
	{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
	{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}};

unsigned int table_IP2[64]=	// 역 치환 테이블
	{40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25};

int keys[16][48]={0,};	// 키 값 저장
int result[48]={0,};
int result2[32]={0,};
	
void bitsPermutation(int beforeBit,int afterBit,int* table,unsigned int permutationTable[]);	
// 테이블 치환하는 함수 
void keyMaker(char words[]);	// 보조 키 만드는 함수
void function(int R[],int round);	// f 함수
void S();				// S 함수
void crypt(int ed);		// 암/복호화 하는 함수
				// ed==0; 암호화
				// ed==1; 복호화

// 메인  ------------------------------------------------------------------------------- 

int main(){
	
	int choice=0;		// 암호화/복호화 선택하는 정수입력
	char password[7]={0,};	// 키 생성
	 
	while(true){
		if(choice==2)
			break;
		printf("★☆★☆★☆★☆★☆\n");
		printf("암/복호화 선택하세요.\n");
		printf("★☆★☆★☆★☆★☆\n");
		printf("0 : 암호화\n");
		printf("1 : 복호화\n");
		printf("2 : 종료\n");
		printf("숫자를 입력하세요.");
		scanf("%d",&choice);
		getchar();	// 버퍼 버리기 because gets
		printf("키를 입력 해주세요.");
		gets(password);
		
		keyMaker(password);	// 보조 키 생성
		crypt(choice);
	}
	return 0;
}

// 함수 구현부 ------------------------------------------------------------------------------- 

void bitsPermutation(int beforeBit,int afterBit,int* table,unsigned int permutationTable[]){ // 비트 치환
	int temp[beforeBit]={0,}; 
	for(int i=0;i<beforeBit;i++){
		temp[i]=table[i];
	}
 	for(int i=0;i<afterBit;i++){
		table[i]=temp[permutationTable[i]-1];
	}
}

void keyMaker(char words[]){ // 보조키 생성
	
	int C[28]={0,};	// 보조키 C
	int D[28]={0,};	// 보조키 D
	int temp28[28]={0,};	//	C,D 값 각각 임시 저장
	int temp64[64]={0,};	//	key 값 임시 저장 
	int i=0;
	int j=0;
	int k=0;
	int key[64]={0,}; 
	int ascii[7]={0,};	// 아스키코드로 변환 
//---------------------------------------------- 패리티 비트 넣기
	for(i=0;i<7;i++){	// 문자->아스키코드 
		ascii[i]=words[i];
	}
	for(i=0;i<7;i++){	// 비트로 변환
		for(j=7+i*8;j>=i*8;j--){
			key[j]=ascii[i]%2; 
			ascii[i]=ascii[i]/2;
		}
	}
	for(j=0;j<7;j++){
		int sum=0;	// 1의 갯수 홀수인지 짝수인지 판단
		for(i=0;i<64;i++){	// 쉬프트 연산 
			temp64[i]=key[i];
		}
		sum=key[0+j*8]+key[1+j*8]+key[2+j*8]+key[3+j*8]+key[4+j*8]+key[5+j*8]+key[6+j*8];
		if(sum%2==0){
			key[7+j*8]=0;
		}
		else{
			key[7+j*8]=1;
		}
		for(i=7+j*8;i<64;i++){
			key[i+1]=temp64[i];
		}
	sum=key[56]+key[57]+key[58]+key[59]+key[60]+key[61]+key[62];
		if(sum%2==0){
			key[63]=0;
		}
		else{
			key[63]=1;
		}
	} 
//--------------------------------------------------- 패리티 비트 끝 
	bitsPermutation(64,56,key,table_PC_1);

	for(i=0;i<28;i++){	// 비트 나누기 
		C[i]=key[i];
		D[i]=key[i+28];
	}
	for(k=0;k<16;k++){	// 키 생성 반복 시작 
		for(j=0;j<roundShift[k];j++){
			for(i=0;i<28;i++){	// 쉬프트 연산 
				temp28[i]=C[i];
			}
			for(i=1;i<28;i++){
				C[i-1]=temp28[i];
				if(i==27){
					C[27]=temp28[0];
				}
			}
			for(i=0;i<28;i++){	// 쉬프트 연산 
				temp28[i]=D[i];
			}
			for(i=1;i<28;i++){
				D[i-1]=temp28[i];
				if(i==27)
					D[27]=temp28[0];
			}
		}// 쉬프트 연산 끝
		for(i=0;i<28;i++){
			key[i]=C[i];
			key[i+28]=D[i];
		}
	
		bitsPermutation(56,48,key,table_PC_2);
		
		for(i=0;i<48;i++){
			keys[k][i]=key[i];
		}

	}
}

void function(int R[],int round){
	
	int i=0;
	
	for(i=0;i<32;i++){
		result[i]=R[i];
	}

	bitsPermutation(32,48,result,table_E);
	
	for(i=0;i<48;i++){
		if(result[i]==keys[round][i]){
			result[i]=0;
		}
		else{
			result[i]=1;
		}
	}
	
	S();
	
	bitsPermutation(32,32,result2,table_P);

}

void S(){
	
	int i=0;
	int j=0;
	int sValue[8]={0,};
	int row[8]={0,};
	int column[8]={0,};
	
	for(i=0;i<8;i++){
		row[i]=result[i*6]*2+result[5+i*6]*1;	// 0,5번째 0~3
		column[i]=result[1+i*6]*8+result[2+i*6]*4+result[3+i*6]*2+result[4+i*6]*1;	
		// 1,2,3,4번 째 0~15 
	}

	for(i=0;i<8;i++){
		sValue[i]=table_S[i][row[i]][column[i]];
	}

	for(i=0;i<8;i++){	// 비트로 변환
		for(j=3+i*4;j>=i*4;j--){
			result2[j]=sValue[i]%2; 
			sValue[i]=sValue[i]/2;
		}
	}

}

void crypt(int ed){			// ed=0 암호화 
					// ed=1 복호화 
	char words[8]={0,};		// 문자 받기 
	int ascii[8]={0,};		// 아스키코드 값 저장 
	int beforeText[64]={0,};	// 아스키코드 비트열로 변환 
	int L[32]={0,};	// L(32비트) 
	int R[32]={0,}; 	// R(32비트) 
	int i=0;
	int j=0;
	int k=0;
	int afterText[64]={0,}; 
	int text[8]={0,};	// 복호화 내용 출력
	
	if(ed==0){
		gets(words);
		for(i=0;i<8;i++){	// 문자->아스키코드
			ascii[i]=words[i];
		}
		for(i=0;i<8;i++){	// 비트로 변환
			for(j=7+i*8;j>=i*8;j--){
				beforeText[j]=ascii[i]%2; 
				ascii[i]=ascii[i]/2;
			}
		}
		
	}
	else if(ed==1){
		for(i=0;i<8;i++)
			scanf("%d",&ascii[i]);
		for(i=0;i<8;i++){	// 비트로 변환
			for(j=7+i*8;j>=i*8;j--){
				beforeText[j]=ascii[i]%2; 
				ascii[i]=ascii[i]/2;
			}
		}
	}
	
	bitsPermutation(64,64,beforeText,table_IP);
	
	for(i=0;i<32;i++){		// R/L 비트 나누기 
		L[i]=beforeText[i];
		R[i]=beforeText[i+32];
	}

	//------------------------------------------------------- 16번 반복 시작
	if(ed==0){
		for(k=0;k<16;k++){
			function(R,k);			// function 함수
			for(i=0;i<32;i++){
				if(result2[i]==L[i]){
					result2[i]=0;
				}
				else{
					result2[i]=1;
				}
			}	
			for(i=0;i<32;i++){
				L[i]=R[i];
				R[i]=result2[i];
			}
		}
	}
	else if(ed==1){
		for(k=15;k>=0;k--){
			function(R,k);			// function 함수
			for(i=0;i<32;i++){
				if(result2[i]==L[i]){
					result2[i]=0;
				}
				else{
					result2[i]=1;
				}
			}
			for(i=0;i<32;i++){
				L[i]=R[i];
				R[i]=result2[i];
			}
		}
	}
	//------------------------------------------------------- 반복 끝 
	
	for(i=0;i<32;i++){ 	// 마지막은 twisted 안하고 그냥 내려옴 
		R[i]=L[i];
		L[i]=result2[i];
	}
		
	for(i=0;i<32;i++){
		afterText[i]=L[i];
		afterText[i+32]=R[i];
	}

	bitsPermutation(64,64,afterText,table_IP2);
	
	for(i=0;i<8;i++){
		text[i]=afterText[i*8]*128+afterText[1+i*8]*64+afterText[2+i*8]*32+afterText[3+i*8]*16+afterText[4+i*8]*8+afterText[5+i*8]*4+afterText[6+i*8]*2+afterText[7+i*8]*1;
	}
	
	if(ed==0){
		printf("-----암호문-----");
		for(i=0;i<64;i++){
			if(i%8==0){
				printf("\n");
			} 
			printf("%d ",afterText[i]);
		}
		printf("\n"); 
		printf("----------------");
		printf("\n");
		for(i=0;i<8;i++)
			printf("%d ",text[i]);
		printf("\n");
	}
	else if(ed==1){
		printf("------평문------\n"); 
		for(i=0;i<8;i++){
			printf("%c",(char)text[i]);
		}
		printf("\n"); 
		printf("----------------");
		for(i=0;i<64;i++){
			if(i%8==0){
				printf("\n");
			} 
			printf("%d ",afterText[i]);
		}
		printf("\n");
	}
}
