#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define PI 3.141592654

double GaussRand(double V,double E);
void DifferentNum(int item_num,int max_item,int* differedata,
				   int flag,double V,double E);
void ProData(int seq_num,int max_transtion_num,int max_item_num,
			 int max_item,int flag,double V,double E);

/*
function : generate different numbers
parameter : @1 item nums @2 max item id @3 data @4 if use gauss 1 for yes 0 for no
						@5 means @6 variance
return     : null
*/
void  DifferentNum(int item_num,int max_item,int* differedata,
				   int flag,double V,double E){
	int i =0, j = 0;
	if (flag == 0)
		differedata[0] = rand() % max_item ;
	else{
		do{
			differedata[0] = GaussRand(V,E);
		}while(differedata[0]< 0 || differedata[0] > max_item);
	}
	for(i = 1; i < item_num; i++){
		if(flag == 0)
			differedata[i] = rand() % max_item;
		else{
			differedata[i] =GaussRand(V,E);
			//fprintf(stderr, "%d\n", differedata[i]);
		}
		for(j = 0; j < i; j++){
			if(differedata[i] == differedata[j] || differedata[i] < 0 
				|| differedata[i] > max_item){
				i--;
				break;
			}
		}
	}
}


/*
function : generate transtion data
parameter : @1 seq nums @2 max transtion nums @3 max item nums @ max item id
					  @4 if use gauss 1 for yes 0 for no @5 means @6 variance
return null
*/
void ProData(int seq_num,int max_transtion_num,int max_item_num,
			 int max_item,int flag,double V,double E){
	int i = 0, j = 0, k = 0;
	int* differentdata = NULL;
	int transtion_num = 0, item_num = 0;
	srand((int)time(0));
	differentdata = (int*)malloc(max_item_num * sizeof(int));
	for(i = 0; i <  seq_num; i++){
		transtion_num = rand()%max_transtion_num+1;
		for(j = 0; j < transtion_num; j++){
			printf("%d ",i);
			item_num = rand() % max_item_num + 1;
			printf("%d ",item_num);
			DifferentNum(item_num,max_item,differentdata,flag,V,E);
			for(k = 0; k < item_num; k++){
				printf("%d ",differentdata[k]);
			}
			printf("\n");
		}
	}
	free(differentdata);
}

double GaussRand(double V,double E)
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if ( phase == 0 ) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while(S >= 1 || S == 0);
		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;

	return X = X * V + E;
}

int main()
{
	freopen("../data/gen.data", "w+", stdout);
	srand((int)time(0)); 
	ProData(1000,20,40,600,1,200,20);
	
	return 0;
}