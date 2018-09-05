// Joshua Acosta - 501 - 05-03-17
// PROJECT BABY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int menu, playerCount;
FILE *fp;

char name[20];
int balance, gain, point;

void getMenuChoice(int menu);
int doRoll(int,int);
void topUp();
void playGame(int);
void getTopBalance();
void getTopWins();
void getGameInfo();
void updateInfo();

struct
{
	char name[20];
	int balance;
	int gain;
} player[10],holder,currentPlayer;

int main()
{
	getGameInfo();
	fp=fopen("example.txt","r+");
	
	for(;;)
	{
		printf("\n0) top up\n1) play game\n2) top five by balance\n3) top five by winnings\n4) exit\n");
		printf("Choose an option: ");
		
		scanf("%d",&menu);
		getchar();
		getMenuChoice(menu);
	}	
}

void getGameInfo()
{
	fp=fopen("example.txt","r+");
	if(fp==NULL)
	{
		printf("File could not be found\n");
		fclose(fp);
	}
	
	for(int i=0;;i++)
	{
		if(feof(fp))
		{
			fclose(fp);
			break;
		}
		else
		{
			fscanf(fp,"%s\t%d\t%d",&player[i].name,&player[i].balance,&player[i].gain);
		}
		playerCount++;
	}
}

void getMenuChoice(int menu)
{
	switch(menu)
	{
			case 0: topUp();break;
			case 1: playGame(0);break;
			case 2: getTopBalance();break;
			case 3: getTopWins(); break;
			case 4: fclose(fp);exit(1);break;
	}
}

void topUp()
{
	printf("\n\nEnter your name: ");
	scanf("%s",&name);
	getchar();
	
	printf("Enter the amount you'd like to put into your balance: ");
	scanf("%d",&balance);
	getchar();
	
	for(int i=0;i<10;i++)
	{
		if(i>=playerCount)//if new player
		{
			printf("Welcome new player!\n");
			playerCount++;
			player[i-1].balance=balance;
			strcpy(player[i-1].name,name);
			printf("Your starting balance is now: %d \n\n",player[i-1].balance);
			updateInfo();
			
			return;
		}
		else
		{
			for(int j=0;j<20;j++)
			{
				if(name[j]!=player[i].name[j])
					break;
				else if(name[j]==player[i].name[j] && j==19)
				{
					printf("Your balance is now: %d \n\n",(player[i].balance+=balance));
					updateInfo();
					return;
				}	
			}
		}	
	}
}

void playGame(int check)
{
	int outcome, answer,current;

	if(check==0)
	{
		printf("\nEnter your name: ");
		scanf("%s",&name);
		getchar();
	
		for(int i=0; i<10; i++)
		{
			if(i>=playerCount)
			{
				printf("Player not found, enter an existing name\n");
				playGame(0);
				break;
			}
		
			for(int j=0;j<20;j++)
			{
				if(name[j]!=player[i].name[j])
					break;
				else if(name[j]==player[i].name[j] && j==strlen(player[i].name))
				{
					currentPlayer=player[i];
					current=i;
					i=20;
					break;
				}
			}	
		}
	}
	
	for(int i=0;;i++)
	{
		printf("Press enter to roll the dice\n");
		getchar();
		int dice1=rand()%6+1;
		int dice2=rand()%6+1;
		int sum=dice1+dice2;
		printf("The value of the dice is %d and %d with a sum of %d, point: %d\n",dice1,dice2,sum,point);
		
		outcome= doRoll(i,sum);
		if(outcome==0)
		{
			currentPlayer.balance-=1;
			currentPlayer.gain-=1;
			printf("Sorry, you lose. Your current balance is now: %d\n",currentPlayer.balance);
			break;
		}
		else if(outcome==2)
		{
			currentPlayer.balance+=10;
			currentPlayer.gain+=10;
			printf("You win! Your current balance is now: %d\n", currentPlayer.balance);
			printf("Would you like to play again? (y/n): \n");
			scanf("%s",&answer);
			
			if(answer=='y')
			{
				printf("\n");
				playGame(1);
			}
			else
			{
				player[current].balance= currentPlayer.balance;
				player[current].gain = currentPlayer.gain;
				updateInfo();		
				
				break;
			}
		}
	}
}

int doRoll(int turn, int sum)
{
	if(turn==0)
	{
		if(sum==7||sum==11)
			return 2;
		else if(sum<4||sum==12)
			return 0;
		else
		{
			point=sum;
			return 1;
		}
	}
	else
	{
		if(sum==7)
			return 0;
		else if(point==sum)
			return 2;
		else	
			return 1;
	}
}

void getTopBalance()
{
	for(int i=0;i<playerCount;i++)
	{
		for(int j=0;j<playerCount;j++)
		{
			if(player[i].balance > player[j].balance)
			{
				holder=player[j];
				player[j]=player[i];
				player[i]=holder;
			}
		}
	}
	
	printf("\n");
	for(int i=0;i<5;i++)
	{
		printf("%d) ",i+1);
		printf("%s with a balance of %d \n",player[i].name, player[i].balance);
	}
}

void getTopWins()
{
	for(int i=0;i<playerCount;i++)
	{
		for(int j=0;j<playerCount;j++)
		{
			if(player[i].gain > player[j].gain)
			{
				holder=player[j];
				player[j]=player[i];
				player[i]=holder;
			}
		}
	}
	
	printf("\n");
	for(int i=0;i<5;i++)
	{
		printf("%d) ",i+1);
		printf("%s with a gain of %d \n",player[i].name, player[i].gain);
	}
}

void updateInfo()
{	
	fp=fopen("example.txt","w");
	fp=fopen("example.txt","r+");
		
	for(int i=0;i<playerCount-1;i++)
	{
		fprintf(fp,"%s\t%d\t%d\r\n",player[i].name,player[i].balance,player[i].gain);
	}
	
}