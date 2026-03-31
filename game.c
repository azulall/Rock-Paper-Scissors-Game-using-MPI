#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

const char* to_rps(int move){ //it converts numbers to string values (rock, paper, scissors)
           if(move == 0) return "ROCK";
           if(move == 1) return "PAPER";
           return "SCISSORS";
}    
int determine_winner(int move0, int move1){
	if(move0 == move1) return -1;
	if((move0 == 2 && move1 == 1) || (move0 == 1 && move1 == 0) || (move0 == 0 && move1 == 2) ) return 0;
        return 1;
}
int main(int argc, char** argv){
	MPI_Init(&argc, &argv);

	int rank, size;
	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	srand(time(NULL)+ rank);
	
	int mode;

	if(rank == 0){
	  printf("********** The game starts! **********\n");
	  printf("Select mode: 0 (P2P) OR 1 (Collective): ");
    	  scanf("%d", &mode);
	}
	MPI_Bcast(&mode,1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(rank == 0){
	  printf("Mode: %d\n", mode);
	}
	
	int move;
	int opponent_move;	
	int all_moves[2];//for collective comm
	
	int score0 = 0;
        int score1 = 0;
        int turn = 1;

	int game_over = 0;	

	while(!game_over){
	 move =  rand() % 3;
	 if(mode ==0){//mode 0 P2P	

	  if(rank == 0){
        	MPI_Recv(&opponent_move, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
          	
		int winner = determine_winner(move, opponent_move);
		
 		printf("---------- Turn %d ---------- \n",turn);
                printf("Process 0: %s VS Process 1: %s\n",to_rps(move), to_rps(opponent_move));
		
		if(winner == -1){
    			printf("Draw, replay round. Score: %d - %d\n", score0, score1);
		}
		else if(winner == 0){
    			score0++;
    			printf("Process 0 wins, Score: %d - %d\n", score0, score1);
			printf("------------------------------\n\n");
			turn++;
		}
		else{
    			score1++;
    			printf("Process 1 wins, Score: %d - %d\n", score0, score1);
			printf("------------------------------\n\n");
			turn++;		
		}
		
		if(score0 == 5 || score1 == 5){
 		   game_over = 1;	
		}
		
		MPI_Send(&game_over, 1, MPI_INT, 1, 0,MPI_COMM_WORLD);
		
	
          }else{ // rank is 1
        
          	MPI_Send(&move, 1, MPI_INT, 0, 0, MPI_COMM_WORLD );
		MPI_Recv(&game_over, 1 ,MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          }

	 }else{//mode 1: collective communication
	 	MPI_Gather(&move, 1, MPI_INT, all_moves, 1, MPI_INT, 0, MPI_COMM_WORLD);

    		if (rank == 0) {
        	  int move0 = all_moves[0];
        	  int move1 = all_moves[1];

        	  int winner = determine_winner(move0, move1);
		  
		  printf("---------- Turn %d ---------- \n",turn);
               	  printf("Process 0: %s VS Process 1: %s\n",to_rps(move), to_rps(opponent_move));
                
		  if(winner == -1){
                        printf("Draw, replay round. Score: %d - %d\n", score0, score1);
		  }
                  else if(winner == 0){
                        score0++;
                        printf("Process 0 wins, Score: %d - %d\n", score0, score1);
                        printf("------------------------------\n\n");
                        turn++;
                  }
                  else{
                        score1++;
                        printf("Process 1 wins, Score: %d - %d\n", score0, score1);
                        printf("------------------------------\n\n");
                        turn++;
                  }
                
                  if(score0 == 5 || score1 == 5){
                     game_over = 1;
                  }           
    		}
		
    		  MPI_Bcast(&game_over, 1, MPI_INT, 0, MPI_COMM_WORLD);
	 }
	
	}

	if (rank == 0) {
		if(score0 == 5){
        		printf("PROCESS 0 has won the game!\nScore: %d - %d\nTurn: %d\n",score0,score1,turn-1);
         	}
         	else{
                	printf("PROCESS 1 has won the game!\nScore: %d - %d\nTurn: %d\n",score0,score1,turn-1);
         	}
		printf("********** The game ends! ********** \n");
	}

	MPI_Finalize();
	return 0;

}

